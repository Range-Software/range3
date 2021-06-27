#!/bin/bash

getScriptPath () {
	echo ${0%/*}/
}
myPath=$(getScriptPath)

. ${myPath}/lib.sh

echo_i "Update"
dnf update -y
assert_success $? "Failed to update" || exit 2

echo_i "Install rpmfusion.org repository"
dnf install -y https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm
assert_success $? "Failed to install rpmfusion.org repository" || exit 2

echo_i "Install tools, Qt and ffmpeg"
dnf install -y which sudo make hostname openssh-clients rpm-build qt qt6-qtbase-devel qt6-qtsvg ffmpeg ffmpeg-devel
assert_success $? "Failed to install tools, Qt and ffmpeg" || exit 2

