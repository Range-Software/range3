#!/bin/bash

if [ -z "$_BUILD_LOG_FNAME" ]
then
    _BUILD_LOG_FNAME=""
fi

if [ -z "$_DEBUG_LOG_FNAME" ]
then
    _DEBUG_LOG_FNAME=""
fi

if [ -z "$_ECHO_INTEND" ]
then
    _ECHO_INTEND=$[0]
fi

function set_indent
{
    _ECHO_INTEND=$[_ECHO_INTEND+1]
}

function set_unindent
{
    _ECHO_INTEND=$[_ECHO_INTEND-1]
    if [ $_ECHO_INTEND -lt 0 ]
    then
        _ECHO_INTEND=$[0]
    fi
}

function get_indent
{
    echo $_ECHO_INTEND
}

function get_indent_text
{
    local intMessage=""
    local int=$[0]
    while [ $int -lt $_ECHO_INTEND ]
    do
        intMessage+=" > "
        int=$[int+1]
    done
    echo "$intMessage"
}

function get_log_time
{
    local logTimeStr="$(date +%d-%b-%Y) $(date +%T)"
    echo $logTimeStr
}

function echo_stack
{
    local nIgnore=0
    if [ ! -z "$1" ]
    then
        nIgnore=$1
    fi

    local i=$[0]
    local n=$[0]
    while [ ! -z "${FUNCNAME[$i]}" ]
    do
        if [ $i -ge $nIgnore ]
        then
            local currLineNo=0
            if [ $i -eq 0 ];then
                currLineNo=$LINENO
            else
                currLineNo=${BASH_LINENO[$i-1]}
            fi
            echo "  $n -> ${BASH_SOURCE[$i]}:$currLineNo:${FUNCNAME[$i]}"
            n=$[n+1]
        fi
        i=$[i+1]
    done
}

function echo_f
{
    if [ $# -gt 1 ]
    then
        local logFile=$1
        local logDir=$(dirname "$logFile")
        if [ -d "$logDir" ]
        then
            shift
            echo "$@" >> $logFile
        fi
    fi
}

function echo_i
{
    local message="-I-|$(get_log_time)|$(get_indent_text)""$@"
    echo "$message" >&1
    if [ ! -z "$_BUILD_LOG_FNAME" ]
    then
        # log into the build log
        echo_f $_BUILD_LOG_FNAME "$message"
    fi
    if [ -n "${_DEBUG_LOG_FNAME}" ]
    then
        # log into the debug log
        message+=" (called from $(basename ${BASH_SOURCE[1]})(${BASH_LINENO[0]}):${FUNCNAME[1]})"
        echo_f "${_DEBUG_LOG_FNAME}" "$message"
    fi
}

function echo_w
{
    local message="-W-|$(get_log_time)|$(get_indent_text)""$@"
    echo "$message" >&2
    if [ ! -z "$_BUILD_LOG_FNAME" ]
    then
        # log into the build log
        echo_f $_BUILD_LOG_FNAME "$message"
    fi
    if [ -n "${_DEBUG_LOG_FNAME}" ]
    then
        # log into the debug log
        message+=" (called from $(basename ${BASH_SOURCE[1]})(${BASH_LINENO[0]}):${FUNCNAME[1]})"
        echo_f "${_DEBUG_LOG_FNAME}" "$message"
    fi
}

function echo_e
{
    local message="-E-|$(get_log_time)|$(get_indent_text)""$@"
    echo "$message" >&2
    if [ ! -z "$_BUILD_LOG_FNAME" ]
    then
        # log into the build log
        echo_f $_BUILD_LOG_FNAME "$message"
    fi
    if [ -n "${_DEBUG_LOG_FNAME}" ]
    then
        # log into the debug log
        message+=" (called from $(basename ${BASH_SOURCE[1]})(${BASH_LINENO[0]}):${FUNCNAME[1]})"
        echo_f "${_DEBUG_LOG_FNAME}" "$message"
    fi
}

function assert_success
{
    local _return_value=$1
    local _error_message=$2
    local _exit=$3
    if [ $_return_value -ne 0 ]; then
        echo_e "$_error_message"
        if [ "$_exit" = "true" ]; then
            exit 2
        fi
    fi  
    return $_return_value
}

function touch_dir
{
    local dir=$1
    local clean=$2
    if [ -z "$dir" ]
    then
        echo_e "Missing directory"
        return 1
    fi
    if [ -d "$dir" ]
    then
        echo_i "Directory '${dir}' already exists"
        if [ "$clean" = true ]
        then
            rm -rfv $dir/*
        fi
        return 0
    fi
    echo_i "Creating directory '${dir}'"
    mkdir -p $dir
    if [ $? -ne 0 ]
    then
        echo_e "Failed to create a directory '${dir}'"
        return 1
    fi
    return 0
}

