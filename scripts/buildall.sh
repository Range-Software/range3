#!/usr/bin/env bash

if [[ ! $(basename $(pwd)) == "scripts" ]]; then
    pushd scripts
    need_pop=1
fi

if [ -f "/etc/redhat-release" ]; then
    echo "Building on Fedora."
    ./prereqs-fedora.sh
elif [ $(uname) == "Darwin" ]; then
    echo "Building on Mac."
    ./prereqs-mac.sh
else
    echo "Don't know how to build on this OS ($(uname))."
    exit 1
fi

./build.sh --clean && ./create_package.sh --install-to=$HOME/bin

if [[ $need_pop ]]; then
    popd
fi
