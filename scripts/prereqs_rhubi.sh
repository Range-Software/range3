#!/bin/bash

getScriptPath () {
	echo ${0%/*}/
}
myPath=$(getScriptPath)

. ${myPath}/lib.sh

echo_i "Remove RH Subscription"
subscription-manager config --rhsm.manage_repos=0
assert_success $? "Failed to remove subscription" || exit 2

echo_i "Update"
yum update -y
assert_success $? "Failed to update" || exit 2

echo_i "Install tools, Qt and ffmpeg"
yum install -y which sudo make hostname openssh-clients rpm-build gcc-c++ qt6-qtbase-devel qt6-qtsvg ffmpeg ffmpeg-devel
assert_success $? "Failed to install tools, Qt and ffmpeg" || exit 2

