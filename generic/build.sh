#!/bin/bash
##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2016 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
#

SCRIPT=$(readlink -f "$0")
SCRIPTS_DIR=`dirname "$SCRIPT"`
export BUILDS_DIR=$SCRIPTS_DIR/../
export COMBINED_DIR=$SCRIPTS_DIR/..

buildReport=$BUILDS_DIR/../Logs/buildDeviceSettReport.txt


    echo "Setting hal Environment variables..."
    source $PWD/hal/src/halenv.sh
    echo "Building Device Setting libraries..."
make
make all >> $buildReport 2>> $buildReport
if [ $? -ne 0 ] ; then
  echo "DeviceSettings Build Failed..."
  exit 1
else
  echo "DeviceSettings Build Success.."
  exit 0
fi

