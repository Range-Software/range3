#!/usr/bin/env bash

getScriptPath () {
	echo ${0%/*}/
}
myPath=$(getScriptPath)

. ${myPath}/lib.sh

$myPath/prereqs.sh && $myPath/build.sh --clean && $myPath/create_package.sh --install-to=$HOME/bin
