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


#include <iostream>
#include <stdlib.h>
#include "frontPanelIndicator.hpp"
#include "manager.hpp"


#include <stdlib.h>  
#include "libIBus.h"


int main(int argc, char *argv[]) 
{
   if (argc != 3) {
        printf("%s : <Indicator  - Message|Power|Record|Remote|RfByPass> <brightness - [0 to 100]>\n", argv[0]);
        return 0;
    }
   
    char *pIndicatorName = argv[1];
    int indicatorBrightness = atoi((const char *)argv[2]);
    

    IARM_Bus_Init("SampleDSClient");
    IARM_Bus_Connect();    

    try {
        device::Manager::Initialize();
		printf("Sample Application: set FrontPanel brightness\n");
        device::FrontPanelIndicator::getInstance(pIndicatorName).setBrightness(indicatorBrightness);
		printf("Sample Application: set FrontPanel completed\n");
        
    }
    catch (...) {
    	printf("Exception Caught during [%s]\r\n", argv[0]);
    }

    device::Manager::DeInitialize();


	IARM_Bus_Disconnect();
	IARM_Bus_Term();
	
    return 0;
}


/** @} */
/** @} */
