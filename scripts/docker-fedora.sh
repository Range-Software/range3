#!/usr/bin/env bash

# This needs to work no matter where it was invoked from.
getScriptPath () {
    cd ${0%/*}/; pwd;
}
myPath=$(getScriptPath)

range_dir="$(dirname ${myPath})"

echo "Mounting $range_dir"

docker run -it --mount src="${range_dir}",target=/mnt/src,type=bind fedora /mnt/src/scripts/buildall.sh
