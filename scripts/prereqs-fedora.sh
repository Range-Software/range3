#!/usr/bin/env bash

if [[ ! -x /usr/bin/which ]]; then
    # This is probably our super basic Docker Fedora image, so install this stuff:
    dnf install -y which sudo make
fi

if [[ ! -x $(which qmake) ]]; then
    echo "Instalilng qt and qt5-qtbase-devel"
    sudo dnf install -y qt qt5-qtbase-devel
fi

if [[ ! -x $(which ffmpeg) ]]; then
    echo "Instaling ffmpeg"
    sudo dnf install -y https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm
    sudo dnf install -y ffmpeg ffmpeg-devel
fi
