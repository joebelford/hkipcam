// Copyright (c) 2015-2019 The HomeKit ADK Contributors
//
// Licensed under the Apache License, Version 2.0 (the “License”);
// you may not use this file except in compliance with the License.
// See [CONTRIBUTORS.md] for the list of HomeKit ADK project authors.

#include "App.h"
#include "DB.h"

#include "HAP.h"
#include "HAPPlatform+Init.h"
#include "HAPPlatformAccessorySetup+Init.h"
#include "HAPPlatformBLEPeripheralManager+Init.h"
#include "HAPPlatformKeyValueStore+Init.h"
#include "HAPPlatformMFiHWAuth+Init.h"
#include "HAPPlatformMFiTokenAuth+Init.h"
#include "HAPPlatformRunLoop+Init.h"
#if IP
#include "HAPPlatformServiceDiscovery+Init.h"
#include "HAPPlatformTCPStreamManager+Init.h"
#endif

#include <signal.h>
static bool requestedFactoryReset = false;
static bool clearPairings = false;

#define PREFERRED_ADVERTISING_INTERVAL (HAPBLEAdvertisingIntervalCreateFromMilliseconds(417.5f))

/**
 * Global platform objects.
 * Only tracks objects that will be released in DeinitializePlatform.
 */
static struct {
    HAPPlatformKeyValueStore keyValueStore;
    HAPAccessoryServerOptions hapAccessoryServerOptions;
    HAPPlatform hapPlatform;
    HAPAccessoryServerCallbacks hapAccessoryServerCallbacks;

    HAPPlatformTCPStreamManager tcpStreamManager;

    HAPPlatformMFiHWAuth mfiHWAuth;
    HAPPlatformMFiTokenAuth mfiTokenAuth;
} platform;

/**
 * HomeKit accessory server that hosts the accessory.
 */
static HAPAccessoryServerRef accessoryServer;

void HandleUpdatedState(HAPAccessoryServerRef* _Nonnull server, void* _Nullable context);

/**
 * Functions provided by App.c for each accessory application.
 */
extern void AppRelease(void);
extern void AppCreate(HAPAccessoryServerRef* server, HAPPlatformKeyValueStoreRef keyValueStore);
extern void AppInitialize(
        HAPAccessoryServerOptions* hapAccessoryServerOptions,
        HAPPlatform* hapPlatform,
        HAPAccessoryServerCallbacks* hapAccessoryServerCallbacks);
extern void AppDeinitialize();
extern void AppAccessoryServerStart(void);
extern void AccessoryServerHandleUpdatedState(HAPAccessoryServerRef* server, void* _Nullable context);
extern const HAPAccessory* AppGetAccessoryInfo();

/**
 * Initialize global platform objects.
 */
static void InitializePlatform() {
    // Key-value store.
    HAPPlatformKeyValueStoreCreate(
            &platform.keyValueStore, &(const HAPPlatformKeyValueStoreOptions) { .rootDirectory = ".HomeKitStore" });
    platform.hapPlatform.keyValueStore = &platform.keyValueStore;

    // Accessory setup manager. Depends on key-value store.
    static HAPPlatformAccessorySetup accessorySetup;
    HAPPlatformAccessorySetupCreate(
            &accessorySetup, &(const HAPPlatformAccessorySetupOptions) { .keyValueStore = &platform.keyValueStore });
    platform.hapPlatform.accessorySetup = &accessorySetup;

    // TCP stream manager.
    HAPPlatformTCPStreamManagerCreate(
            &platform.tcpStreamManager,
            &(const HAPPlatformTCPStreamManagerOptions) {
                    .interfaceName = NULL,       // Listen on all available network interfaces.
                    .port = kHAPNetworkPort_Any, // Listen on unused port number from the ephemeral port range.
                    .maxConcurrentTCPStreams = kHAPIPSessionStorage_DefaultNumElements });

    // Service discovery.
    static HAPPlatformServiceDiscovery serviceDiscovery;
    HAPPlatformServiceDiscoveryCreate(
            &serviceDiscovery,
            &(const HAPPlatformServiceDiscoveryOptions) {
                    0 /* Register services on all available network interfaces. */
            });
    platform.hapPlatform.ip.serviceDiscovery = &serviceDiscovery;

    // Software Token provider. Depends on key-value store.
    HAPPlatformMFiTokenAuthCreate(
            &platform.mfiTokenAuth,
            &(const HAPPlatformMFiTokenAuthOptions) { .keyValueStore = &platform.keyValueStore });

    // Run loop.
    HAPPlatformRunLoopCreate(&(const HAPPlatformRunLoopOptions) { .keyValueStore = &platform.keyValueStore });

    platform.hapAccessoryServerOptions.maxPairings = kHAPPairingStorage_MinElements;

    platform.hapPlatform.authentication.mfiTokenAuth =
            HAPPlatformMFiTokenAuthIsProvisioned(&platform.mfiTokenAuth) ? &platform.mfiTokenAuth : NULL;

    platform.hapAccessoryServerCallbacks.handleUpdatedState = HandleUpdatedState;
}

/**
 * Deinitialize global platform objects.
 */
static void DeinitializePlatform() {
    // TCP stream manager.
    HAPPlatformTCPStreamManagerRelease(&platform.tcpStreamManager);

    AppDeinitialize();

    // Run loop.
    HAPPlatformRunLoopRelease();
}

/**
 * Restore platform specific factory settings.
 */
void RestorePlatformFactorySettings(void) {
}

/**
 * Either simply passes State handling to app, or processes Factory Reset
 */
void HandleUpdatedState(HAPAccessoryServerRef* _Nonnull server, void* _Nullable context) {
    if (HAPAccessoryServerGetState(server) == kHAPAccessoryServerState_Idle && requestedFactoryReset) {
        HAPPrecondition(server);

        HAPError err;

        HAPLogInfo(&kHAPLog_Default, "A factory reset has been requested.");

        // Purge app state.
        err = HAPPlatformKeyValueStorePurgeDomain(&platform.keyValueStore, ((HAPPlatformKeyValueStoreDomain) 0x00));
        if (err) {
            HAPAssert(err == kHAPError_Unknown);
            HAPFatalError();
        }

        // Reset HomeKit state.
        err = HAPRestoreFactorySettings(&platform.keyValueStore);
        if (err) {
            HAPAssert(err == kHAPError_Unknown);
            HAPFatalError();
        }

        // Restore platform specific factory settings.
        RestorePlatformFactorySettings();

        // De-initialize App.
        AppRelease();

        requestedFactoryReset = false;

        // Re-initialize App.
        AppCreate(server, &platform.keyValueStore);

        // Restart accessory server.
        AppAccessoryServerStart();
        return;
    } else if (HAPAccessoryServerGetState(server) == kHAPAccessoryServerState_Idle && clearPairings) {
        HAPError err;
        err = HAPRemoveAllPairings(&platform.keyValueStore);
        if (err) {
            HAPAssert(err == kHAPError_Unknown);
            HAPFatalError();
        }
        AppAccessoryServerStart();
    } else {
        AccessoryServerHandleUpdatedState(server, context);
    }
}

static void InitializeIP() {
    // Prepare accessory server storage.
    static HAPIPSession ipSessions[kHAPIPSessionStorage_DefaultNumElements];
    static uint8_t ipInboundBuffers[HAPArrayCount(ipSessions)][kHAPIPSession_DefaultInboundBufferSize];
    static uint8_t ipOutboundBuffers[HAPArrayCount(ipSessions)][kHAPIPSession_DefaultOutboundBufferSize];
    static HAPIPEventNotificationRef ipEventNotifications[HAPArrayCount(ipSessions)][kAttributeCount];
    for (size_t i = 0; i < HAPArrayCount(ipSessions); i++) {
        ipSessions[i].inboundBuffer.bytes = ipInboundBuffers[i];
        ipSessions[i].inboundBuffer.numBytes = sizeof ipInboundBuffers[i];
        ipSessions[i].outboundBuffer.bytes = ipOutboundBuffers[i];
        ipSessions[i].outboundBuffer.numBytes = sizeof ipOutboundBuffers[i];
        ipSessions[i].eventNotifications = ipEventNotifications[i];
        ipSessions[i].numEventNotifications = HAPArrayCount(ipEventNotifications[i]);
    }
    static HAPIPReadContextRef ipReadContexts[kAttributeCount];
    static HAPIPWriteContextRef ipWriteContexts[kAttributeCount];
    static uint8_t ipScratchBuffer[kHAPIPSession_DefaultScratchBufferSize];
    static HAPIPAccessoryServerStorage ipAccessoryServerStorage = {
        .sessions = ipSessions,
        .numSessions = HAPArrayCount(ipSessions),
        .readContexts = ipReadContexts,
        .numReadContexts = HAPArrayCount(ipReadContexts),
        .writeContexts = ipWriteContexts,
        .numWriteContexts = HAPArrayCount(ipWriteContexts),
        .scratchBuffer = { .bytes = ipScratchBuffer, .numBytes = sizeof ipScratchBuffer }
    };

    platform.hapAccessoryServerOptions.ip.transport = &kHAPAccessoryServerTransport_IP;
    platform.hapAccessoryServerOptions.ip.accessoryServerStorage = &ipAccessoryServerStorage;

    platform.hapPlatform.ip.tcpStreamManager = &platform.tcpStreamManager;
}

int main(int argc HAP_UNUSED, char* _Nullable argv[_Nullable] HAP_UNUSED) {
    HAPAssert(HAPGetCompatibilityVersion() == HAP_COMPATIBILITY_VERSION);

    // Initialize global platform objects.
    InitializePlatform();

    InitializeIP();

    // Perform Application-specific initalizations such as setting up callbacks
    // and configure any additional unique platform dependencies
    AppInitialize(&platform.hapAccessoryServerOptions, &platform.hapPlatform, &platform.hapAccessoryServerCallbacks);

    // Initialize accessory server.
    HAPAccessoryServerCreate(
            &accessoryServer,
            &platform.hapAccessoryServerOptions,
            &platform.hapPlatform,
            &platform.hapAccessoryServerCallbacks,
            /* context: */ NULL);

    // Create app object.
    AppCreate(&accessoryServer, &platform.keyValueStore);

    // Start accessory server for App.
    AppAccessoryServerStart();

    // Run main loop until explicitly stopped.
    HAPPlatformRunLoopRun();
    // Run loop stopped explicitly by calling function HAPPlatformRunLoopStop.

    // Cleanup.
    AppRelease();

    HAPAccessoryServerRelease(&accessoryServer);

    DeinitializePlatform();

    return 0;
}
