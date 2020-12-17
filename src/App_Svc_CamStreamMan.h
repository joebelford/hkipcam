#ifndef APP_SVC_CAMSTREAMMAN_H
#define APP_SVC_CAMSTREAMMAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAP.h"

/**
 * Handle read request to the 'SupportedVideoStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSupportedVideoStreamConfigurationRead(HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicReadRequest *request,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context);

/**
 * Handle read request to the 'SupportedAudioStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSupportedAudioStreamConfigurationRead(HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicReadRequest *request,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context);

/**
 * Handle read request to the 'SupportedRTPConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSupportedRTPConfigurationRead(HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicReadRequest *request,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context);

/**
 * Handle read request to the 'StreamingStatus' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleStreamingStatusRead(HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicReadRequest *request,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context);

/**
 * Handle read request to the 'SetupEndpoints' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSetupEndpointsRead(HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicReadRequest *request,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context);

/**
 * Handle write request to the 'SetupEndpoints' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSetupEndpointsWrite(
        HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicWriteRequest* request,
        HAPTLVReaderRef* requestReader,
        void* _Nullable context);

/**
 * Handle read request to the 'SelectedRtpStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSelectedRtpStreamConfigurationRead(HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicReadRequest *request,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context);

/**
 * Handle write request to the 'SelectedRtpStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSelectedRtpStreamConfigurationWrite(
        HAPAccessoryServerRef* server,
        const HAPTLV8CharacteristicWriteRequest* request,
        HAPTLVReaderRef* requestReader,
        void* _Nullable context);


#ifdef __cplusplus
}
#endif

#endif

