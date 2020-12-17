#include "App_Svc_CamStreamMan.h"

#include "HAPTLV+Internal.h"

bool isValid(void *unsused HAP_UNUSED)
{
    return true;
}

typedef struct
{
    uint8_t audioChannels;
    uint8_t bitRate;
    uint8_t sampleRate;
} audioCodecParamsStruct;

typedef struct
{
    uint16_t audioCodecType;
    audioCodecParamsStruct audioCodecParams;
} audioCodecConfigStruct;

typedef struct
{
    audioCodecConfigStruct audioCodecConfig;
    uint8_t comfortNoiseSupport;

} supportedAudioConfigStruct;

supportedAudioConfigStruct supportedAudioConfigValue =
{
    .audioCodecConfig =
    {
        .audioCodecType = 2,     // AAC-ELD
        .audioCodecParams =
        {
            .audioChannels = 1,  // 1 channel
            .bitRate = 0,        // Variable
            .sampleRate = 0      // 8kHz
        }
    },
    .comfortNoiseSupport = false
};

HAP_STRUCT_TLV_SUPPORT(void, SupportedAudioConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, AudioCodecConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, AudioCodecParamsFormat)

const HAPUInt8TLVFormat audioCodecParamsAudioChannelsFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 1, .maximumValue = 1},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember audioCodecParamsAudioChannelsMember = {
    .valueOffset = HAP_OFFSETOF(audioCodecParamsStruct, audioChannels),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Audio Codec Config Params Audio Channels",
    .format = &audioCodecParamsAudioChannelsFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat audioCodecParamsBitRateFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 2},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember audioCodecParamsBitRateMember = {
    .valueOffset = HAP_OFFSETOF(audioCodecParamsStruct, bitRate),
    .isSetOffset = 0,
    .tlvType = 2,
    .debugDescription = "Audio Codec Config Params Bit-Rate",
    .format = &audioCodecParamsBitRateFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat audioCodecParamsSampleRateFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 2},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember audioCodecParamsSampleRateMember = {
    .valueOffset = HAP_OFFSETOF(audioCodecParamsStruct, sampleRate),
    .isSetOffset = 0,
    .tlvType = 3,
    .debugDescription = "Audio Codec Config Params Sample Rate",
    .format = &audioCodecParamsSampleRateFormat,
    .isOptional = false,
    .isFlat = false
};

/* ---------------------------------------------------------------------------------------------*/

const HAPUInt16TLVFormat audioCodecTypeFormat = {
    .type = kHAPTLVFormatType_UInt16,
    .constraints = { .minimumValue = 0, .maximumValue = 6},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember audioCodecTypeMember = {
    .valueOffset = HAP_OFFSETOF(audioCodecConfigStruct, audioCodecType),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Audio Codec Type",
    .format = &audioCodecTypeFormat,
    .isOptional = false,
    .isFlat = false
};

/* ---------------------------------------------------------------------------------------------*/

const AudioCodecConfigFormat audioCodecParamsFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &audioCodecParamsAudioChannelsMember,
                                                     &audioCodecParamsBitRateMember,
                                                     &audioCodecParamsSampleRateMember,
                                                     NULL},
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember audioCodecParamsMember = {
    .valueOffset = HAP_OFFSETOF(audioCodecConfigStruct, audioCodecParams),
    .isSetOffset = 0,
    .tlvType = 2,
    .debugDescription = "Audio Codec Parameters",
    .format = &audioCodecParamsFormat,
    .isOptional = false,
    .isFlat = false
};


/* ---------------------------------------------------------------------------------------------*/

const AudioCodecConfigFormat audioCodecConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &audioCodecTypeMember,
                                                     &audioCodecParamsMember,
                                                     NULL},
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember audioCodecConfigMember = {
    .valueOffset = HAP_OFFSETOF(supportedAudioConfigStruct, audioCodecConfig),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Audio Codec Config",
    .format = &audioCodecConfigFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat comfortNoiseFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 255},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember comfortNoiseMember = {
    .valueOffset = HAP_OFFSETOF(supportedAudioConfigStruct, comfortNoiseSupport),
    .isSetOffset = 0,
    .tlvType = 2,
    .debugDescription = "Comfort Noise Support",
    .format = &comfortNoiseFormat,
    .isOptional = false,
    .isFlat = false
};

const SupportedAudioConfigFormat supportedAudioConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &audioCodecConfigMember,
                                                     &comfortNoiseMember,
                                                     NULL},
    .callbacks = { .isValid = isValid }
};

/* ---------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------- */

typedef struct
{
    uint8_t profileID;
    uint8_t level;
    uint8_t packetizationMode;
} videoCodecParamsStruct;

typedef struct
{
    uint16_t imageWidth;
    uint16_t imageHeight;
    uint8_t  frameRate;
} videoAttributesStruct;

typedef struct
{
    uint8_t videoCodecType;
    videoCodecParamsStruct videoCodecParams;
    videoAttributesStruct videoAttributes;

} videoCodecConfigStruct;

typedef struct
{
    videoCodecConfigStruct videoCodecConfig;
} supportedVideoConfigStruct;

supportedVideoConfigStruct supportedVideoConfigValue =
{
    .videoCodecConfig = {
        .videoCodecType = 0,
        .videoCodecParams = {
            .profileID = 0,
            .level = 0,
            .packetizationMode = 0
        },
        .videoAttributes = {
            .imageWidth = 1280,
            .imageHeight = 720,
            .frameRate = 30
        }
    }
};

HAP_STRUCT_TLV_SUPPORT(void, SupportedVideoConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, VideoCodecConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, VideoCodecParamsFormat)
HAP_STRUCT_TLV_SUPPORT(void, VideoAttributesFormat)

const HAPUInt8TLVFormat videoCodecParamsProfileIDFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 2},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember videoCodecParamsProfileIDMember = {
    .valueOffset = HAP_OFFSETOF(videoCodecParamsStruct, profileID),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Video Codec Config Params Profile ID",
    .format = &videoCodecParamsProfileIDFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat videoCodecParamsLevelFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 2},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember videoCodecParamsLevelMember = {
    .valueOffset = HAP_OFFSETOF(videoCodecParamsStruct, level),
    .isSetOffset = 0,
    .tlvType = 2,
    .debugDescription = "Video Codec Config Params Level",
    .format = &videoCodecParamsLevelFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat videoCodecParamsPacketizationModeFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 2},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember videoCodecParamsPacketizationModeMember = {
    .valueOffset = HAP_OFFSETOF(videoCodecParamsStruct, packetizationMode),
    .isSetOffset = 0,
    .tlvType = 3,
    .debugDescription = "Video Codec Config Packetization Mode",
    .format = &videoCodecParamsPacketizationModeFormat,
    .isOptional = false,
    .isFlat = false
};

/* ---------------------------------------------------------------------------------------------*/

const HAPUInt8TLVFormat videoCodecTypeFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 1},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember videoCodecTypeMember = {
    .valueOffset = HAP_OFFSETOF(videoCodecConfigStruct, videoCodecType),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Video Codec Type",
    .format = &videoCodecTypeFormat,
    .isOptional = false,
    .isFlat = false
};

/* ---------------------------------------------------------------------------------------------*/

const VideoCodecConfigFormat videoCodecParamsFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoCodecParamsProfileIDMember,
                                                     &videoCodecParamsLevelMember,
                                                     &videoCodecParamsPacketizationModeMember,
                                                     NULL},
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember videoCodecParamsMember = {
    .valueOffset = HAP_OFFSETOF(videoCodecConfigStruct, videoCodecParams),
    .isSetOffset = 0,
    .tlvType = 2,
    .debugDescription = "Video Codec Parameters",
    .format = &videoCodecParamsFormat,
    .isOptional = false,
    .isFlat = false
};

/* ---------------------------------------------------------------------------------------------*/

const HAPUInt16TLVFormat videoAttributesImageWidthFormat = {
    .type = kHAPTLVFormatType_UInt16,
    .constraints = { .minimumValue = 0, .maximumValue = 4096},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember videoAttributesImageWidthMember = {
    .valueOffset = HAP_OFFSETOF(videoAttributesStruct, imageWidth),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Video Attributes Image Width",
    .format = &videoAttributesImageWidthFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt16TLVFormat videoAttributesImageHeightFormat = {
    .type = kHAPTLVFormatType_UInt16,
    .constraints = { .minimumValue = 0, .maximumValue = 4096},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember videoAttributesImageHeightMember = {
    .valueOffset = HAP_OFFSETOF(videoAttributesStruct, imageHeight),
    .isSetOffset = 0,
    .tlvType = 2,
    .debugDescription = "Video Attributes Image Height",
    .format = &videoAttributesImageHeightFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat videoAttributesFrameRateFormat = {
    .type = kHAPTLVFormatType_UInt8,
    .constraints = { .minimumValue = 0, .maximumValue = 255},
    .callbacks = { .getDescription = NULL }
};
const HAPStructTLVMember videoAttributesFrameRateMember = {
    .valueOffset = HAP_OFFSETOF(videoAttributesStruct, frameRate),
    .isSetOffset = 0,
    .tlvType = 3,
    .debugDescription = "Video Attributes Frame Rate",
    .format = &videoAttributesFrameRateFormat,
    .isOptional = false,
    .isFlat = false
};

const VideoAttributesFormat videoAttributesFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoAttributesImageWidthMember,
                                                     &videoAttributesImageHeightMember,
                                                     &videoAttributesFrameRateMember,
                                                     NULL},
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember videoAttributesMember = {
    .valueOffset = HAP_OFFSETOF(videoCodecConfigStruct, videoAttributes),
    .isSetOffset = 0,
    .tlvType = 3,
    .debugDescription = "Video Attributes",
    .format = &videoAttributesFormat,
    .isOptional = false,
    .isFlat = false
};

/* ---------------------------------------------------------------------------------------------*/

const VideoCodecConfigFormat videoCodecConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoCodecTypeMember,
                                                     &videoCodecParamsMember,
                                                     &videoAttributesMember,
                                                     NULL},
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember videoCodecConfigMember = {
    .valueOffset = HAP_OFFSETOF(supportedVideoConfigStruct, videoCodecConfig),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Video Codec Config",
    .format = &videoCodecConfigFormat,
    .isOptional = false,
    .isFlat = false
};

/* ---------------------------------------------------------------------------------------------*/

const SupportedVideoConfigFormat supportedVideoConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoCodecConfigMember, NULL},
    .callbacks = { .isValid = isValid }
};

/**
 * Handle read request to the 'SupportedVideoStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSupportedVideoStreamConfigurationRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;

    err = HAPTLVWriterEncode(responseWriter, &supportedVideoConfigFormat, &supportedVideoConfigValue);
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        return err;
    }

    return kHAPError_None;
}

/**
 * Handle read request to the 'SupportedAudioStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSupportedAudioStreamConfigurationRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;

    err = HAPTLVWriterEncode(responseWriter, &supportedAudioConfigFormat, &supportedAudioConfigValue);
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        return err;
    }

    return kHAPError_None;
}
/**
 * Handle read request to the 'SupportedRTPConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSupportedRTPConfigurationRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;

    void* bytes;
    size_t maxBytes;
    HAPTLVWriterGetScratchBytes(responseWriter, &bytes, &maxBytes);

    uint8_t supportedSRTPcryptoSuite = 0; // 0 - AES_CM_128_HMAC_SHA1_80

    err = HAPTLVWriterAppend(
            responseWriter,
            &(const HAPTLV) { .type = 2, // SRTP Crypto Suite
                              .value = { .bytes = &supportedSRTPcryptoSuite, .numBytes = 1 } });
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        return err;
    }

    return kHAPError_None;
}

/**
 * Handle read request to the 'StreamingStatus' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleStreamingStatusRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    return kHAPError_None;
}

/**
 * Handle read request to the 'SetupEndpoints' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSetupEndpointsRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    return kHAPError_None;
}

/**
 * Handle write request to the 'SetupEndpoints' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSetupEndpointsWrite(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicWriteRequest* request HAP_UNUSED,
        HAPTLVReaderRef* requestReader,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(requestReader);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;

    // Simply validate input.
    err = HAPTLVReaderGetAll(requestReader, (HAPTLV* const[]) { NULL });
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }
    return kHAPError_None;
}

/**
 * Handle read request to the 'SelectedRtpStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSelectedRtpStreamConfigurationRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    return kHAPError_None;
}

/**
 * Handle write request to the 'SelectedRtpStreamConfiguration' characteristic of the Camera RTP Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSelectedRtpStreamConfigurationWrite(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicWriteRequest* request HAP_UNUSED,
        HAPTLVReaderRef* requestReader,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(requestReader);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;

    // Simply validate input.
    err = HAPTLVReaderGetAll(requestReader, (HAPTLV* const[]) { NULL });
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }
    return kHAPError_None;
}
