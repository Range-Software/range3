#!/bin/bash

moduleList="range"
qmakeCmd=$(which qmake-qt5)
if [[ ! -x $qmakeCmd ]]; then
    qmakeCmd=$(which qmake)
fi
selfDebug=false

clean=false
debug=false

myName=$(basename $0 .sh)
myPath=$(dirname  $(realpath ${BASH_SOURCE[0]}))
myUser=$(id -nu)
myNode=$(hostname -s)
timeStamp=$(date +%H%M%S)

. ${myPath}/lib.sh

topDir="$(dirname $myPath)"
moduleDir="${topDir}/range"
buildDir="${topDir}/build"

np=$[1]
MAKE="make"
if [ -f /proc/cpuinfo ]
then
    np=$(grep -c ^processor /proc/cpuinfo)
    np=$[np-1]
elif [ $(uname -s) == "Darwin" ]
then
    # This could be logical CPUs (hw.ncpu) if you wanted to use hyperthreading.
    np=$(sysctl -n hw.physicalcpu)
    maxNp=$np
    np=$[np-1]
fi
maxNp=$np

function print_help
{
    echo "Usage: $myName"
    echo "Usage: $myName.sh [OPTION]..."
    echo ""
    echo " mandatory"
    echo ""
    echo " optional"
    echo ""
    echo "  --max-nc=[NUMBER]        Maximum number of cores used to compile (default=$maxNp)"
    echo "  --clean                  Clean old build"
    echo "  --debug                  Build debug version"
    echo "  --help, -h, -?           Print this help and exit"
}

while [ $# -gt 0 ]
do
    case $1 in
        --max-nc=*)
            maxNp=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
            ;;
        --clean)
            clean=true
            ;;
        --debug)
            debug=true
            ;;
        --help | -h | -?)
            print_help; exit 0;;
        *)
            echo_e "Unknown parameter '$1'"
            exit 1
            ;;
    esac
    shift
done

if [ $np -gt $maxNp ]
then
    np=$maxNp
fi

echo_i "Using $np core(s) to compile."
if [ $np -gt $[1] ]
then
    MAKE=$MAKE" -j$np"
fi

if [ $debug = true ]
then
    buildDir+="-Debug"
else
    buildDir+="-Release"
fi

if [ $clean = true ]
then
    echo_i "Removing build directory '${buildDir}'"
    rm -rf $buildDir
    if [ $? -ne 0 ]
    then
        echo_e "Failed to remove build directory '${buildDir}'"
        exit 1
    fi
fi

logDir="${buildDir}/log"

if [ ! -x $qmakeCmd ]
then
    echo_e "File is not executable '${qmakeCmd}'"
    exit 1
fi

touch_dir $buildDir
if [ $? -ne 0 ]
then
    echo_e "Failed to touch build directory '${buildDir}'"
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
qmakeLogFile="${logDir}/${myName}-${timeStamp}-qmake.log"
makeLogFile="${logDir}/${myName}-${timeStamp}-make.log"

qmakeArgs="-recursive"
if [ $debug = true ]
then
    qmakeArgs+=" CONFIG+=debug CONFIG+=qml_debug"
fi

currentDir=$(pwd)

cd $buildDir

if [[ $(uname -s) == "Darwin" ]]; then
    # This doesn't work yet.
    echo_i "Setting path to include llvm clang for OpenMP support."
    OLDPATH=$PATH
    export PATH=/usr/local/opt/llvm/bin:$PATH
    export LLVM_INCLUDE_FLAGS="-L/usr/local/opt/llvm/lib -I/usr/local/opt/llvm/include"
    export CC=clang
    export CXX=clang++

    echo_i "Jeez, what does it take to get qmake to use the right compiler?"
    qmakeArgs+=" CC=/usr/local/opt/llvm/bin/clang CXX=/usr/local/opt/llvm/bin/clang++"
fi

for module in $moduleList
do
    projectFile="$moduleDir/$module.pro"
    makefile="Makefile"

    echo_i "Building $module"
    set_indent
    # QMAKE
    echo_i "Running qmake as $qmakeCmd"
    $qmakeCmd $projectFile $qmakeArgs -o $makefile | tee -a $qmakeLogFile
    if [ ${PIPESTATUS[0]} -ne 0 ]
    then
        echo_e "Command '$qmakeCmd $projectFile $qmakeArgs -o $makefile' failed. Check log file '$qmakeLogFile' for errors"
        cd $currentDir
        exit 1
    fi
    # MAKE
    echo_i "Running make"
    $MAKE | tee -a $makeLogFile
    if [ ${PIPESTATUS[0]} -ne 0 ]
    then
        echo_e "Command '$MAKE' failed. Check log file '$makeLogFile' for errors"
        cd $currentDir
        exit 1
    fi
    set_unindent
done

cd $currentDir
