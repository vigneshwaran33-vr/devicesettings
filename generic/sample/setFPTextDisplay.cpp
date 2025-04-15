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
#include <unistd.h>
#include <stdlib.h>
#include "manager.hpp"

#include "frontPanelConfig.hpp"
#include "frontPanelTextDisplay.hpp"
#include <stdlib.h>  


#include "libIBus.h"




int main(int argc, char *argv[]) 
{
   
    int i = 0;
	
	IARM_Bus_Init("SampleDSClient");
	IARM_Bus_Connect();
    
    try {

           device::Manager::Initialize();
   
           if (argc != 2) {
               printf("%s : <Text Message [3 Chars]>\n", argv[0]);
               return 0;
           }
           char *Message = argv[1];
   
	       int bright = device::FrontPanelConfig::getInstance().getIndicator("Power").getBrightness();
	       printf("Power : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getIndicator("Message").getBrightness();
	       printf("Message : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getIndicator("Record").getBrightness();
	       printf("Record : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getIndicator("RfByPass").getBrightness();
	       printf("RfByPass : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getTextDisplay("Text").getTextBrightness();
	       printf("Text : brightness is %d\n",bright);
       
	       sleep(20);
       
	       device::FrontPanelConfig::getInstance().getIndicator("Power").setBrightness(i);
	       device::FrontPanelConfig::getInstance().getIndicator("Message").setBrightness(i);
	       device::FrontPanelConfig::getInstance().getIndicator("Record").setBrightness(i);
	       device::FrontPanelConfig::getInstance().getIndicator("RfByPass").setBrightness(i);
	       device::FrontPanelConfig::getInstance().getTextDisplay("Text").setTextBrightness(i);
   
	       sleep(20);
   
	       bright = device::FrontPanelConfig::getInstance().getIndicator("Power").getBrightness();
	       printf("Power : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getIndicator("Message").getBrightness();
	       printf("Message : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getIndicator("Record").getBrightness();
	       printf("Record : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getIndicator("RfByPass").getBrightness();
	       printf("RfByPass : brightness is %d\n",bright);
	       bright = device::FrontPanelConfig::getInstance().getTextDisplay("Text").getTextBrightness();
	       printf("Text : brightness is %d\n",bright);

		   printf("Sample Application: set text display------- %s\n",Message);
           device::FrontPanelConfig::getInstance().getTextDisplay("Text").setText(Message);
		 	 
           for (i=10;i < 100 ; )
           {
				printf("Sample Application: set text brightness------- %d\n",i);
				device::FrontPanelConfig::getInstance().getTextDisplay("Text").setTextBrightness(i);
				device::FrontPanelConfig::getInstance().getIndicator("Power").setBrightness(i);
				i = i+ 10;
				sleep(10);
		   }
       
			bright = device::FrontPanelConfig::getInstance().getIndicator("Power").getBrightness();
			printf("Power : brightness is %d\n",bright);
			bright = device::FrontPanelConfig::getInstance().getIndicator("Message").getBrightness();
			printf("Message : brightness is %d\n",bright);
			bright = device::FrontPanelConfig::getInstance().getIndicator("Record").getBrightness();
			printf("Record : brightness is %d\n",bright);
			bright = device::FrontPanelConfig::getInstance().getIndicator("RfByPass").getBrightness();
			printf("RfByPass : brightness is %d\n",bright);
			bright = device::FrontPanelConfig::getInstance().getTextDisplay("Text").getTextBrightness();
			printf("Text : brightness is %d\n",bright);
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
