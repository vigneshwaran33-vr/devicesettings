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
// TODO: Include your class to test here.


/**
* @defgroup devicesettings
* @{
* @defgroup sample
* @{
**/


#include "videoOutputPort.hpp"
#include "videoOutputPortType.hpp"
#include "host.hpp"
#include "audioOutputPort.hpp"
#include "dsUtl.h"
#include "dsError.h"
#include "stdlib.h"
#include "manager.hpp"



#include "libIBus.h"


#include <string>



int main(int argc, char *argv[]) 
{
   
	IARM_Bus_Init("SetAudioEnable");
	IARM_Bus_Connect();
    
    try {
        device::Manager::Initialize();

        if (argc != 4) {
            printf("%s : <Port Type - HDMI, SPDIF > <Port Number-0, 1, 2...> <Enable-1 or Disable-0>\r\n", argv[0]);
            return 0;
        }

        char *portType = argv[1];
        char *portId = argv[2];
        bool toEnable = (bool) atoi((const char *)argv[3]);

		
		device::VideoOutputPort vPort = device::Host::getInstance().getVideoOutputPort(std::string(portType).append(portId));

        if(toEnable)
        {
            vPort.getAudioOutputPort().enable();
            printf("Enable the Audio Port..\r\n");
        }
        else
        {
            vPort.getAudioOutputPort().disable();   
            printf("Disable the Audio Port..\r\n");
        }

         printf("\t Audio Port Status - [%s]\r\n",vPort.getAudioOutputPort().isEnabled()? "On" : "Off");

		printf("Sample Application: set Audio Mute is completed\r\n");
        device::Manager::DeInitialize();
    }
    catch (...) {
    	printf("Exception Caught during [%s]\r\n", argv[0]);
    }

	IARM_Bus_Disconnect();
	IARM_Bus_Term();

    return 0;
}


/** @} */
/** @} */
