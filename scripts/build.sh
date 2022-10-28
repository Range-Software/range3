#!/bin/bash

moduleList="range"
selfDebug=false

clean=false
debug=false
makeInstall=false

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

qmakeCmd="qmake6"
if ! which $qmakeCmd
then
    echo_e "File does not exist '${qmakeCmd}'"
    qmakeCmd="qmake"
    if ! which $qmakeCmd
    then
        echo_e "File does not exist '${qmakeCmd}'"
        exit 1
    fi
fi

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

print_help()
{
cat <<End-of-help
Usage: $myName
Usage: $myName.sh [OPTION]...

  mandatory

  optional

    --max-nc=[NUMBER]        Maximum number of cores used to compile (default=$maxNp)
    --install                Run make install
    --clean                  Clean old build
    --debug                  Build debug version
    --help, -h, -?           Print this help and exit
End-of-help
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
        --install)
            makeInstall=true
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

pushd $buildDir

if [[ $(uname -s) == "Darwin" ]]; then
    qmakeArgs+=" -spec macx-clang"

    override_clang="true"
    if [ "$override_clang" == "true" ]
    then
        qmake_cc="$(which clang)"
        qmake_link_c="$(which clang++)"
        qmake_cxx="$(which clang++)"
        qmake_link="$(which clang++)"
        qmakeArgs+=" QMAKE_CC=$qmake_cc QMAKE_CXX=$qmake_cxx QMAKE_LINK_C=$qmake_link_c QMAKE_LINK=$qmake_link"
    fi
    includePath="$HOMEBREW_REPOSITORY/include"
    libDirectory="$HOMEBREW_REPOSITORY/lib"
    qmakeArgs+=" QMAKE_INCDIR=$includePath QMAKE_LIBDIR=$libDirectory"
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
        popd
        exit 1
    fi
    # MAKE
    echo_i "Running make"
    $MAKE | tee -a $makeLogFile
    if [ ${PIPESTATUS[0]} -ne 0 ]
    then
        echo_e "Command '$MAKE' failed. Check log file '$makeLogFile' for errors"
        popd
        exit 1
    fi
    if [ "$makeInstall" == "true" ]
    then
        # INSTALL
        echo_i "Running make install"
        $MAKE install | tee -a $makeLogFile
        if [ ${PIPESTATUS[0]} -ne 0 ]
        then
            echo_e "Command '$MAKE install' failed. Check log file '$makeLogFile' for errors"
            popd
            exit 1
        fi
    fi
    set_unindent
done

popd
