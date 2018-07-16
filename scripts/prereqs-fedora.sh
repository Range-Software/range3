#!/usr/bin/env bash

if [[ ! -x $(which qmake) ]]; then
    echo "Instalilng qt and qt5-qtbase-devel"
    sudo yum install qt qt5-qtbase-devel
fi

if [[ ! -x $(which ffmpeg) ]]; then
    echo "Instaling ffmpeg"
    sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm
    sudo yum install ffmpeg ffmpeg-devel
fi
