#!/bin/bash

#--------------------------------------------------------------------------
# Script to deploy qt files for provided executable on linux
#--------------------------------------------------------------------------

myName=$(basename $0 .sh)
myUser=$(id -nu)
myNode=$(hostname -s)
timeStamp=$(date +%H%M%S)

getScriptPath()
{
	echo ${0%/*}/
}
realPath()
{
    local _path=$1
    local _dirPath=$_path
    if [ ! -d "$_path" ]
    then
        _dirPath=$(dirname $_dirPath)
    fi
    pushd "$_dirPath" &>/dev/null
    _dirPath=$(pwd)
    popd &>/dev/null
    if [ -d "$_path" ]
    then
        _path=$_dirPath
    else
        _path="$_dirPath/$(basename $_path)"
    fi
    echo $_path
}

myPath="$(realPath $(getScriptPath))"

. ${myPath}/lib.sh

executable=
installToDir=
qtDir=
qtImageFormatsDir="plugins/imageformats"
qtPlatformsDir="plugins/platforms"
qtPlatformThemesDir="plugins/platformthemes"
qtTlsDir="plugins/tls"
qtXcbGlIntegrationsDir="plugins/xcbglintegrations"
onlyQt=false

print_help()
{
cat <<End-of-help
Usage: $myName
Usage: $myName.sh [OPTION]...

  mandatory

    --executable=[EXECUTABLE]      Path to executable for which resources will be collected
    --qt-path=[DIRECTORY]          Path to Qt installation

  optional

    --install-to=[DIRECTORY]       Install collected resources to 'DIRECTORY' (default=\$(dirname EXEUTABLE))
    --only-qt                      Install only Qt libraries

    --help, -h, -?                 Print this help and exit
End-of-help
}

while [ $# -gt 0 ]
do
    case $1 in
        --executable=*)
            executable=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
            ;;
        --install-to=*)
            installToDir=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
            ;;
        --qt-path=*)
            qtDir=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
            ;;
        --only-qt)
            onlyQt=true
            ;;
        --help | -h | -?)
            print_help
            exit 0
            ;;
        *)
            echo_e "Unknown parameter '$1'"
            print_help
            exit 1
            ;;
    esac
    shift
done

if [ -z "$executable" ]
then
    echo_e "Missing executable"
    exit 1
fi

if [ -z "$qtDir" ]
then
    echo_e "Missing path to Qt installation"
    exit 1
fi

if [ ! -d "$qtDir" ]
then
    echo_e "Qt installation directory '$qtDir' not found"
    exit 1
fi

if [ ! -d "$qtDir/$qtImageFormatsDir" ]
then
    echo_e "Qt directory '$qtDir/$qtImageFormatsDir' not found"
    exit 1
fi
if [ ! -d "$qtDir/$qtPlatformsDir" ]
then
    echo_e "Qt directory '$qtDir/$qtPlatformsDir' not found"
    exit 1
fi
if [ ! -d "$qtDir/$qtPlatformThemesDir" ]
then
    echo_e "Qt directory '$qtDir/$qtPlatformThemesDir' not found"
    exit 1
fi
if [ ! -d "$qtDir/$qtTlsDir" ]
then
    echo_e "Qt directory '$qtDir/$qtTlsDir' not found"
    exit 1
fi
if [ ! -d "$qtDir/$qtXcbGlIntegrationsDir" ]
then
    echo_e "Qt directory '$qtDir/$qtXcbGlIntegrationsDir' not found"
    exit 1
fi

if [ -z "$installToDir" ]
then
    installToDir=$(dirname $executable)
fi

executable=$(realPath $executable)
installToDir=$(realPath $installToDir)

echo_i "Executable: $executable"
echo_i "Installation directory: $installToDir"

touch_dir $installToDir/lib true
touch_dir $installToDir/$qtImageFormatsDir true
touch_dir $installToDir/$qtPlatformsDir true
touch_dir $installToDir/$qtPlatformThemesDir true
touch_dir $installToDir/$qtTlsDir true
touch_dir $installToDir/$qtXcbGlIntegrationsDir true

getLibraries()
{
    onlyQtGrep=
    if [ $onlyQt == "true" ]
    then
        onlyQtGrep=".*Qt"
    fi

    for lddRecord in $(ldd $executable $qtDir/$qtImageFormatsDir/*.so $qtDir/$qtPlatformsDir/*.so $qtDir/$qtPlatformThemesDir/*.so $qtDir/$qtTlsDir/*.so $qtDir/$qtXcbGlIntegrationsDir/*.so | grep -e "^\s*lib$onlyQtGrep\|^\s*libavcodec\|^\s*libavdevice\|^\s*libavfilter\|^\s*libavformat\|^\s*libavutil\|^\s*libpostproc\|^\s*libswresample\|^\s*libswscale" | sed 's/^\s\+//;s/\s\+=>\s\+/,/;s/\s\+.*//')
    do
        libraryName=$(echo $lddRecord | cut -d',' -f1)
        libraryPath=$(realPath $(echo $lddRecord | cut -d',' -f2))
        echo $libraryName,$libraryPath
    done
}

for libRecord in $(getLibraries | sort -u)
do
    libraryName=$(echo $libRecord | cut -d',' -f1)
    libraryPath=$(echo $libRecord | cut -d',' -f2)
    cp -v $libraryPath $installToDir/lib/$libraryName
    if [ $? -ne 0 ]
    then
        echo_e "Failed to copy library '$libraryPath' to '$installToDir/lib/$libraryName'"
    fi
done

cp -v $qtDir/$qtImageFormatsDir/*.so $installToDir/$qtImageFormatsDir && \
cp -v $qtDir/$qtPlatformsDir/*.so $installToDir/$qtPlatformsDir && \
cp -v $qtDir/$qtPlatformThemesDir/*.so $installToDir/$qtPlatformThemesDir && \
cp -v $qtDir/$qtTlsDir/*.so $installToDir/$qtTlsDir
cp -v $qtDir/$qtXcbGlIntegrationsDir/*.so $installToDir/$qtXcbGlIntegrationsDir
if [ $? -ne 0 ]
then
    echo_e "Failed to copy Qt plugins"
fi
