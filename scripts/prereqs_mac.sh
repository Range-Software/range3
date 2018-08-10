#!/bin/bash

myPath=$(dirname $(realpath ${BASH_SOURCE[0]}))

. ${myPath}/lib.sh

# You're gonna need xcode already installed.

# Install brew if it's not there.
if [[ ! -x $(which brew) ]]; then
    echo_i "Installing brew."
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    if [ $? -ne 0 ]
    then
        echo_e "Failed to install brew"
        exit 2
    fi
fi

packagesToInstall=

if [[ ! -x $(which qmake) ]]; then
    echo_i "Instalilng qt"
    packagesToInstall+="qt "
fi

if [[ ! -x $(which ffmpeg) ]]; then
    echo_i "Instaling ffmpeg"
    packagesToInstall+="ffmpeg "
fi

if [[ ! -x /usr/local/opt/llvm/bin/clang ]]; then
    echo_i "Installing llvm/clang"
    packagesToInstall+="llvm "
fi

brew install $packagesToInstall
if [ $? -ne 0 ]
then
    echo_e "Failed to install required packages"
    exit 2
fi
