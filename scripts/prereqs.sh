#!/usr/bin/env bash

getScriptPath () {
	echo ${0%/*}/
}
myPath=$(getScriptPath)

. ${myPath}/lib.sh

prereqsScript=

if [ -f "/etc/redhat-release" ]; then
    # RedHat based Linux
    grep -qi "fedora" /etc/redhat-release
    if [ $? -eq 0 ]; then
        echo_i "Building on $(cat /etc/redhat-release)"
        prereqsScript="${myPath}/prereqs_fedora.sh"
    fi
    if [ -z "$prereqsScript" ]; then
        grep -qi "centos" /etc/redhat-release
        if [ $? -eq 0 ]; then
            echo_i "Building on $(cat /etc/redhat-release)"
            prereqsScript="${myPath}/prereqs_centos.sh"
        fi
    fi
    if [ -z "$prereqsScript" ]; then
        grep -qi "rhel" /etc/redhat-release
        if [ $? -eq 0 ]; then
            echo_i "Building on $(cat /etc/redhat-release)"
            prereqsScript="${myPath}/prereqs_rhubi.sh"
        fi
    fi
    if [ -z "$prereqsScript" ]; then
        echo_e "Don't know how to build on $(cat /etc/redhat-release)"
    fi
elif [ -f "/etc/os-release" ]; then
    # Suse based Linux
    . /etc/os-release
    echo $ID_LIKE | grep -qi "suse"
    if [ $? -eq 0 ]; then
        echo_i "Building on $PRETTY_NAME"
        prereqsScript="${myPath}/prereqs_opensuse.sh"
    fi
elif [ -f "/etc/issue" ]; then
    # Ububntu
    echo_i "Building on $(cat /etc/issue)"
    prereqsScript="${myPath}/prereqs_ubuntu.sh"
elif [ $(uname) == "Darwin" ]; then
    # MacOS
    echo_i "Building on Mac."
    prereqsScript="${myPath}/prereqs_mac.sh"
fi

if [ -z "$prereqsScript" ]; then
    echo_e "Don't know how to build on this OS ($(uname))"
    exit 1
fi

$prereqsScript
assert_success $? "Failed to prepare build environment" || exit 2
