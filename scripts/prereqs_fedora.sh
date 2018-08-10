#!/bin/bash

myPath=$(dirname $(realpath ${BASH_SOURCE[0]}))

. ${myPath}/lib.sh

packagesToInstall=

qt5Pkg=$(rpm -q qt5-qtbase-devel qt5-qtsvg 2> /dev/null)
if [ $? -ne 0 ]
then
    echo_i "Instalilng qt"
    packagesToInstall+="qt qt5-qtbase-devel qt5-qtsvg "
fi

ffmpegPkg=$(rpm -q ffmpeg-devel 2> /dev/null)
if [ $? -ne 0 ]
then
    echo_i "Instaling ffmpeg"
    packagesToInstall+="ffmpeg ffmpeg-devel "
    sudo dnf install -y https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm
    if [ $? -ne 0 ]
    then
        echo_e "Failed to install rpmfusion repositories"
        exit 2
    fi
fi

if [ ! -z "$packagesToInstall" ]
then
    sudo dnf install -y $packagesToInstall
    if [ $? -ne 0 ]
    then
        echo_e "Failed to install required packages"
        exit 2
    fi
fi
