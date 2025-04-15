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
* @defgroup ds
* @{
**/


#ifndef VIDEOOUTPUTPORTCONFIG_HPP_
#define VIDEOOUTPUTPORTCONFIG_HPP_

#include "videoOutputPortType.hpp"
#include "videoResolution.hpp"




#include <vector>
#include <string>

namespace device {

class  VideoOutputPortConfig {

	

    std::vector<PixelResolution>	 	_vPixelResolutions;
    std::vector<AspectRatio> 			_vAspectRatios;
    std::vector<StereoScopicMode> 		_vStereoScopieModes;
    std::vector<VideoResolution> 		_supportedResolutions;
    std::vector<FrameRate> 				_vFrameRates;
    std::vector<VideoOutputPortType>	_vPortTypes;
    std::vector<VideoOutputPort>        _vPorts;


	VideoOutputPortConfig();
	~VideoOutputPortConfig();

public:
	static VideoOutputPortConfig & getInstance();

	const PixelResolution 	&getPixelResolution(int id) const;
	const AspectRatio 		&getAspectRatio(int id) const;
	const StereoScopicMode 	&getSSMode(int id) const;
	const VideoResolution   &getVideoResolution (int id) const;
	const FrameRate 		&getFrameRate(int id) const;
	VideoOutputPortType 	&getPortType(int id);
	VideoOutputPort 			&getPort(int id);
	VideoOutputPort 			&getPort(const std::string &name);
	List<VideoOutputPort> 		 getPorts();

	List<VideoOutputPortType> getSupportedTypes();
	List<VideoResolution> getSupportedResolutions(bool isIgnoreEdid=false);

	void load();
	void release();

};

}

#endif /* VIDEOOUTPUTPORTCONFIG_HPP_ */


/** @} */
/** @} */
