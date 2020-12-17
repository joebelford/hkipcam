# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

project(IpCamera)

set (HOMEKIT_ADK "${CMAKE_CURRENT_LIST_DIR}/../homekit_adk")

set (include_dirs 
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper"
        "${CMAKE_SOURCE_DIR}/src"

        "${HOMEKIT_ADK}/PAL/POSIX"

        "${HOMEKIT_ADK}/PAL"
        "${HOMEKIT_ADK}/HAP"

        "${HOMEKIT_ADK}/External/Base64"
        "${HOMEKIT_ADK}/External/JSON"
        "${HOMEKIT_ADK}/External/HTTP"
        )

set (srcs 
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap.h"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap.hpp"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap_accessory.cpp"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap_accessory.hpp"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap_chars.cpp"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap_chars.hpp"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap_service.cpp"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/hap_service.hpp"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/homekit_adk.c"
#        "${CMAKE_SOURCE_DIR}/src/cpp_wrapper/homekit_adk_db.c"
        "${CMAKE_SOURCE_DIR}/src/App.c"
        "${CMAKE_SOURCE_DIR}/src/App_Svc_CamStreamMan.c"
        "${CMAKE_SOURCE_DIR}/src/App_Svc_CamStreamMan.h"
        "${CMAKE_SOURCE_DIR}/src/DB.c"
        "${CMAKE_SOURCE_DIR}/src/Main.c"

        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatform.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformAbort.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformAccessorySetup.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformAccessorySetupDisplay.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformAccessorySetupNFC.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformBLEPeripheralManager.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformClock.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformFileManager.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformKeyValueStore.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformLog.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformMFiHWAuth.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformMFiTokenAuth.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformRandomNumber.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformRunLoop.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformServiceDiscovery.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformSystemCommand.c"
        "${HOMEKIT_ADK}/PAL/POSIX/HAPPlatformTCPStreamManager.c"

        #"${HOMEKIT_ADK}/PAL/Crypto/MbedTLS/HAPMbedTLS.c"
        "${HOMEKIT_ADK}/PAL/Crypto/OpenSSL/HAPOpenSSL.c"

        "${HOMEKIT_ADK}/PAL/HAPAssert.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+Crypto.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+Double.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+Float.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+Int.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+MACAddress.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+RawBuffer.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+Sha1Checksum.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+String.c"
        "${HOMEKIT_ADK}/PAL/HAPBase+UTF8.c"
        "${HOMEKIT_ADK}/PAL/HAPLog.c"
        "${HOMEKIT_ADK}/PAL/HAPPlatformSystemInit.c"
        "${HOMEKIT_ADK}/HAP/HAPAccessory+Info.c"
        "${HOMEKIT_ADK}/HAP/HAPAccessoryServer+Reset.c"
        "${HOMEKIT_ADK}/HAP/HAPAccessoryServer.c"
        "${HOMEKIT_ADK}/HAP/HAPAccessorySetup.c"
        "${HOMEKIT_ADK}/HAP/HAPAccessorySetupInfo.c"
        "${HOMEKIT_ADK}/HAP/HAPAccessoryValidation.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEAccessoryServer+Advertising.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEAccessoryServer+Broadcast.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEAccessoryServer.c"
        "${HOMEKIT_ADK}/HAP/HAPBLECharacteristic+Broadcast.c"
        "${HOMEKIT_ADK}/HAP/HAPBLECharacteristic+Configuration.c"
        "${HOMEKIT_ADK}/HAP/HAPBLECharacteristic+Signature.c"
        "${HOMEKIT_ADK}/HAP/HAPBLECharacteristic.c"
        "${HOMEKIT_ADK}/HAP/HAPBLECharacteristicParseAndWriteValue.c"
        "${HOMEKIT_ADK}/HAP/HAPBLECharacteristicReadAndSerializeValue.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEPDU+TLV.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEPDU.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEPeripheralManager.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEProcedure.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEProtocol+Configuration.c"
        "${HOMEKIT_ADK}/HAP/HAPBLEService+Signature.c"
        "${HOMEKIT_ADK}/HAP/HAPBLESession.c"
        "${HOMEKIT_ADK}/HAP/HAPBLETransaction.c"
        "${HOMEKIT_ADK}/HAP/HAPBitSet.c"
        "${HOMEKIT_ADK}/HAP/HAPCharacteristic.c"
        "${HOMEKIT_ADK}/HAP/HAPCharacteristicTypes.c"
        "${HOMEKIT_ADK}/HAP/HAPDeviceID.c"
        "${HOMEKIT_ADK}/HAP/HAPIP+ByteBuffer.c"
        "${HOMEKIT_ADK}/HAP/HAPIPAccessory.c"
        "${HOMEKIT_ADK}/HAP/HAPIPAccessoryProtocol.c"
        "${HOMEKIT_ADK}/HAP/HAPIPAccessoryServer.c"
        "${HOMEKIT_ADK}/HAP/HAPIPCharacteristic.c"
        "${HOMEKIT_ADK}/HAP/HAPIPSecurityProtocol.c"
        "${HOMEKIT_ADK}/HAP/HAPIPServiceDiscovery.c"
        "${HOMEKIT_ADK}/HAP/HAPJSONUtils.c"
        "${HOMEKIT_ADK}/HAP/HAPLegacyImport.c"
        "${HOMEKIT_ADK}/HAP/HAPMACAddress.c"
        "${HOMEKIT_ADK}/HAP/HAPMFiHWAuth.c"
        "${HOMEKIT_ADK}/HAP/HAPMFiTokenAuth.c"
        "${HOMEKIT_ADK}/HAP/HAPPDU.c"
        "${HOMEKIT_ADK}/HAP/HAPPairing.c"
        "${HOMEKIT_ADK}/HAP/HAPPairingBLESessionCache.c"
        "${HOMEKIT_ADK}/HAP/HAPPairingPairSetup.c"
        "${HOMEKIT_ADK}/HAP/HAPPairingPairVerify.c"
        "${HOMEKIT_ADK}/HAP/HAPPairingPairings.c"
        "${HOMEKIT_ADK}/HAP/HAPRequestHandlers+AccessoryInformation.c"
        "${HOMEKIT_ADK}/HAP/HAPRequestHandlers+HAPProtocolInformation.c"
        "${HOMEKIT_ADK}/HAP/HAPRequestHandlers+Pairing.c"
        "${HOMEKIT_ADK}/HAP/HAPRequestHandlers.c"
        "${HOMEKIT_ADK}/HAP/HAPServiceTypes.c"
        "${HOMEKIT_ADK}/HAP/HAPSession.c"
        "${HOMEKIT_ADK}/HAP/HAPStringBuilder.c"
        "${HOMEKIT_ADK}/HAP/HAPTLV.c"
        "${HOMEKIT_ADK}/HAP/HAPTLVMemory.c"
        "${HOMEKIT_ADK}/HAP/HAPTLVReader.c"
        "${HOMEKIT_ADK}/HAP/HAPTLVWriter.c"
        "${HOMEKIT_ADK}/HAP/HAPUUID.c"
        "${HOMEKIT_ADK}/HAP/HAPVersion.c"

        "${HOMEKIT_ADK}/External/JSON/util_json_reader.c"
        "${HOMEKIT_ADK}/External/HTTP/util_http_reader.c"
        "${HOMEKIT_ADK}/External/Base64/util_base64.c"
        )

add_definitions(-DHAP_LOG_LEVEL=3
                -DHAP_ENABLE_DEVELOPMENT_ONLY_CODE=1
                -DIP=1
                -O0 -g)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Bonjour)

add_executable(${PROJECT_NAME} ${srcs})

target_include_directories(${PROJECT_NAME} PUBLIC ${include_dirs})

target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto ${BONJOUR_LIBRARY})
