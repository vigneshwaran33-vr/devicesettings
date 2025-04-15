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
   
	IARM_Bus_Init("SampleDSClient");
	IARM_Bus_Connect();

    try {
        device::Manager::Initialize();

        if ((argc < 4) || (argc > 5)) {
            printf("%s : <Port Type - HDMI, SPEAKER> <Port Number-0, 1, 2...> <Mode- 0 - Off, 1 - On, 2 - Auto> <Level 0-10 (required for On mode)>\r\n", argv[0]);
	    if((atoi((const char *)argv[3]) == 1) && (argc != 5)) {
	        printf("%s: Level required for Mode = 1 - On \r\n",argv[0]);
	        return 0;
	    }
            return 0;
        }

        char *portType = argv[1];
        char *portId = argv[2];
        dsVolumeLeveller_t leveller;
        leveller.mode = atoi((const char *)argv[3]);
        leveller.level = 0;
        if(leveller.mode == 1) {
            leveller.level = atoi((const char *)argv[4]);
        }
		printf("Sample Application: set Volume leveller\r\n");
		device::AudioOutputPort aPort = device::Host::getInstance().getAudioOutputPort(std::string(portType).append(portId));
		aPort.setVolumeLeveller(leveller);
		printf("Sample Application: set volume leveller\r\n");

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
