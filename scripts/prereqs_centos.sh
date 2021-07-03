#!/bin/bash

getScriptPath () {
	echo ${0%/*}/
}
myPath=$(getScriptPath)

. ${myPath}/lib.sh

echo_i "Update"
yum update -y
assert_success $? "Failed to update" || exit 2

if ! rpm --quiet --query epel-release; then
    echo_i "Install EPEL repository"
    yum install -y http://dl.fedoraproject.org/pub/epel/epel-release-latest-$(rpm -E %centos).noarch.rpm
    assert_success $? "Failed to install EPEL repository" || exit 2
fi

if ! rpm --quiet --query rpmfusion-free-release; then
    echo_i "Install rpmfusion.org repository"
    yum install -y https://download1.rpmfusion.org/free/el/rpmfusion-free-release-$(rpm -E %centos).noarch.rpm
    assert_success $? "Failed to install rpmfusion.org/free repository" || exit 2
fi

if ! rpm --quiet --query rpmfusion-nonfree-release; then
    echo_i "Install rpmfusion.org repository"
    yum install -y https://download1.rpmfusion.org/nonfree/el/rpmfusion-nonfree-release-$(rpm -E %centos).noarch.rpm
    assert_success $? "Failed to install rpmfusion.org/nonfree repository" || exit 2
fi

echo_i "Install tools, Qt and ffmpeg"
yum install -y which sudo make hostname openssh-clients rpm-build gcc-c++ qt6-qtbase-devel qt6-qtsvg ffmpeg ffmpeg-devel
assert_success $? "Failed to install tools, Qt and ffmpeg" || exit 2

