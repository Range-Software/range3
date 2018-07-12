#!/usr/bin/env bash

# You're gonna need xcode already installed.

# Install brew if it's not there.
if [[ ! -x $(which brew) ]]; then
    echo "Installing brew."
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

if [[ ! -x $(which qmake) ]]; then
    echo "Instalilng qt"
    brew install qt
fi

if [[ ! -x $(which ffmpeg) ]]; then
    echo "Instaling ffmpeg"
    brew install ffmpeg
fi
