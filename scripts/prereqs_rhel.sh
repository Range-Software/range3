#!/bin/bash

myPath=$(dirname $(realpath ${BASH_SOURCE[0]}))

. ${myPath}/lib.sh

echo_i "Update"
yum update -y
assert_success $? "Failed to update" || exit 2

echo_i "Install rpmfusion.org repository"
yum install -y https://download1.rpmfusion.org/free/el/rpmfusion-free-release-$(rpm -E %rhel).noarch.rpm https://download1.rpmfusion.org/nonfree/el/rpmfusion-nonfree-release-$(rpm -E %rhel).noarch.rpm
assert_success $? "Failed to install rpmfusion.org repository" || exit 2

echo_i "Install tools, Qt and ffmpeg"
yum install -y which sudo make hostname openssh-clients rpm-build gcc-c++ qt qt5-qtbase-devel qt5-qtsvg ffmpeg ffmpeg-devel
assert_success $? "Failed to install tools, Qt and ffmpeg" || exit 2

