#!/bin/bash

getScriptPath () {
	echo ${0%/*}/
}
myPath=$(getScriptPath)

. ${myPath}/lib.sh

echo_i "Update"
apt-get update -q
assert_success $? "Failed to update" || exit 2

echo_i "Install software-properties-common"
DEBIAN_FRONTEND=noninteractive apt-get install -y software-properties-common
assert_success $? "Failed to install software-properties-common" || exit 2

echo_i "Install tools, Qt and ffmpeg"
DEBIAN_FRONTEND=noninteractive apt-get install -y pkg-config build-essential alien openssh-client rpm qt6-base-dev qt6-pdf-dev libqt6svg6-dev libgl1-mesa-dev libavutil-dev libavformat-dev libswscale-dev libavdevice-dev ffmpeg
assert_success $? "Failed to install tools, Qt and ffmpeg" || exit 2

