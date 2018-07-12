#!/usr/bin/env bash
if [[ ! $(basename $(pwd)) == "scripts" ]]; then
    pushd scripts
    need_pop=1
fi
./prereqs.sh
./build.sh --clean && ./create_package.sh --install-to=$HOME/bin
if [[ $need_pop ]]; then
    popd
fi
