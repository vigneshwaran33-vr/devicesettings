/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright ARRIS Enterprises, Inc. 2015.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under
*/


/**
 * @file hdmiIn.cpp
 * @brief Configuration of HDMI Input
 */
 


/**
* @defgroup devicesettings
* @{
* @defgroup ds
* @{
**/


#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include "hdmiIn.hpp"
#include "illegalArgumentException.hpp"
#include "host.hpp"

#include "dslogger.h"
#include "dsError.h"
#include "dsTypes.h"
#include "dsHdmiIn.h"
#include "dsUtl.h"
#include "edid-parser.hpp"


namespace device 
{


/**
 * @fn  HdmiInput::HdmiInput()
 * @brief default constructor
 *
 * @param None
 *
 * @return None
 * @callergraph
 */
HdmiInput::HdmiInput() 
{
    dsHdmiInInit();
}

/**
 * @fn  HdmiInput::~HdmiInput()
 * @brief destructor
 *
 * @param None
 *
 * @return None
 * @callergraph
 */
HdmiInput::~HdmiInput() 
{
    dsHdmiInTerm();
}

/**
 * @fn  HdmiInput::getInstance()
 * @brief This API is used to get the instance of the HDMI Input
 *
 * @param None
 *
 * @return Reference to the instance of HDMI Input class instance
 * @callergraph
 */
HdmiInput & HdmiInput::getInstance()
{
    static HdmiInput _singleton;
    return _singleton;
}

/**
 * @fn  HdmiInput::getNumberOfInputs()
 * @brief This API is used to get the number of HDMI Input ports on the set-top
 *
 * @param[in] None
 *
 * @return number of HDMI Inputs
 * @callergraph
 */
uint8_t HdmiInput::getNumberOfInputs() const
{
    
    uint8_t numHdmiInputs;
    dsError_t eError = dsHdmiInGetNumberOfInputs (&numHdmiInputs);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError) 
	{
		throw Exception(eError);
	}
    
    return (numHdmiInputs);
}

/**
 * @fn  HdmiInput::isPresented()
 * @brief This API is used to specify if HDMI Input is being
 *        presented via HDMI Out
 *
 * @param[in] None
 *
 * @return true if HDMI Input is being presetned.
 * @callergraph
 */
bool HdmiInput::isPresented() const
{
    dsHdmiInStatus_t Status;
    dsError_t eError = dsHdmiInGetStatus (&Status);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError) 
	{
		throw Exception(eError);
	}
	
	return (Status.isPresented);
}

/**
 * @fn  HdmiInput::isActivePort()
 * @brief This API is used to specify if the provided HDMI Input port is
 *        active (i.e. communicating with the set-top)
 *
 * @param[in] HDMI Input port
 *
 * @return true if the provided HDMI Input port is active.
 * @callergraph
 */
bool HdmiInput::isActivePort(int8_t Port) const
{
    dsHdmiInStatus_t Status;
    dsError_t eError = dsHdmiInGetStatus (&Status);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError) 
	{
		throw Exception(eError);
	}
	
	return (Status.activePort == Port);
}

/**
 * @fn  HdmiInput::getActivePort()
 * @brief This API is used to specify the active (i.e. communicating with
 *        the set-top) HDMI Input port
 *
 * @param[in] None
 *
 * @return the HDMI Input port which is currently active.
 * @callergraph
 */
int8_t HdmiInput::getActivePort() const
{
    dsHdmiInStatus_t Status;
    dsError_t eError = dsHdmiInGetStatus (&Status);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError) 
	{
		throw Exception(eError);
	}
	
	return (Status.activePort);
}

/**
 * @fn  HdmiInput::isPortConnected()
 * @brief This API is used to specify if the prvided HDMI Input port is
 *        connected (i.e. HDMI Input devie is plugged into the set-top).
 *
 * @param[in] HDMI Input port
 *
 * @return true if the HDMI Input port is connected
 * @callergraph
 */
bool HdmiInput::isPortConnected(int8_t Port) const
{
    dsHdmiInStatus_t Status;
    dsError_t eError =  dsHdmiInGetStatus (&Status);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError) 
	{
		throw Exception(eError);
	}
	
	return (Status.isPortConnected[Port]);
}

/**
 * @fn  HdmiInput::selectPort()
 * @brief This API is used to select the HDMI In port to be presented
 *
 * @param[in] int8_t Port : -1 for No HDMI Input port to be presented
 *                           0..n for HDMI Input port (n) to be presented 
 *
 * @return None
 * @callergraph
 */
void HdmiInput::selectPort (int8_t Port,bool requestAudioMix, int videoPlaneType,bool topMost) const
{
	dsError_t eError = dsHdmiInSelectPort ((dsHdmiInPort_t)Port, requestAudioMix ,(dsVideoPlaneType_t)videoPlaneType,topMost);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError) 
	{
		throw Exception(eError);
	}
	
}


/**
 * @fn  HdmiInput::scaleVideo()
 * @brief This API is used to scale the HDMI In video
 *
 * @param[in] int32_t x      : x coordinate for the video
 * @param[in] int32_t y      : y coordinate for the video
 * @param[in] int32_t width  : width of the video
 * @param[in] int32_t height : height of the video
 *
 * @return None
 * @callergraph
 */
void HdmiInput::scaleVideo (int32_t x, int32_t y, int32_t width, int32_t height) const
{
    dsError_t eError = dsHdmiInScaleVideo (x, y, width, height);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError) 
	{
		throw Exception(eError);
	}
}

/**
 * @fn  HdmiInput::selectZoomMode()
 * @brief This API is used to select the HDMI In video zoom mode
 *
 * @param[in] int8_t zoomMoode : 0 for NONE
 *                               1 for FULL
 *
 * @return None
 * @callergraph
 */
void HdmiInput::selectZoomMode (int8_t zoomMode) const
{
    dsError_t eError = dsHdmiInSelectZoomMode ((dsVideoZoom_t)zoomMode);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError)
	{
		throw Exception(eError);
	}
}

static std::string getResolutionStr (dsVideoResolution_t resolution)
{
    std::string resolutionStr;

    switch (resolution)
    {
        case dsVIDEO_PIXELRES_720x480:
            resolutionStr = "480";
            break;

        case dsVIDEO_PIXELRES_720x576:
            resolutionStr = "576";
            break;

        case dsVIDEO_PIXELRES_1280x720:
            resolutionStr = "720";
            break;

        case dsVIDEO_PIXELRES_1366x768:
            resolutionStr = "1366x768";
            break;

        case dsVIDEO_PIXELRES_1920x1080:
            resolutionStr = "1080";
            break;

        case dsVIDEO_PIXELRES_3840x2160:
            resolutionStr = "3840x2160";
            break;

        case dsVIDEO_PIXELRES_4096x2160:
            resolutionStr = "4096x2160";
            break;

        default:
            resolutionStr = "unknown";
            break;
    }

    printf ("%s:%d - ResolutionStr:  %s\n", __PRETTY_FUNCTION__,__LINE__, resolutionStr.c_str());
    return resolutionStr;
}

static std::string getFrameRateStr (dsVideoFrameRate_t frameRate)
{
    std::string FrameRateStr;

    switch (frameRate)
    {
        case dsVIDEO_FRAMERATE_24:
            FrameRateStr = "24";
            break;

        case dsVIDEO_FRAMERATE_25:
            FrameRateStr = "25";
            break;

        case dsVIDEO_FRAMERATE_30:
            FrameRateStr = "30";
            break;

        case dsVIDEO_FRAMERATE_60:
            FrameRateStr = "60";
            break;

        case dsVIDEO_FRAMERATE_23dot98:
            FrameRateStr = "23.98";
            break;

        case dsVIDEO_FRAMERATE_29dot97:
            FrameRateStr = "29.97";
            break;

        case dsVIDEO_FRAMERATE_50:
            FrameRateStr = "50";
            break;

        case dsVIDEO_FRAMERATE_59dot94:
            FrameRateStr = "59.94";
            break;

         default:
            // Not all video formats have a specified framerate.
            break;
    }

    printf ("%s:%d - FrameRateStr: %s\n", __PRETTY_FUNCTION__,__LINE__, FrameRateStr.c_str());
    return FrameRateStr;
}

static std::string getInterlacedStr (bool interlaced)
{
    std::string InterlacedStr = (interlaced) ? "i" : "p";
    printf ("%s:%d - InterlacedStr:  %s\n", __PRETTY_FUNCTION__,__LINE__, InterlacedStr.c_str());
    return InterlacedStr;
}

static std::string CreateResolutionStr (const dsVideoPortResolution_t &resolution)
{
    printf("%s ---> \n", __PRETTY_FUNCTION__);

    std::string resolutionStr = getResolutionStr(resolution.pixelResolution);
    if(resolutionStr.compare("unknown") != 0){
    	resolutionStr = getResolutionStr(resolution.pixelResolution) +
                                getInterlacedStr(resolution.interlaced) +
                                getFrameRateStr(resolution.frameRate);
    }
    printf ("%s <--- %s\n", __PRETTY_FUNCTION__, resolutionStr.c_str());
    return resolutionStr;
}

/**
 * @fn  HdmiInput::getCurrentVideoMode()
 * @brief This API is used to get the current HDMI In video mode (resolution)
 *
 * @param[in] None
 *
 * @return HDMI Input video resolution string
 * @callergraph
 */
std::string HdmiInput::getCurrentVideoMode () const
{

    dsVideoPortResolution_t resolution;
    memset(&resolution, 0, sizeof(resolution));

    dsError_t eError = dsHdmiInGetCurrentVideoMode (&resolution);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError)
	{
		throw Exception(eError);
	}

    std::string resolutionStr = CreateResolutionStr (resolution);
    printf("%s:%d - Resolution =%s\n", __PRETTY_FUNCTION__,__LINE__, resolutionStr.c_str());
    return resolutionStr;
}

void HdmiInput::getCurrentVideoModeObj (dsVideoPortResolution_t& resolution)
{

    memset(&resolution, 0, sizeof(resolution));

    dsError_t eError = dsHdmiInGetCurrentVideoMode (&resolution);

	// Throw an exception if there was an error
	if (dsERR_NONE != eError)
	{
		throw Exception(eError);
	}

    printf("%s:%d - pixelResolution =%d interlaced:%d frameRate:%d\n", __PRETTY_FUNCTION__, __LINE__, resolution.pixelResolution, resolution.interlaced, resolution.frameRate);
}

void HdmiInput::getEDIDBytesInfo (int iHdmiPort, std::vector<uint8_t> &edidArg) const
{

    printf("HdmiInput::getEDIDBytesInfo \r\n");

    dsError_t ret = dsERR_NONE;
    int length = 0;
    unsigned char edid[512] = {0};

    const char* exceptionstr = "";
    ret = dsGetEDIDBytesInfo (static_cast<dsHdmiInPort_t>(iHdmiPort), edid, &length);
    if (NULL == edid) {
        printf("HdmiInput::getEDIDBytesInfo dsGetEDIDBytesInfo returned NULL \r\n");
        exceptionstr = "EDID is NULL";
        ret = dsERR_GENERAL;
    }

    printf("HdmiInput::getEDIDBytesInfo has ret %d\r\n", ret);
    if (ret == dsERR_NONE) {
        if (length <= MAX_EDID_BYTES_LEN) {
            printf("HdmiInput::getEDIDBytesInfo has %d bytes\r\n", length);
            if (edid_parser::EDID_STATUS_OK == edid_parser::EDID_Verify(edid, length)) {
                edidArg.clear();
                edidArg.insert(edidArg.begin(), edid, edid + length);
            } else {
                ret = dsERR_GENERAL;
                exceptionstr = "EDID verification failed";
            }
        } else {
            ret = dsERR_OPERATION_NOT_SUPPORTED;
            exceptionstr = "EDID length > MAX_EDID_BYTES_LEN";
        }
    } else {
        exceptionstr = "dsGetEDIDBytesInfo failed";
    }

    if (ret != dsERR_NONE) {
        throw Exception(ret, exceptionstr);
    }
}

void HdmiInput::getHDMISPDInfo (int iHdmiPort, std::vector<uint8_t> &data) {
    printf("HdmiInput::getHDMISPDInfo \r\n");

    unsigned char spdinfo[sizeof(struct dsSpd_infoframe_st)] = {0};
    const char* exceptionstr = "";
    dsError_t ret = dsGetHDMISPDInfo (static_cast<dsHdmiInPort_t>(iHdmiPort), spdinfo);
    if (NULL == spdinfo) {
        printf("HdmiInput::dsGetHDMISPDInfo returned NULL \r\n");
        exceptionstr = "SPDInfo is NULL";
        ret = dsERR_GENERAL;
    }
 
    printf("HdmiInput::getHDMISPDInfo has ret %d\r\n", ret);
    data.clear();
    if (ret == dsERR_NONE) {
        if (sizeof(spdinfo) <= sizeof(struct dsSpd_infoframe_st)) {
            printf("HdmiInput::getHDMISPDInfo has %d bytes\r\n", sizeof(spdinfo));
                data.insert(data.begin(), spdinfo, spdinfo + sizeof(struct dsSpd_infoframe_st));
        } else {
            ret = dsERR_OPERATION_NOT_SUPPORTED;
            exceptionstr = "size is greater";
        }
    } else {
        exceptionstr = "getHDMISPDInfo failed";
    }
    printf("HdmiInput::getHDMISPDInfo data: \r\n");
        for (int itr = 0; itr < data.size(); itr++) {
            printf("%02X ", data[itr]);
        }
    printf("\n");

    if (ret != dsERR_NONE) {
        throw Exception(ret, exceptionstr);
    }
	
}

void HdmiInput::setEdidVersion (int iHdmiPort, int iEdidVersion) {
    printf ("HdmiInput::setEdidVersion \r\n");
    dsError_t ret = dsSetEdidVersion (static_cast<dsHdmiInPort_t>(iHdmiPort), static_cast<tv_hdmi_edid_version_t>(iEdidVersion));
    if (ret != dsERR_NONE)
    {
        throw Exception(ret);
    }
    printf ("%s:%d - Set EDID Version = %d\n", __PRETTY_FUNCTION__, __LINE__, iEdidVersion);
}

void HdmiInput::getEdidVersion (int iHdmiPort, int *iEdidVersion) {
    printf ("HdmiInput::getEdidVersion \r\n");
    tv_hdmi_edid_version_t EdidVersion;
    dsError_t ret = dsGetEdidVersion (static_cast<dsHdmiInPort_t>(iHdmiPort), &EdidVersion);
    if (ret != dsERR_NONE)
    {
        throw Exception(ret);
    }
    int tmp = static_cast<int>(EdidVersion);
    *iEdidVersion = tmp;
    printf ("%s:%d - EDID Version = %d\n", __PRETTY_FUNCTION__, __LINE__, *iEdidVersion);
}

void HdmiInput::getHdmiALLMStatus (int iHdmiPort, bool *allmStatus) {
    printf ("HdmiInput::getHdmiALLMStatus \r\n");
    dsError_t ret = dsGetAllmStatus (static_cast<dsHdmiInPort_t>(iHdmiPort), allmStatus);
    if (ret != dsERR_NONE)
    {
        throw Exception(ret);
    }
    printf ("%s:%d - ALLM Status = %d\n", __FUNCTION__, __LINE__, *allmStatus);
}

void HdmiInput::getSupportedGameFeatures (std::vector<std::string> &featureList) {
    dsError_t ret = dsERR_NONE;

    dsSupportedGameFeatureList_t feList;
    ret = dsGetSupportedGameFeaturesList(&feList);
    if ( ret != dsERR_NONE)
    {
        throw Exception(ret);
    }

    char* token;

    token = strtok(feList.gameFeatureList, ",");
    while(token != NULL) {
        featureList.emplace_back(token);
        token = strtok(NULL, ",");
    }

    if(featureList.size() != feList.gameFeatureCount){
        printf ("%s:%d - Number of Supported Game Features in list doesn't match with count from HAL", __FUNCTION__, __LINE__);
        throw Exception(dsERR_GENERAL);
    }
}


void HdmiInput::getAVLatency (int *audio_output_delay,int *video_latency) {
    dsError_t ret = dsGetAVLatency (audio_output_delay,video_latency);
    printf ("HdmiInput::getHdmiDAL_ - VideoLatency: %d , Audio Latency:  %d \r\n",*video_latency,*audio_output_delay);
    if (ret != dsERR_NONE)
    {
        throw Exception(ret);
    }
}

void HdmiInput::setEdid2AllmSupport(int iHdmiPort,bool allmSupport)
{
    printf ("HdmiInput::setEdid2AllmSupport \r\n");
    dsError_t ret = dsSetEdid2AllmSupport (static_cast<dsHdmiInPort_t>(iHdmiPort), allmSupport);
    if (ret != dsERR_NONE)
    {
        throw Exception(ret);
    }
    printf ("%s:%d - Set EDID Allm Support = %d\n", __PRETTY_FUNCTION__, __LINE__, allmSupport);

}

void HdmiInput::getEdid2AllmSupport (int iHdmiPort, bool *allmSupport) {
    printf ("HdmiInput::getEdid2AllmSupport \r\n");
    dsError_t ret = dsGetEdid2AllmSupport (static_cast<dsHdmiInPort_t>(iHdmiPort), allmSupport);
    if (ret != dsERR_NONE)
    {
        throw Exception(ret);
    }
    printf ("%s:%d - EDID allm Support = %d\n", __PRETTY_FUNCTION__, __LINE__, *allmSupport);
}

void HdmiInput::getHdmiVersion (int iHdmiPort, dsHdmiMaxCapabilityVersion_t *capversion) {

    dsError_t ret = dsGetHdmiVersion (static_cast<dsHdmiInPort_t>(iHdmiPort), capversion);

    if (ret != dsERR_NONE)
    {
        throw Exception(ret);
    }

    printf ("%s:%d - HDMI Compatibility Version = %d\n", __PRETTY_FUNCTION__, __LINE__, *capversion);
}

}


/** @} */
/** @} */
