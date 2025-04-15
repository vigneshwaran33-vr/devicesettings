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
* @defgroup sample
* @{
**/


#include <iostream>
#include "host.hpp"
#include "dsUtl.h"
#include "dsError.h"
#include "manager.hpp"
#include "list.hpp"
#include "libIBus.h"
#include "sleepMode.hpp"
#include <string>
#include "list.hpp"
#include "dsConstant.hpp"
#include "dsTypes.h"

int main(int argc, char *argv[]) 
{
	std::string socID;

	IARM_Bus_Init("SampleDSClient");
	IARM_Bus_Connect();
	try{
		device::Manager::Initialize();

		socID = device::Host::getInstance().getSocIDFromSDK();
		printf("SOC ID: %s\n",socID.c_str());

    	device::Manager::DeInitialize();
	}
	catch(...)
	{
		printf("Exception caught\n");
	}
	IARM_Bus_Disconnect();
	IARM_Bus_Term();
    return 0;
}


/** @} */
/** @} */
