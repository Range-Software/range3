#!/usr/bin/env bash

getScriptPath () {
	echo ${0%/*}/
}
myPath=$(getScriptPath)

. ${myPath}/lib.sh

prereqsScript=

if [ -f "/etc/redhat-release" ]; then
    echo_i "Building on $(cat /etc/redhat-release)."
    prereqsScript="${myPath}/prereqs_fedora.sh"
elif [ $(uname) == "Darwin" ]; then
    echo_i "Building on Mac."
    prereqsScript="${myPath}/prereqs_mac.sh"
else
    echo_e "Don't know how to build on this OS ($(uname))."
    exit 1
fi

$prereqsScript && $myPath/build.sh --clean && $myPath/create_package.sh --install-to=$HOME/bin
