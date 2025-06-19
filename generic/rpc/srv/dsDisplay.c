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
 


/**
* @defgroup devicesettings
* @{
* @defgroup rpc
* @{
**/


#include "dsDisplay.h"

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "dsError.h"
#include "dsUtl.h"
#include "dsTypes.h"
#include "pthread.h"
#include <pthread.h>
#include "libIARM.h"
#include "libIBus.h"
#include "iarmUtil.h"
#include "dsRpc.h"
#include "dsMgr.h"
#include "dsserverlogger.h"
#include "dsVideoPort.h"
#include "dsVideoPortSettings.h"
#include "dsVideoResolutionSettings.h"

#include "safec_lib.h"

static int m_isInitialized = 0;
static int m_isPlatInitialized = 0;
static pthread_mutex_t dsLock = PTHREAD_MUTEX_INITIALIZER;

#define NULL_HANDLE 0
#define IARM_BUS_Lock(lock) pthread_mutex_lock(&dsLock)
#define IARM_BUS_Unlock(lock) pthread_mutex_unlock(&dsLock)

IARM_Result_t _dsDisplayInit(void *arg);
IARM_Result_t _dsGetDisplay(void *arg);
IARM_Result_t _dsGetDisplayAspectRatio(void *arg);
IARM_Result_t _dsGetEDID(void *arg);
IARM_Result_t _dsGetEDIDBytes(void *arg);
IARM_Result_t _dsDisplayTerm(void *arg);
void _dsDisplayEventCallback(intptr_t handle, dsDisplayEvent_t event, void *eventData);
static void  filterEDIDResolution(intptr_t Shandle, dsDisplayEDID_t *edid);
static void  dumpEDIDInformation( dsDisplayEDID_t *edid);
static dsVideoPortType_t _GetDisplayPortType(intptr_t handle);
extern void resetColorDepthOnHdmiReset(intptr_t handle);
static intptr_t _hdmiVideoPortHandle = 0;

IARM_Result_t dsDisplayMgr_init()
{
	IARM_BUS_Lock(lock);
	if (!m_isPlatInitialized) {
    	/* Nexus init, if any here */
    	dsDisplayInit();
    }
	/*coverity[missing_lock]  CID-19379 using Coverity Annotation to ignore error*/
	m_isPlatInitialized++;

	IARM_BUS_Unlock(lock);  //CID:136387 - Data race condition
	IARM_Bus_RegisterCall(IARM_BUS_DSMGR_API_dsDisplayInit, _dsDisplayInit);
    return IARM_RESULT_SUCCESS;
}

IARM_Result_t dsDisplayMgr_term()
{
    return IARM_RESULT_SUCCESS;
}

IARM_Result_t _dsDisplayInit(void *arg)
{
    IARM_BUS_Lock(lock);

    if (!m_isInitialized) {

		/* Register appropriate dsRegisterDisplayEventCallback here*/	
        dsRegisterDisplayEventCallback(NULL,_dsDisplayEventCallback);
		
		IARM_Bus_RegisterCall(IARM_BUS_DSMGR_API_dsGetDisplay,_dsGetDisplay);
		IARM_Bus_RegisterCall(IARM_BUS_DSMGR_API_dsGetDisplayAspectRatio,_dsGetDisplayAspectRatio);
		IARM_Bus_RegisterCall(IARM_BUS_DSMGR_API_dsGetEDID,_dsGetEDID);
		IARM_Bus_RegisterCall(IARM_BUS_DSMGR_API_dsGetEDIDBytes,_dsGetEDIDBytes);
		IARM_Bus_RegisterCall(IARM_BUS_DSMGR_API_dsDisplayTerm,_dsDisplayTerm);

		m_isInitialized = 1;
    }

    if (!m_isPlatInitialized) {
    	/* Nexus init, if any here */
    	dsDisplayInit();
    }
	m_isPlatInitialized++;
    IARM_BUS_Unlock(lock);
	return IARM_RESULT_SUCCESS;

}

IARM_Result_t _dsGetDisplay(void *arg)
{
    _DEBUG_ENTER();

    IARM_BUS_Lock(lock);
    
	dsDisplayGetHandleParam_t *param = (dsDisplayGetHandleParam_t *)arg;
    dsGetDisplay(param->type, param->index, &param->handle);
    
	IARM_BUS_Unlock(lock);
	
	return IARM_RESULT_SUCCESS;
}

IARM_Result_t _dsGetDisplayAspectRatio(void *arg)
{
    _DEBUG_ENTER();

    IARM_BUS_Lock(lock);
    
	dsDisplayGetAspectRatioParam_t *param = (dsDisplayGetAspectRatioParam_t *)arg;
    dsGetDisplayAspectRatio(param->handle, &param->aspectRatio);
    
	
	IARM_BUS_Unlock(lock);
	
	return IARM_RESULT_SUCCESS;
}

IARM_Result_t _dsGetEDID(void *arg)
{
    _DEBUG_ENTER();

    IARM_BUS_Lock(lock);
	dsDisplayGetEDIDParam_t *param = (dsDisplayGetEDIDParam_t *)arg;
    dsVideoPortType_t _VPortType;

    dsGetEDID(param->handle, &param->edid);
    
    filterEDIDResolution(param->handle, &param->edid);
    dumpEDIDInformation( &param->edid);
	
	IARM_BUS_Unlock(lock);
	
	return IARM_RESULT_SUCCESS;
}

IARM_Result_t _dsGetEDIDBytes(void *arg)
{

#ifndef RDK_DSHAL_NAME
#warning   "RDK_DSHAL_NAME is not defined"
#define RDK_DSHAL_NAME "RDK_DSHAL_NAME is not defined"
#endif
    errno_t rc = -1;
    _DEBUG_ENTER();

    IARM_BUS_Lock(lock);

    INT_DEBUG("dsSRV::getEDIDBytes \r\n");

    typedef dsError_t (*dsGetEDIDBytes_t)(intptr_t handle, unsigned char *edid, int *length);
    static dsGetEDIDBytes_t func = 0;
    if (func == 0) {
        void *dllib = dlopen(RDK_DSHAL_NAME, RTLD_LAZY);
        if (dllib) {
            func = (dsGetEDIDBytes_t) dlsym(dllib, "dsGetEDIDBytes");
            if (func) {
                INT_DEBUG("dsGetEDIDBytes(void) is defined and loaded\r\n");
            }   
            else {
                INT_INFO("dsGetEDIDBytes(void) is not defined\r\n");
            }   
            dlclose(dllib);
        }   
        else {
            INT_ERROR("Opening RDK_DSHAL_NAME [%s] failed\r\n", RDK_DSHAL_NAME);
        }   
    }   

    dsDisplayGetEDIDBytesParam_t *param = (dsDisplayGetEDIDBytesParam_t *)arg;

    if (func != 0) {
        unsigned char edid[1024] = {0}; // Increased buffer size to 1024 to avoid out of bounds access
        int length = 0;
        dsError_t ret = func(param->handle, edid, &length);
        if (ret == dsERR_NONE && length <= 1024) {
            rc = memcpy_s(param->bytes,sizeof(param->bytes),edid,length);
            if(rc!=EOK)
            {
                    ERR_CHK(rc);
            }
     	    param->length = length;
        }
        param->result = ret;
    }
    else {
        param->result = dsERR_OPERATION_NOT_SUPPORTED;
    }

    IARM_BUS_Unlock(lock);
	
	return IARM_RESULT_SUCCESS;
}

IARM_Result_t _dsDisplayTerm(void *arg)
{
    _DEBUG_ENTER();
    
	IARM_BUS_Lock(lock);

    m_isPlatInitialized--;
	
	if (0 == m_isPlatInitialized)
	{
		dsDisplayTerm();
	}

    IARM_BUS_Unlock(lock);
	return IARM_RESULT_SUCCESS;
}


/*HDMI Call back */
void _dsDisplayEventCallback(intptr_t handle, dsDisplayEvent_t event, void *eventData)
{
	IARM_Bus_DSMgr_EventData_t _eventData;
    IARM_Bus_DSMgr_EventId_t _eventId;


	if (NULL_HANDLE == handle)
	{
		INT_ERROR("Err:HDMI Hot plug back has NULL Handle... !!!!!!..\r\n");
	}
	switch(event)
	{
		case dsDISPLAY_EVENT_CONNECTED:
			INT_INFO("connecting HDMI to display !!!!!!..\r\n");
			_eventData.data.hdmi_hpd.event =  dsDISPLAY_EVENT_CONNECTED;
            _eventId = IARM_BUS_DSMGR_EVENT_HDMI_HOTPLUG;
	        break;

		case dsDISPLAY_EVENT_DISCONNECTED:
			INT_INFO("Disconnecting HDMI from display !!!!!!!! ..\r\n");
			_eventData.data.hdmi_hpd.event =  dsDISPLAY_EVENT_DISCONNECTED ;
            _eventId = IARM_BUS_DSMGR_EVENT_HDMI_HOTPLUG;
            break;

        case dsDISPLAY_RXSENSE_ON:
            INT_INFO("Rx Sense Status ON !!!!!!!! ..\r\n");
            _eventData.data.hdmi_rxsense.status =  dsDISPLAY_RXSENSE_ON ;
            _eventId = IARM_BUS_DSMGR_EVENT_RX_SENSE;
            break;

        case dsDISPLAY_RXSENSE_OFF:
            INT_INFO("Rx Sense Status OFF !!!!!!!! ..\r\n");
            _eventData.data.hdmi_rxsense.status =  dsDISPLAY_RXSENSE_OFF ;
            _eventId = IARM_BUS_DSMGR_EVENT_RX_SENSE;
            break;    
                
        case dsDISPLAY_HDCPPROTOCOL_CHANGE:
             INT_INFO("HDCP Protocol Version Change !!!!!!!! ..\r\n");
             _eventId = IARM_BUS_DSMGR_EVENT_HDCP_STATUS;
             break;

        default:
			INT_ERROR("Error: Unsupported event in _dsHdmiCallback...\r\n");
			return;
			
	}
    IARM_Bus_BroadcastEvent(IARM_BUS_DSMGR_NAME,(IARM_EventId_t)_eventId,(void *)&_eventData, sizeof(_eventData));
    if (dsDISPLAY_EVENT_CONNECTED == event) {
        dsError_t eRet = dsERR_NONE;
        if (!_hdmiVideoPortHandle){
            eRet = dsGetVideoPort(dsVIDEOPORT_TYPE_HDMI,0,&_hdmiVideoPortHandle);
            if (dsERR_NONE != eRet) {
                _hdmiVideoPortHandle = 0;
            }
        }
        if (_hdmiVideoPortHandle){
            resetColorDepthOnHdmiReset(_hdmiVideoPortHandle);
        } else {
            INT_INFO("HDMI get port handle failed %d \r\n", eRet);
	}
    }
}

static void filterEDIDResolution(intptr_t handle, dsDisplayEDID_t *edid)
{
    errno_t rc = -1;
    dsVideoPortResolution_t *edidResn = NULL;
    dsVideoPortResolution_t *presolution = NULL;
    dsDisplayEDID_t *edidData = (dsDisplayEDID_t*)malloc(sizeof(dsDisplayEDID_t));
    dsVideoPortType_t _VPortType = _GetDisplayPortType(handle);
    if (edid == NULL) {
    	return; // Handle malloc failure
    }
    int numOfSupportedResolution = 0;

    if(_VPortType == dsVIDEOPORT_TYPE_HDMI)
    { 
        INT_DEBUG("EDID for HDMI Port\r\n");    
        size_t iCount = dsUTL_DIM(kResolutions);
        
        /*Initialize the struct*/
        memset(edidData,0,sizeof(*edidData));
        /*Copy the content */
        rc = memcpy_s(edidData,sizeof(*edidData),edid,sizeof(*edidData));
        if(rc!=EOK)
        {
                ERR_CHK(rc);
        }
        INT_INFO("[DsMgr] Total Resolution Count from HAL: %d\r\n",edid->numOfSupportedResolution);
        edid->numOfSupportedResolution = 0;
        for (size_t i = 0; i < iCount; i++)
        {
            presolution = &kResolutions[i];
            for (size_t j = 0; j < edidData->numOfSupportedResolution; j++)
            {    
                edidResn = &(edidData->suppResolutionList[j]);
                
                if (0 == (strcmp(presolution->name,edidResn->name)))
                {
                    edid->suppResolutionList[edid->numOfSupportedResolution] = kResolutions[i];
                    edid->numOfSupportedResolution++;
                    numOfSupportedResolution++;
                    INT_DEBUG("[DsMgr] presolution->name : %s, resolution count : %d\r\n",presolution->name,numOfSupportedResolution);
                }
            }    
        }    
    }
    else
    {
        INT_INFO("EDID for Non HDMI Port\r\n");            
    }

    free(edidData);
}

static void dumpEDIDInformation( dsDisplayEDID_t *edid)
{
    INT_INFO("[DsMgr] Product Code: %x\r\n",edid->productCode);
    INT_INFO("[DsMgr] Serial Number: %x\r\n",edid->serialNumber);
    INT_INFO("[DsMgr] Manufacturer Year: %d\r\n",edid->manufactureYear);
    INT_INFO("[DsMgr] Manufacturer week: %d\r\n",edid->manufactureWeek);
    INT_INFO("[DsMgr] Monitor Name : %s\r\n",edid->monitorName);
    INT_INFO("[DsMgr] Hdmi Device Type : %s\r\n",(edid->hdmiDeviceType)?"HDMI":"DVI");
    INT_INFO("[DsMgr] Hdmi Device Is Repeater : %x\r\n",edid->isRepeater);
    INT_INFO("[DsMgr] No of Resolutions: %x\r\n",edid->numOfSupportedResolution);
    for (size_t j = 0; j < edid->numOfSupportedResolution; j++)
    {
        dsVideoPortResolution_t *edidResn = &(edid->suppResolutionList[j]);
        INT_INFO("[DsMgr] Resolution Name: %s\r\n",edidResn->name);
    }
}

static dsVideoPortType_t _GetDisplayPortType(intptr_t handle)
{
    int numPorts,i;
    intptr_t halhandle = 0;
    
    numPorts = dsUTL_DIM(kSupportedPortTypes);
    for(i=0; i< numPorts; i++)
    {
        dsGetDisplay(kPorts[i].id.type, kPorts[i].id.index, &halhandle);
        if (handle == halhandle)
        {
            return kPorts[i].id.type;
        }
    }
    INT_INFO("Error: The Requested Display is not part of Platform Port Configuration \r\n");
    return dsVIDEOPORT_TYPE_MAX;
}

/** @} */
/** @} */
