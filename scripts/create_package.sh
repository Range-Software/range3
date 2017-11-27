#!/bin/bash

#--------------------------------------------------------------------------
# Script to generate RPM spec file
#--------------------------------------------------------------------------

# Configuration section - Begin -------------------------------------------

selfDebug=false

myName=$(basename $0 .sh)
myPath=$(dirname  $(readlink -f ${BASH_SOURCE[0]}))
myUser=$(id -nu)
myNode=$(hostname -s)
timeStamp=$(date +%H%M%S)

. ${myPath}/lib.sh

moduleDir=$(dirname $myPath)
sourceDir="${moduleDir}/range"

# build source
buildDir="${moduleDir}/build-Release"
buildBin="${buildDir}/Range/Range ${buildDir}/RangeSolver/RangeSolver"
buildMan="${sourceDir}/Range/man/*"
buildDoc="${sourceDir}/Range/doc/*"
buildDat="${sourceDir}/Range/data/*"
buildMat="${sourceDir}/Range/materials/*"
buildLnk="${sourceDir}/Range/desktop/*"
buildPix="${sourceDir}/Range/pixmaps/range-logo-128.png"

# paths for a binaries, documents, pixmaps and other data
path_bin='bin'
path_man='man'
path_doc='doc'
path_dat='data'
path_mat='materials'
path_lnk='desktop'
path_pix='pixmaps'

# path where pacgage will be installed after un-packing
installDir='/opt/range3'

# RPM package information
summary='Range Software Package'
name='range'
release='1'
vendor='Tomas Soltys'
license='Commercial'
group='Application/Engineering'
desc='Range Software Package.\nSoftware for engineering simulations.\nFinite Element Analysis.'

version=
distro=
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
    echo "  --version=[VERSION]        Version string"
    echo "  --distro=[STRING]          Distribution string"
    echo ""
    echo " optional"
    echo ""
    echo "  --debug                    Create debug package"
    echo "  --build-dir=[DIRECTORY]    Custom build directory"
    echo "  --install-dir=[DIRECTORY]  Custom install directory"
    echo "  --help, -h, -?             Print this help and exit"
}

while [ $# -gt 0 ]
do
    case $1 in
        --install-dir=*)
            installDir=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
            ;;
        --version=*)
            version=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
            ;;
        --distro=*)
            distro=$( echo $1 | awk 'BEGIN{ FS="=" } { print $2 }' )
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

logDir="${buildDir}/log"

if [ -z "$version" ]
then
    echo_e "Missing --version parameter."
    exit 1
fi

if [ -z "$distro" ]
then
    echo_e "Missing --distro parameter."
    exit 1
fi

if [ ! -d "$buildDir" ]
then
    echo_e "Missing build directory '${buildDir}'"
    exit 1
fi

if [ $debug = true ]
then
    version+="_debug"
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
# Argument parsing section - End ------------------------------------------

# Set-up section - Begin --------------------------------------------------

# Package directory
package="${name}-${version}.${distro}"
packageDir="${buildDir}/packages"
packageSpec="${packageDir}/${package}.spec"
packageFile="${packageDir}/${package}.tar.gz"

touch_dir $packageDir
if [ $? -ne 0 ]
then
    echo_e "Failed to touch package directory '${packageDir}'"
    exit 1
fi

# source package
src=$package'.tar.gz'

source_path_bin=$buildDir'/'$path_bin
source_path_man=$buildDir'/'$path_man
source_path_doc=$buildDir'/'$path_doc
source_path_dat=$buildDir'/'$path_dat
source_path_mat=$buildDir'/'$path_mat
source_path_lnk=$buildDir'/'$path_lnk
source_path_pix=$buildDir'/'$path_pix

inst_path_bin=$installDir'/'$path_bin
inst_path_man='/usr/share/man/man1'
inst_path_doc=$installDir'/'$path_doc
inst_path_dat=$installDir'/'$path_dat
inst_path_mat=$installDir'/'$path_mat
inst_path_lnk='/usr/share/applications'
inst_path_pix=$installDir'/'$path_pix

# Set-up section - End ----------------------------------------------------

# File preparation - Begin ------------------------------------------------

touch_dir $source_path_bin true && \
touch_dir $source_path_man true && \
touch_dir $source_path_doc true && \
touch_dir $source_path_dat true && \
touch_dir $source_path_mat true && \
touch_dir $source_path_lnk true && \
touch_dir $source_path_pix true
if [ $? -ne 0 ]
then
    echo_e "Failed to touch build package directories"
    exit 1
fi

# Copy binaries
for file in $buildBin
do
    cp -v "$file" $source_path_bin
done
# Copy man pages
for file in $buildMan
do
    cp -v "$file" $source_path_man
done
# Copy documents
for file in $buildDoc
do
    cp -v "$file" $source_path_doc
done
# Copy data
for file in $buildDat
do
    cp -v "$file" $source_path_dat
done
# Copy materials
for file in $buildMat
do
    cp -v "$file" $source_path_mat
done
# Copy desktop files
for file in $buildLnk
do
    cp -v "$file" $source_path_lnk
done
# Copy pixmap files
for file in $buildPix
do
    cp -v "$file" $source_path_pix
done

# File preparation - End --------------------------------------------------

# Generation section - Begin ----------------------------------------------

packageFiles=
declare -a files
declare -a directories
m=$[1]
n=$[1]

# header
echo "Summary: ${summary}"                     > $packageSpec
echo "Name: ${name}"                          >> $packageSpec
echo "Version: ${version}.${distro}"          >> $packageSpec
echo "Release: ${release}"                    >> $packageSpec
echo "Vendor: ${vendor}"                      >> $packageSpec
echo "License: ${license}"                    >> $packageSpec
echo "Group: ${group}"                        >> $packageSpec
echo "Source: ${src}"                         >> $packageSpec
echo "BuildRoot: /var/tmp/%{name}-buildroot"  >> $packageSpec
#echo 'Requires: '$reqPkgs                     >> $packageSpec
echo                                          >> $packageSpec

# global section
echo "%global debug_package %{nil}"           >> $packageSpec

# description
echo '%description'                           >> $packageSpec
echo -e $desc                                 >> $packageSpec
echo                                          >> $packageSpec

# preparation
echo '%prep'                                  >> $packageSpec
echo '%setup -q'                              >> $packageSpec
echo                                          >> $packageSpec

# build
echo '%build'                                 >> $packageSpec
echo                                          >> $packageSpec

# install section
echo '%install'                               >> $packageSpec

# install section - create directory structure
echo 'rm -rf $RPM_BUILD_ROOT'                 >> $packageSpec
echo 'mkdir -p $RPM_BUILD_ROOT'$inst_path_bin >> $packageSpec
echo 'mkdir -p $RPM_BUILD_ROOT'$inst_path_doc >> $packageSpec
for file in $source_path_doc/*
do
    if [ -d "$file" ]; then
        echo 'mkdir -p $RPM_BUILD_ROOT'$inst_path_doc'/'`basename "$file"` >> $packageSpec
    fi
done
echo 'mkdir -p $RPM_BUILD_ROOT'$inst_path_dat >> $packageSpec
echo 'mkdir -p $RPM_BUILD_ROOT'$inst_path_mat >> $packageSpec
echo 'mkdir -p $RPM_BUILD_ROOT'$inst_path_lnk >> $packageSpec
echo 'mkdir -p $RPM_BUILD_ROOT'$inst_path_man >> $packageSpec
echo                                          >> $packageSpec

# install section - create executables
echo 'install -d -m 755 "'$path_bin'" "$RPM_BUILD_ROOT'$inst_path_bin'"' >> $packageSpec
directories[$m]=$inst_path_bin
m=$[m+1]
for file in $(ls -1 $source_path_bin)
do
    if [ $debug = true ] && [[ $file != *"_debug" ]]
    then
        continue
    fi
    if [ $debug = false ] && [[ $file == *"_debug" ]]
    then
        continue
    fi
    baseName=$(basename $file)
    src_file="${path_bin}/$baseName"
    dst_file="${inst_path_bin}/$baseName"
    packageFiles+=" ${src_file// /\\\ }"
    files[$n]=$dst_file
    n=$[n+1]
    echo "install -m 755 ${src_file} \$RPM_BUILD_ROOT${dst_file}" >> $packageSpec
done

# install section - create documents
echo 'install -d -m 755 "'$path_doc'" "$RPM_BUILD_ROOT'$inst_path_doc'"' >> $packageSpec
directories[$m]=$inst_path_doc
m=$[m+1]
for file in $source_path_doc/*
do
    baseName=$(basename "$file")
    src_file="${path_doc}/$baseName"
    dst_file="${inst_path_doc}/$baseName"
    packageFiles+=" ${src_file// /\\\ }"
    files[$n]=$dst_file
    n=$[n+1]
    echo 'install -m 644 "'$src_file'" $RPM_BUILD_ROOT"'$dst_file'"' >> $packageSpec
done

# install section - create data files
echo 'install -d -m 755 "'$path_dat'" "$RPM_BUILD_ROOT'$inst_path_dat'"' >> $packageSpec
directories[$m]=$inst_path_dat
m=$[m+1]
for file in $(ls -1 $source_path_dat)
do
    baseName=$(basename $file)
    src_file="${path_dat}/$baseName"
    dst_file="${inst_path_dat}/$baseName"
    packageFiles+=" ${src_file// /\\\ }"
    files[$n]=$dst_file
    n=$[n+1]
    echo 'install -m 644 "'$src_file'" $RPM_BUILD_ROOT"'$dst_file'"' >> $packageSpec
done

# install section - create materials
echo 'install -d -m 755 "'$path_mat'" "$RPM_BUILD_ROOT'$inst_path_mat'"' >> $packageSpec
directories[$m]=$inst_path_mat
m=$[m+1]
for file in $(ls -1 $source_path_mat)
do
    baseName=$(basename $file)
    src_file="${path_mat}/$baseName"
    dst_file="${inst_path_mat}/$baseName"
    packageFiles+=" ${src_file// /\\\ }"
    files[$n]=$dst_file
    n=$[n+1]
    echo 'install -m 644 "'$src_file'" $RPM_BUILD_ROOT"'$dst_file'"' >> $packageSpec
done

# install section - create man pages
echo 'install -d -m 755 "'$path_man'" "$RPM_BUILD_ROOT'$inst_path_man'"' >> $packageSpec
directories[$m]=$inst_path_man
m=$[m+1]
for file in $(ls -1 $source_path_man)
do
    baseName=$(basename $file)
    src_file="${path_man}/$baseName"
    dst_file="${inst_path_man}/$baseName"
    packageFiles+=" ${src_file// /\\\ }"
    files[$n]=$dst_file
    n=$[n+1]
    echo 'install -m 644 "'$src_file'" $RPM_BUILD_ROOT"'$dst_file'"' >> $packageSpec
done

# install section - create desktop files
echo 'install -d -m 755 "'$path_lnk'" "$RPM_BUILD_ROOT'$inst_path_lnk'"' >> $packageSpec
directories[$m]=$inst_path_lnk
m=$[m+1]
for file in $(ls -1 $source_path_lnk)
do
    baseName=$(basename $file)
    src_file="${path_lnk}/$baseName"
    dst_file="${inst_path_lnk}/$baseName"
    packageFiles+=" ${src_file// /\\\ }"
    files[$n]=$dst_file
    n=$[n+1]
    echo 'install -m 644 "'$src_file'" $RPM_BUILD_ROOT"'$dst_file'"' >> $packageSpec
done

# install section - create pixmap files
echo 'install -d -m 755 "'$path_pix'" "$RPM_BUILD_ROOT'$inst_path_pix'"' >> $packageSpec
directories[$m]=$inst_path_pix
m=$[m+1]
for file in $(ls -1 $source_path_pix)
do
    baseName=$(basename $file)
    src_file="${path_pix}/$baseName"
    dst_file="${inst_path_pix}/$baseName"
    packageFiles+=" ${src_file// /\\\ }"
    files[$n]=$dst_file
    n=$[n+1]
    echo 'install -m 644 "'$src_file'" $RPM_BUILD_ROOT"'$dst_file'"' >> $packageSpec
done

echo                                          >> $packageSpec

# clean
echo '%clean'                                 >> $packageSpec
echo 'rm -rf $RPM_BUILD_ROOT'                 >> $packageSpec
echo                                          >> $packageSpec

# files
echo '%files'                                 >> $packageSpec
echo '%defattr(-,root,root)'                  >> $packageSpec
echo                                          >> $packageSpec
echo '%dir '$installDir                       >> $packageSpec
for idx in ${!directories[*]}
do
    directory=${directories[$idx]}
    echo '%dir "'$directory'"'                >> $packageSpec
done
for idx in ${!files[*]}
do
    file=${files[$idx]}
    echo '"'$file'"'                          >> $packageSpec
done
echo                                          >> $packageSpec

# changelog
echo '%changelog'                             >> $packageSpec
echo                                          >> $packageSpec

# Generation section - End ------------------------------------------------

# Package creation section - Begin ----------------------------------------

echo_i "Creating package file"
eval tar -czvf $packageFile --directory=$buildDir $packageFiles
if [ $? -ne 0 ]
then
    echo_e "Failed to create package file '$packageFile'"
    exit 1
fi

tmpDir=$(mktemp -d)
if [ $? -ne 0 ]
then
    echo_e "Failed to create temporary directory"
    exit 1
fi
tmpPackageDir="$tmpDir/$package"
touch_dir $tmpPackageDir
if [ $? -ne 0 ]
then
    echo_e "Failed to touch directory '$tmpPackageDir'"
    rm -rf $tmpDir
    exit 1
fi

echo_i "Extracting package file"
tar -xvf $packageFile --directory=$tmpPackageDir
if [ $? -ne 0 ]
then
    echo_e "Failed to extract package file '$packageFile'"
    rm -rf $tmpDir
    exit 1
fi

echo_i "Creating package file"
tar -czvf $packageFile --directory=$tmpDir $package
if [ $? -ne 0 ]
then
    echo_e "Failed to create package file '$packageFile'"
    rm -rf $tmpDir
    exit 1
fi

rm -rf $tmpDir

echo_i "Creating an RPM package"
rpmBuildDir="${packageDir}/rpmbuild"
rpmbuild -bb $packageSpec --define="_topdir $rpmBuildDir" --define="_sourcedir $packageDir"

# Package creation section - End ------------------------------------------

# End of script -----------------------------------------------------------

