#!/bin/bash

#--------------------------------------------------------------------------
# Script to generate RPM spec file
#--------------------------------------------------------------------------

# Configuration section - Begin -------------------------------------------

selfDebug=false

myName=$(basename $0 .sh)
myUser=$(id -nu)
myNode=$(hostname -s)
timeStamp=$(date +%H%M%S)

getScriptPath () {
	echo ${0%/*}/
}
pushd "$(getScriptPath)"
myPath="$(pwd)"
popd

. ${myPath}/lib.sh

moduleDir=$(dirname $myPath)
sourceDir="${moduleDir}/range"

# paths for a binaries, documents, pixmaps and other data
path_bin='bin'
path_man='man'
path_doc='doc'
path_hlp='help'
path_dat='data'
path_mat='materials'
path_lnk='desktop'
path_pix='pixmaps'

# Package information
summary='Range Software Package'
name='range'
release='1'
vendor='Tomas Soltys'
license='Commercial'
group='Application/Engineering'
desc='Range Software Package.\nSoftware for engineering simulations.\nFinite Element Analysis.'

installToDir=$HOME/bin
debug=false

# Configuration section - End ---------------------------------------------

# Argument parsing section - Begin ----------------------------------------

function print_help
{
    echo "Usage: $myName"
    echo "Usage: $myName.sh [OPTION]..."
    echo ""
    echo " mandatory"
    echo ""
    echo " optional"
    echo ""
    echo "  --install-to=[DIRECTORY]       Install created package to 'DIRECTORY' (default='$installToDir')"
    echo ""
    echo "  --debug                        Create debug package"
    echo ""
    echo "  --build-dir=[DIRECTORY]        Custom build directory"
    echo "  --help, -h, -?                 Print this help and exit"
}

while [ $# -gt 0 ]
do
    case $1 in
        --install-to=*)
            installToDir=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
            ;;
        --debug)
            debug=true
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

# build source
buildType="Release"
versionSuffix=

if [ $debug = true ]
then
    buildType="Debug"
    versionSuffix="_debug"
fi

buildDir="${moduleDir}/build-${buildType}"
buildApp="${buildDir}/Range/Range${versionSuffix}.app"
buildRangeBin="${buildDir}/RangeSolver/Range${versionSuffix}"
buildRangeSolverBin="${buildDir}/RangeSolver/RangeSolver${versionSuffix}"
buildManDir="${sourceDir}/Range/man"
buildDocDir="${sourceDir}/Range/doc"
buildHlpDir="${sourceDir}/Range/help"
buildDatDir="${sourceDir}/Range/data"
buildMatDir="${sourceDir}/Range/materials"
buildPixDir="${sourceDir}/Range/pixmaps"
buildRangeLogo="${buildPixDir}/range-logo-128.png"

logDir="${buildDir}/log"

if [ ! -d "$buildDir" ]
then
    echo_e "Missing build directory '${buildDir}'"
    exit 1
fi

touch_dir $logDir
if [ $? -ne 0 ]
then
    echo_e "Failed to touch log directory '${logDir}'"
    exit 1
fi

_BUILD_LOG_FNAME="${logDir}/${myName}-${timeStamp}.log"
if [ $selfDebug = true ]
then
    _DEBUG_LOG_FNAME="${logDir}/op-${myName}-${timeStamp}.log"
fi

version="$($buildDir/RangeSolver/RangeSolver$versionSuffix --version | sed -n -e '/VERSION/ s/.*\= *//p')${versionSuffix}"
# Argument parsing section - End ------------------------------------------

# Set-up section - Begin --------------------------------------------------

# Package directory
packageVersion=$version
if [ ! -z "$distro" ]
then
    packageVersion+=".${distro}"
fi
package="${name}-${packageVersion}"
packagesDir="${buildDir}/packages"
packageDir="${packagesDir}/$package"
packageFile="${packagesDir}/${package}.dmg"

packageAppDir="${packageDir}/Range${versionSuffix}.app"
packageBinDir="${packageAppDir}/Contents/MacOS"
packageDocDir="${packageAppDir}/Contents/Resources/doc"
packageMatDir="${packageAppDir}/Contents/Resources/materials"
packageDatDir="${packageAppDir}/Contents/Resources/data"
packageHlpDir="${packageAppDir}/Contents/Resources/help"
packagePixDir="${packageAppDir}/Contents/Resources/pixmaps"
packagePlist="${packageAppDir}/Contents/Info.plist"

packageRangeSolverBin="${packageBinDir}/RangeSolver${versionSuffix}"

touch_dir $packageDir true && \
touch_dir $packageBinDir && \
touch_dir $packageDocDir && \
touch_dir $packageMatDir && \
touch_dir $packageDatDir && \
touch_dir $packageHlpDir && \
touch_dir $packagePixDir
if [ $? -ne 0 ]
then
    echo_e "Failed to touch directories"
    exit 1
fi

# Set-up section - End ----------------------------------------------------

# File preparation - Begin ------------------------------------------------
 
cp -vR $buildApp/* $packageAppDir && \
cp -v $buildRangeSolverBin $packageBinDir && \
cp -v $buildDocDir/* $packageDocDir && \
cp -v $buildDatDir/* $packageDatDir && \
cp -v $buildMatDir/* $packageMatDir && \
cp -vR $buildHlpDir/* $packageHlpDir && \
cp -v $buildRangeLogo $packagePixDir
if [ $? -ne 0 ]
then
    echo_e "Failed to copy files"
    exit 1
fi

macdeployqt $packageAppDir -executable=$packageRangeSolverBin -verbose=2
if [ $? -ne 0 ]
then
    echo_e "Failed to run macdeployqt"
    exit 1
fi

plutil -replace CFBundleIdentifier -string "Range-Software" $packagePlist && \
plutil -replace CFBundleVersion -string "$version" $packagePlist
if [ $? -ne 0 ]
then
    echo_e "Failed to edit application plist file"
    exit 1
fi

# File preparation - End --------------------------------------------------

# Package creation section - Begin ----------------------------------------

rm -rf "$packageFile"
if [ $? -ne 0 ]
then
    echo_e "Failed to delete olf package file '$packageFile'"
    exit 1
fi

create-dmg --volname "Range $version installer" \
           --volicon "$buildPixDir/range.icns" \
           --window-pos 200 120 \
           --window-size 800 400 \
           --icon-size 100 \
           --icon "Range.app" 200 190 \
           --hide-extension "Range.app" \
           --app-drop-link 600 185 \
           "$packageFile" \
           "$packageDir"
if [ $? -ne 0 ]
then
    echo_e "Failed to create package file '$packageFile'"
    exit 1
fi

# Package creation section - End ------------------------------------------

echo_i "Created installer package '$packageFile'"
echo_i "Install with:"
echo_i "  open '$packageFile'"

# End of script -----------------------------------------------------------
