#!/bin/sh


# conf2build.sh: Script to create file list, configuration
#   headers and project files for applications based on a feature set
#   specifying conf(ig) file
#
# (C) Copyright 2009 - 2010 by OSB AG and developing partners
#
# See the repository-log for details on the authors and file-history.
# (Repository information can be found at <http://isoaglib.com/download>)
#
# Use, modification and distribution are subject to the GNU General
# Public License. (See accompanying file LICENSE.txt)

# Prologue
POSIXLY_CORRECT=y # for the case that bash is running
unset -f unalias
\unalias -a
unset -f command
SYSPATH="$(command -p getconf PATH 2>/dev/null)"
: ${SYSPATH:=/usr/bin:/bin}
PATH="$SYSPATH:$PATH"
umask 022
set -o nounset
set -o errexit

# global constants:
TAB="$(printf '\t')"
LF="
"
PATH_SEPARATOR1='=_=_'
PATH_SEPARATOR2='_=_='
TEMPFILE_PREFIX="/tmp/conf2build$$"

main()
{
    case `uname -s` in
        *CYGWIN*|*MINGW*) SYSTEM='WINDOWS' ;;
        *)                SYSTEM='LINUX' ;;
    esac
    
    set_default_values
    check_before_user_configuration "$@"
    
    # now source the config setting file
    for i in $CONF_FILES;
    do
      . $i
    done

    check_after_user_configuration
    create_buildfiles "$CONF_DIR" "$SCRIPT_DIR"
    report_summary
}

# Prefer these functions to echo due to incompatible versions (Bourne
# shell builtin, Bash builtin, command):
echo_() { printf '%s\n' "$*"; }
echo_e() { printf '%b\n' "$*"; }
echo_e_n() { printf '%b' "$*"; }
echo_n() { printf '%s' "$*"; }
# Compatible but slow variant as fallback:
eval 'echo() { '$(which echo)' "$@"; }'

# wrap words into lines
wrap()
{
    printf '%s\n' "$@"
}

# Escapes % and \ to get a literal printf-format:
literal_format()
{
    printf '%s' "$1" | sed -e 's|[%\\]|&&|g'
}

# Joins to one string.
# E.g. the following command prints "a,bc,def":
#   join , a bc def
join()
{
    local IFS="$1"
    shift
    printf '%s' "$*"
}

join_comma() { join ',' "$@"; }
join_semicolon() { join ';' "$@"; }
join_space() { join ' ' "$@"; }
prefix_library_path() { printf -- '-L%s' "$@"; }

# map CONTINUATION FUNCTION ARG ...
# apply FUNCTION to each ARG and continue with CONTINUATION
map()
{
    local CONTINUE="$1"
    local F="$2"
    shift 2
    for A in "$@"; do
        shift
        set -- "$@" "$("$F" "$A")"
    done
    "$CONTINUE" "$@"
}

status_le1() { [ $? -le 1 ]; }

# this function is used to verify and
# correct the several project configuration
# variables
# if one of the following variables isn't set
# the corresponding default values are used
set_default_values()
{
    PARAMETER_TARGET_SYSTEM="UseConfigFile"
    PARAMETER_CAN_DRIVER="UseConfigFile"
    PARAMETER_RS232_DRIVER="UseConfigFile"
    PARAMETER_EEPROM_DRIVER="UseConfigFile"
    # may be overridden by configuration file or check_after_user_configuration:
    USE_EMBED_LIB_DIRECTORY=''
    # may be overridden by configuration file or check_after_user_configuration:
    USE_EMBED_HEADER_DIRECTORY=''
    #USE_EMBED_LIBS="C756/Xos20l.lib Module/Xma20l.lib"
    #USE_EMBED_BIOS_SRC="Xos20go.asm Xos20err.c xos20esx.h XOS20EEC.H XOS20EEC.OBJ"
    #USE_EMBED_ILO="Xos20lcs.ilo"
    USE_EMBED_COMPILER_DIR="c:/programme/tasking/c166"
    # if spec file defines this, the setting is simply overwritten
    APP_SEARCH_SRC_CONDITION="*.C *.cc *.c *.cpp *.cxx"
    APP_SEARCH_HDR_CONDITION="*.h *.hpp"

    APP_PATH=""
    ISO_AG_LIB_PATH=""
    APP_SRC_FILE=""
    REL_APP_PATH=""
    APP_PATH_EXCLUDE=""
    APP_SRC_EXCLUDE=""
    ISOAGLIB_INSTALL_PATH="/usr/local"
    GENERATE_FILES_ROOT_DIR="$PWD"

    OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED=0
    # set default USE_LITTLE_ENDIAN_CPU, so that quick number variable
    # to CAN string conversions are possible:
    USE_LITTLE_ENDIAN_CPU=1
    # set default USE_BIG_ENDIAN_CPU, so that quick number variable to
    # CAN string conversions are possible:
    USE_BIG_ENDIAN_CPU=0
    USE_VT_UNICODE_SUPPORT=0
    CAN_INSTANCE_CNT=1
    PRT_INSTANCE_CNT=1
    RS232_CHANNEL_CNT=1
    RS232_INSTANCE_CNT=1
    APP_NAME=''
    PRJ_PROPRIETARY_PGN_INTERFACE=0
    PRJ_ISO11783=0
    PRJ_ISO_VIRTUALTERMINAL_CLIENT=0
    PRJ_ISO_FILESERVER_CLIENT=0
    PRJ_ISO_TASKCONTROLLER_CLIENT=0
    PRJ_RS232_OVER_CAN=0
    PRJ_MULTIPACKET_STREAM_CHUNK=1
    PRJ_BASE=0
    PRJ_TRACTOR_GENERAL=0
    PRJ_TRACTOR_MOVE=0
    PRJ_TRACTOR_PTO=0
    PRJ_TIME_GPS=0
    PRJ_TRACTOR_LIGHT=0
    PRJ_TRACTOR_FACILITIES=0
    PRJ_TRACTOR_AUX=0
    PRJ_TRACTOR_GUIDANCE=0
    PRJ_TRACTOR_MOVE_SETPOINT=0
    PRJ_TRACTOR_PTO_SETPOINT=0
    PRJ_ISB_CLIENT=0
    PRJ_EEPROM=0
    PRJ_DATASTREAMS=0
    PRJ_OUTPUTS=0
    PRJ_INPUTS_DIGITAL=0
    PRJ_INPUTS_ANALOG=0
    PRJ_INPUTS_COUNTER=0
    PRJ_RS232=0
    # has to be overridden by configuration:
    USE_TARGET_SYSTEM='void'
    USE_HAL_PATH=''
    USE_ISO_TERMINAL_GRAPHICCONTEXT=0
    USE_ISO_TERMINAL_GETATTRIBUTES=0
    USE_ISO_TERMINAL_ATTRIBUTES=0
    PRJ_ISO_TERMINAL_OBJECT_SELECTION1=''
    PRJ_ISO_TERMINAL_OBJECT_SELECTION2=''
    PRJ_ISO_TERMINAL_OBJECT_SELECTION3=''
    PRJ_ISO_TERMINAL_OBJECT_SELECTION4=''
    PRJ_DEFINES=''
    ISOAGLIB_SWITCHES=''
    PRJ_INCLUDE_PATH=''
    USE_EXTERNAL_INCLUDE_PATH=''
    USE_EXTERNAL_LIBRARIES=''
    USE_EXTERNAL_LIBRARY_PATH=''
    USE_RS232_DRIVER='none'
	USE_EEPROM_DRIVER='none'
	USE_INPUTS_DRIVER='none'
	USE_OUTPUTS_DRIVER='none'
    DEBUG_DEFINES=''
    NDEBUG_DEFINE="$(wrap NDEBUG)"
    COMBINED_DEFINES="$NDEBUG_DEFINE $DEBUG_DEFINES"
}

check_set_correct_variables()
{
    local CONF_DIR="$1"

    : ${ISO_AG_LIB_PATH:?"ERROR! Please specify the path to the root directory of IsoAgLib - i.e. where xgpl_src is located"}
    : ${PROJECT:?"ERROR! Please set the variable PROJECT to the build sub-directory and executable filename"}

    if [ ! -d "$ISO_AG_LIB_PATH" ]; then
        echo_ "ERROR! ISO_AG_LIB_PATH is not a directory"
    else
        if [ ! -d "$ISO_AG_LIB_PATH/library" ]; then
            echo_ "ERROR! No library found within ISO_AG_LIB ($ISO_AG_LIB_PATH) - maybe your directory is not updated?"
        fi
    fi

    if [ "$PRT_INSTANCE_CNT" -gt "$CAN_INSTANCE_CNT" ]; then
        echo_ "ERROR! The Amount of protocol instances must be smaller or equal to amount of CAN instances"
        echo_ "Current false setting: PRT_INSTANCE_CNT=$PRT_INSTANCE_CNT \t CAN_INSTANCE_CNT=$CAN_INSTANCE_CNT"
        exit 2
    elif [ "$PRT_INSTANCE_CNT" -lt 1 ]; then
        echo_ "ERROR! There must be at least one protocol instance"
        exit 2
    fi

    if [ "$PRJ_ISO11783" -lt 1 -a "$PRJ_ISO_VIRTUALTERMINAL_CLIENT" -gt 0 ]; then
        echo_ "ERROR! Can't utilize PRJ_ISO_VIRTUALTERMINAL_CLIENT as ISO11783 is not activated"
        echo_ "Set PRJ_ISO11783 to 1 if you want ISO 11783 virtual terminal-client support."
        exit 2
    fi

    if [ "$PRJ_ISO11783" -lt 1 -a "$PRJ_ISO_FILESERVER_CLIENT" -gt 0 ]; then
        echo_ "ERROR! Can't utilize PRJ_ISO_FILESERVER_CLIENT as ISO11783 is not activated"
        echo_ "Set PRJ_ISO11783 to 1 if you want ISO 11783 fileserver-client support."
        exit 2
    fi

    if [ "$PRJ_ISO11783" -lt 1 -a "$PRJ_ISB_CLIENT" -gt 0 ]; then
        echo_ "ERROR! Can't utilize PRJ_ISB_CLIENT as ISO11783 is not activated"
        echo_ "Set PRJ_ISO11783 to 1 if you want ISO 11783 ISB support."
        exit 2
    fi

    if [ "$PRJ_RS232" -lt 1 -a "$PRJ_RS232_OVER_CAN" -gt 0 ]; then
        echo_ "ERROR! Can't utilize PRJ_RS232_OVER_CAN as PRJ_RS232 isn't set"
        echo_ "Set PRJ_RS232 to 1 to enable RS232 support."
        exit 2
    fi

    if [ -z "$APP_PATH" ] ; then
        APP_PATH="$ISO_AG_LIB_PATH"
    fi
    APP_INSIDE="../../$APP_PATH"
    ISO_AG_LIB_INSIDE="../../$ISO_AG_LIB_PATH"

    if [ "$PRJ_BASE" -gt 0 ]; then
        # activate all base data sub-features, when PRJ_BASE is activated
        PRJ_TRACTOR_GENERAL=1
        PRJ_TRACTOR_MOVE=1
        PRJ_TRACTOR_PTO=1
        PRJ_TIME_GPS=1
        PRJ_TRACTOR_LIGHT=1
        PRJ_TRACTOR_FACILITIES=1
        PRJ_TRACTOR_AUX=1
        PRJ_TRACTOR_GUIDANCE=1
        PRJ_ISB_CLIENT=1
    fi

    if [ "$PRJ_TRACTOR_MOVE_SETPOINT" -ne 0 ]; then
        PRJ_TRACTOR_MOVE=1 # force basic trac move to compile in
    fi
    if [ "$PRJ_TRACTOR_PTO_SETPOINT" -gt 0 ]; then
        PRJ_TRACTOR_PTO=1 # force basic trac move to compile in
    fi

    # overwrite settings from config file with command line parameter settings
    if [ $PARAMETER_TARGET_SYSTEM != "UseConfigFile" ] ; then
        USE_TARGET_SYSTEM=$PARAMETER_TARGET_SYSTEM
    fi
    if [ $PARAMETER_CAN_DRIVER != "UseConfigFile" ] ; then
        USE_CAN_DRIVER=$PARAMETER_CAN_DRIVER
    fi
    if [ $PARAMETER_RS232_DRIVER != "UseConfigFile" ] ; then
        USE_RS232_DRIVER=$PARAMETER_RS232_DRIVER
    fi
    if [ $PARAMETER_EEPROM_DRIVER != "UseConfigFile" ] ; then
        USE_EEPROM_DRIVER=$PARAMETER_EEPROM_DRIVER
    fi

    # workaround as long as we don't have separate HALs
    case "$USE_TARGET_SYSTEM" in
      (pc_win32|pc_linux)
        HAL_FIND_PATH="$ISO_AG_LIB_PATH/library/xgpl_src"
        HAL_PREFIX_ISOAGLIB="IsoAgLib/hal/pc"
        HAL_PREFIX_SUPPLEMENTARY="supplementary_driver/hal/pc"
        ;;
       (c2c|esx|esxu|p1mc|src9)
        HAL_FIND_PATH="$ISO_AG_LIB_PATH/library/xgpl_src"
        HAL_PREFIX_ISOAGLIB="IsoAgLib/hal/$USE_TARGET_SYSTEM"
        HAL_PREFIX_SUPPLEMENTARY="supplementary_driver/hal/$USE_TARGET_SYSTEM"
        ;;
      (*) #All other one are proprietary and need the USE_HAL_PATH set!
        HAL_FIND_PATH="$USE_HAL_PATH"
        HAL_PREFIX_ISOAGLIB="$USE_TARGET_SYSTEM"
        HAL_PREFIX_SUPPLEMENTARY="$USE_TARGET_SYSTEM"
        ;;
    esac

    # set default values for the default target platforms (HAL)
    # the N/A ones will be re-set in the case afterwards
    HAL_PATH_ISOAGLIB="$HAL_PREFIX_ISOAGLIB"
    HAL_PATH_ISOAGLIB_SYSTEM="$HAL_PREFIX_ISOAGLIB/system"
    HAL_PATH_ISOAGLIB_CAN="$HAL_PREFIX_ISOAGLIB/can"
    HAL_PATH_SUPPLEMENTARY_EEPROM="$HAL_PREFIX_SUPPLEMENTARY/eeprom"
    HAL_PATH_SUPPLEMENTARY_RS232="$HAL_PREFIX_SUPPLEMENTARY/rs232"
    HAL_PATH_SUPPLEMENTARY_OUTPUTS="$HAL_PREFIX_SUPPLEMENTARY/outputs"
    HAL_PATH_SUPPLEMENTARY_INPUTS="$HAL_PREFIX_SUPPLEMENTARY/inputs"
    HAL_PATH_SUPPLEMENTARY_DATASTREAMS="$HAL_PREFIX_SUPPLEMENTARY/datastreams"

    case "$USE_TARGET_SYSTEM" in
        (pc_win32|pc_linux|ees)
            GENERATE_FILES_ROOT_DIR="$CONF_DIR/cmake"
            IDE_NAME="CMake"
            ;;
        (esx|esxu|c2c)
            GENERATE_FILES_ROOT_DIR="$CONF_DIR/EDE"
            IDE_NAME="Tasking EDE"
            ;;
        (*)
            GENERATE_FILES_ROOT_DIR="$CONF_DIR/proprietary_hal"
            IDE_NAME="Proprietary - Filelist only"
            ;;
    esac

    # Force mandatory modules
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_ISOAGLIB" ]; then echo_ "Invalid proprietary HAL path: $HAL_FIND_PATH/$HAL_PATH_ISOAGLIB"; exit 2; fi
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_ISOAGLIB_SYSTEM" ]; then echo_ "Proprietary HAL is missing the 'system' directory: $HAL_FIND_PATH/$HAL_PATH_ISOAGLIB_SYSTEM"; exit 2; fi
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_ISOAGLIB_CAN" ]; then echo_ "Proprietary HAL is missing the 'can' directory: $HAL_FIND_PATH/$HAL_PATH_ISOAGLIB_CAB"; exit 2; fi
    # Disable N/A supplementary modules
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_SUPPLEMENTARY_EEPROM" ]; then HAL_PATH_SUPPLEMENTARY_EEPROM=""; fi
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_SUPPLEMENTARY_RS232" ]; then HAL_PATH_SUPPLEMENTARY_RS232=""; fi
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_SUPPLEMENTARY_OUTPUTS" ]; then HAL_PATH_SUPPLEMENTARY_OUTPUTS=""; fi
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_SUPPLEMENTARY_INPUTS" ]; then HAL_PATH_SUPPLEMENTARY_INPUTS=""; fi
    if [ ! -d "$HAL_FIND_PATH/$HAL_PATH_SUPPLEMENTARY_DATASTREAMS" ]; then HAL_PATH_SUPPLEMENTARY_DATASTREAMS=""; fi

    # handle virtual drivers
    if [ "$PRJ_RS232_OVER_CAN" -gt 0 ]; then
        HAL_PATH_SUPPLEMENTARY_RS232="supplementary_driver/hal/virtualDrivers/rs232_over_can"
    fi

    case "$USE_CAN_DRIVER" in
        (simulating|msq_server|socket_server|socket_server_hal_simulator|sys)
            ;;
        (*)
            echo_ 'ERROR! Please set the config variable "USE_CAN_DRIVER" to one of "simulating"|"sys"|"msq_server"|"socket_server"|"socket_server_hal_simulator"'
            echo_ 'Current Setting is $USE_CAN_DRIVER'
            exit 3
            ;;
    esac
}

# Take the first argument as variable name and append the remaining
# arguments to the variable's value.
append()
{
    local VARIABLE="$1"
    local FUNCTION="append_$VARIABLE"
    type "$FUNCTION" >/dev/null 2>&1 ||
        eval "$FUNCTION() { $VARIABLE=\"\${$VARIABLE:-}\$*\"; }"
    shift
    "$FUNCTION" "$@"
}

# Write to FD3 what's needed for finding COMM features.
comm_features()
{
    printf '%s' " -name 'isoaglib_config.h'" >&3
    if [ "$PRJ_BASE" -gt 0 ]; then
        printf '%s' " -o -path '*/Part7_ApplicationLayer/*'" >&3
    else
        if expr "$PRJ_TRACTOR_GENERAL" \| "$PRJ_TRACTOR_MOVE" \| "$PRJ_TRACTOR_FACILITIES" \| "$PRJ_TRACTOR_PTO" \| "$PRJ_TRACTOR_LIGHT" \| "$PRJ_TRACTOR_AUX" \| "$PRJ_TIME_GPS" \| "$PRJ_TRACTOR_GUIDANCE" >/dev/null; then
            printf '%s' " -o -name 'ibasetypes.h' -o -name 'basecommon_c*'" >&3
        fi
        if [ "$PRJ_TRACTOR_GENERAL" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracgeneral_c*' \)" >&3
        fi
        if [ "$PRJ_TRACTOR_MOVE" -gt 0 -a "$PRJ_TRACTOR_MOVE_SETPOINT" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracmove*' \)" >&3
        elif [ "$PRJ_TRACTOR_MOVE" -gt 0 -a "$PRJ_TRACTOR_MOVE_SETPOINT" -lt 1 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracmove_c.*' \)" >&3
        fi
        if [ "$PRJ_TRACTOR_PTO" -gt 0 -a "$PRJ_TRACTOR_PTO_SETPOINT" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracpto*' \)" >&3
        elif [ "$PRJ_TRACTOR_PTO" -gt 0 -a "$PRJ_TRACTOR_PTO_SETPOINT" -lt 1 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracpto_c.*' \)" >&3
        fi
        if [ "$PRJ_TRACTOR_LIGHT" -gt 0 -a "$PRJ_ISO11783" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*traclight*' \)" >&3
        fi
        if [ "$PRJ_TRACTOR_FACILITIES" -gt 0 -a "$PRJ_ISO11783" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracfacilities_c*' \)" >&3
        fi
        if [ "$PRJ_TRACTOR_AUX" -gt 0 -a "$PRJ_ISO11783" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracaux*' \)" >&3
        fi
        if [ "$PRJ_TRACTOR_GUIDANCE" -gt 0 -a "$PRJ_ISO11783" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*tracguidance*' \)" >&3
        fi
        if [ "$PRJ_TIME_GPS" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*timeposgps*' \)" >&3
        fi
        if [ "$PRJ_ISB_CLIENT" -gt 0 ]; then
            printf '%s' " -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*isbclient*' \) -o \( -path '*/Part7_ApplicationLayer/*' -a -name '*isbstatehandler*' \)" >&3
        fi
    fi

    if [ "$PRJ_PROPRIETARY_PGN_INTERFACE" -gt 0 ]; then
        printf '%s' " -o -path '*/Part3_ProprietaryMessages/*'" >&3
    fi

    if [ "$PRJ_ISO_FILESERVER_CLIENT" -gt 0 ]  ; then
        printf '%s' " -o -path '*/Part13_FileServer_Client/*'" >&3
    fi

    if [ "$PRJ_ISO_TASKCONTROLLER_CLIENT" -gt 0 ]; then
        printf '%s' " -o -path '*/Part10_TaskController_Client/*'" >&3
    fi
    
    if [ "$PRJ_ISO_VIRTUALTERMINAL_CLIENT" -gt 0 ]; then
        if [ "$USE_ISO_TERMINAL_GRAPHICCONTEXT" -eq 0 ]; then
            # exclude graphicscontext_c
            printf '%s' " -o \( -path '*/Part6_VirtualTerminal_Client/i*' -a -not -name '*graphicscontext_c*' \)" >&3
        else
            printf '%s' " -o -path '*/Part6_VirtualTerminal_Client/i*'" >&3
        fi
    fi
    if [ "$PRJ_DATASTREAMS" -lt 1 ]; then
        if [ "$PRJ_ISO_VIRTUALTERMINAL_CLIENT" -gt 0 -o "$PRJ_TIME_GPS" -gt 0 ]; then
            printf '%s' " -o -path '*/driver/datastreams/volatilememory_c.*'" >&3
        fi
    fi
    if [ "$PRJ_ISO11783" -gt 0 ]; then
        printf '%s' " -o -path '*/i*isobus_c.*' -o -path '*i*proprietarybus_c.*' -o -path '*/Part3_DataLink/i*multi*' -o -path '*/Part3_DataLink/impl/sendstream_c.*' -o -path '*/Part3_DataLink/impl/stream_c.*' -o -path '*/Part3_DataLink/istream_c.*' -o -path '*/supplementary_driver/driver/datastreams/streaminput_c.h'  -o -path '*/IsoAgLib/convert.h'" >&3
        if [ "$PRJ_MULTIPACKET_STREAM_CHUNK" -gt 0 ]; then
            printf '%s' " -o -path '*/Part3_DataLink/impl/streamchunk_c.*' -o -path '*/Part3_DataLink/impl/chunk_c.*'" >&3
        else
            printf '%s' " -o -path '*/Part3_DataLink/impl/streamlinear_c.*'" >&3
        fi
    fi
}

# Write to FD3 what's needed for finding DRIVER features.
driver_and_hal_features()
{
    printf '%s' \
      " -path '*/hal/hal_system.h' -o " \
      " -path '*/hal/hal_can.h' -o " \
      " -path '*/hal/hal_config.h' -o " \
	  " -path '*/hal/hal_errcodes.h' -o " \
      " -path '*/hal/hal_typedef.h' -o " \
      " -path '*/hal/generic_utils/*' -o " \
      " -path '*/driver/system*' " >&3
    # For now, simply copying all generic_utils/system files
    # as it doesn't matter as it's just about headers...

    printf '%s' \
      " -path '*${HAL_PATH_ISOAGLIB}/system*' -o " \
      " -path '*${HAL_PATH_ISOAGLIB}/errcodes.h' -o " \
      " -path '*${HAL_PATH_ISOAGLIB}/config.h' -o " \
      " -path '*${HAL_PATH_ISOAGLIB}/typedef.h' " >&4

    echo_ "CAN driver:    $USE_CAN_DRIVER"
    case "$USE_CAN_DRIVER" in
        (simulating)
            printf '%s' " -o -path '*${HAL_PATH_ISOAGLIB_CAN}/can_driver_simulating.*'" >&4
            ;;
        (msq_server)
            printf '%s' " -o -path '*${HAL_PATH_ISOAGLIB_CAN}/can_driver_canserver_msq.*' -o -path '*${HAL_PATH_ISOAGLIB}/can/can_server_interface_client.cpp' -o -path '*${HAL_PATH_ISOAGLIB}/can/can_server_interface.h'" >&4
            ;;
        (socket_server)
            printf '%s' " -o -path '*${HAL_PATH_ISOAGLIB_CAN}/can_driver_canserver_socket.*' -o -path '*${HAL_PATH_ISOAGLIB}/can/can_server_interface_client.cpp' -o -path '*${HAL_PATH_ISOAGLIB}/can/can_server_interface.h'" >&4
            ;;
        (socket_server_hal_simulator)
            printf '%s' " -o -path '*${HAL_PATH_ISOAGLIB_CAN}/can_driver_canserver_socket_hal_simulator.*'" >&4
            ;;
        (sys)
            printf '%s' " -o -path '*${HAL_PATH_ISOAGLIB_CAN}/can_driver_sys.*'" >&4
            ;;
    esac

    # add the standard driver directory sources for CAN
    printf '%s' " -o -path '*/driver/can/*'" >&3

    if [ "$PRJ_DATASTREAMS" -gt 0 ]; then
        printf '%s' " -o -path '*/driver/datastreams/*' -o -path '*/hal/hal_datastreams.h'" >&3
        printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_DATASTREAMS}/*'" >&4
    fi
    if [ "$PRJ_OUTPUTS" -gt 0 ]; then
        printf '%s' " -o -path '*/driver/outputs*' -o -path '*/hal/hal_outputs.h'" >&3
        printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_OUTPUTS}/outputs.h' -o -path '*${HAL_PATH_SUPPLEMENTARY_OUTPUTS}/outputs_target_extensions.h'" >&4
		echo_ "OUTPUTS driver:$USE_OUTPUTS_DRIVER"
        case "$USE_OUTPUTS_DRIVER" in
			(simulating)
				printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_OUTPUTS}/target_extension_outputs_simulating*'" >&4
				;;
			(hal_simulator)
				printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_OUTPUTS}/target_extension_outputs_hal_simulator*'" >&4
				;;
			(sys)
				printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_OUTPUTS}/target_extension_outputs_sys*'" >&4
				;;
			(*)
				echo_ 'ERROR! Please set the config variable "USE_OUTPUTS_DRIVER" to one of "simulating"|"sys"|"hal_simulator"'
				echo_ 'Current Setting is $USE_OUTPUTS_DRIVER'
				exit 3
		esac
    fi
    if [ "$PRJ_INPUTS_DIGITAL" -gt 0 ]; then
        printf '%s' " -o -name '*digitali_c.*'" >&3
    fi
    if [ "$PRJ_INPUTS_ANALOG" -gt 0 ]; then
        printf '%s' " -o -name '*analogi*'" >&3
    fi
    if [ "$PRJ_INPUTS_COUNTER" -gt 0 ]; then
        printf '%s' " -o -name '*counteri*'" >&3
    fi
    if [ "$PRJ_INPUTS_DIGITAL" -gt 0 -o "$PRJ_INPUTS_ANALOG" -gt 0 -o "$PRJ_INPUTS_COUNTER" -gt 0 ]; then
        printf '%s' " -o -name '*inputbase_c.*' -o -name '*iinput_c.*' -o -name '*inputs_c.*' -o -path '*/hal/hal_inputs.h'" >&3
        printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_INPUTS}/inputs.h' -o -path '*${HAL_PATH_SUPPLEMENTARY_INPUTS}/inputs_target_extensions.h'" >&4
		echo_ "INPUTS driver: $USE_INPUTS_DRIVER"
        case "$USE_INPUTS_DRIVER" in
			(simulating)
				printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_INPUTS}/target_extension_inputs_simulating*'" >&4
				;;
			(hal_simulator)
				printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_INPUTS}/target_extension_inputs_hal_simulator*'" >&4
				;;
			(sys)
				printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_INPUTS}/target_extension_inputs_sys*'" >&4
				;;
			(*)
				echo_ 'ERROR! Please set the config variable "USE_INPUTS_DRIVER" to one of "simulating"|"sys"|"hal_simulator"'
				echo_ 'Current Setting is $USE_INPUTS_DRIVER'
				exit 3
		esac
    fi
    if [ "$PRJ_RS232" -gt 0 ]; then
        printf '%s' " -o -path '*/driver/rs232/*' -o -path '*/hal/hal_rs232.h'" >&3
        printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/rs232.h'" >&4
        if [ "$PRJ_RS232_OVER_CAN" -gt 0 ]; then
            echo_ "RS232 driver:  rs232_over_can"
            printf '%s' " -o -path '*/hal/virtualDrivers/rs232_over_can/*'" >&3
        else
            echo_ "RS232 driver:  $USE_RS232_DRIVER"
            case "$USE_RS232_DRIVER" in
                (simulating)
                    printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/target_extension_rs232_simulating*'" >&4
                    ;;
                (hal_simulator)
                    printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/target_extension_rs232_hal_simulator*'" >&4
                    ;;
                (rte)
                    printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/target_extension_rs232_rte*'" >&4
                    ;;
                (sys)
                    case "$USE_TARGET_SYSTEM" in
					    (pc_win32)
                            printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/target_extension_rs232_w32_sys*'" >&4
					        ;;
                        (pc_linux)
                            printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/target_extension_rs232_linux_sys*'" >&4
                            ;;
                        (*)
                            printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/target_extension_rs232_sys*'" >&4
                            ;;
                    esac
                    ;;
                (*)
                    echo_ 'ERROR! Please set the config variable "USE_RS232_DRIVER" to one of "simulating"|"sys"|"rte"|"hal_simulator"'
                    echo_ 'Current Setting is $USE_RS232_DRIVER'
                    exit 3
            esac
            printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_RS232}/rs232_target_extensions.h'" >&4
        fi
    fi
    if [ "$PRJ_EEPROM" -gt 0 ]; then
        printf '%s' " -o -path '*/driver/eeprom/*' -o -path '*/hal/hal_eeprom.h'" >&3
        printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_EEPROM}/eeprom.h'" >&4
        echo_ "EEPROM driver: $USE_EEPROM_DRIVER"
        case "$USE_EEPROM_DRIVER" in
            (simulating)
                printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_EEPROM}/target_extension_eeprom_simulating*'" >&4
                ;;
            (hal_simulator)
                printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_EEPROM}/target_extension_eeprom_hal_simulator*'" >&4
                ;;
            (sys)
                printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_EEPROM}/target_extension_eeprom_sys*'" >&4
                ;;
            (*)
                echo_ 'ERROR! Please set the config variable "USE_EEPROM_DRIVER" to one of "sys"|"simulating"|"hal_simulator"'
                echo_ 'Current Setting is $USE_EEPROM_DRIVER'
                exit 3
        esac
        printf '%s' " -o -path '*${HAL_PATH_SUPPLEMENTARY_EEPROM}/eeprom_target_extensions.h'" >&4
    fi
}

# Split given string by delimiting whitespace and print parts with
# given format.
# Parameters: input string, first format, continuation format
split_and_printf()
{
    local INPUT="$1"
    local FORMAT="$2"
    local NEXT_FORMAT="$3"

    while [ -n "$INPUT" ]; do
        local N_MAXIMAL=$(expr length "$INPUT")
        local N_WHITESPACE=$(expr "$INPUT" : ' *')
        if [ 0 -lt "$N_WHITESPACE" ]; then
            INPUT="$(expr substr "$INPUT" $(($N_WHITESPACE + 1)) $N_MAXIMAL)"
        fi
        local N_NONWHITE=$(expr "$INPUT" : '[^ ]*')
        local PART=$(expr substr "$INPUT" 1 $N_NONWHITE)
        printf -- "$FORMAT" "$PART"
        FORMAT="$NEXT_FORMAT"
        if [ 0 -lt "$N_NONWHITE" ]; then
            INPUT="$(expr substr "$INPUT" $(($N_NONWHITE + 1)) $N_MAXIMAL || printf '')"
        fi
    done
}

# Calculate part of find command and write to FD3
find_part()
{
    local OUTER_OP="$1"
    local INNER_OP="$2"
    local INPUT="$3"
    if [ -n "$INPUT" ]; then
        printf '%s \( ' "$OUTER_OP" >&3
        split_and_printf "$INPUT" "$INNER_OP" " -or $INNER_OP" >&3
        printf ' \)' >&3
    fi
}


# this function uses the "find" cmd
# to derive based on the selected features the
# corresponding file list into filelist_$PROJECT_PURE.txt
create_filelist( )
{
    local GENERATE_FILES_ROOT_DIR="$1"
    local SCRIPT_DIR="$2"

    # Create directory to place generated files into...
    mkdir -p "$GENERATE_FILES_ROOT_DIR"
    cd "$GENERATE_FILES_ROOT_DIR"
    mkdir -p $PROJECT
    cd $PROJECT

    {
        local COMM_FEATURES="$(comm_features 3>&1 1>&9)"
        TMP_HAL_FEATURES="${TEMPFILE_PREFIX}hal_features"
        local DRIVER_FEATURES="$(driver_and_hal_features 3>&1 4>"$TMP_HAL_FEATURES" 1>&9)"
        local HAL_FEATURES="$(cat "$TMP_HAL_FEATURES")"
    } 9>&1

    local SRC_EXT="\( -name '*.c' -o -name '*.cc' -o -name '*.cpp' \)"
    local HDR_EXT="-name '*.h'"

    FILELIST_LIBRARY_HDR="filelist"'__'"${PROJECT}_hdr.library.txt"
    FILELIST_LIBRARY_PURE="filelist"'__'"${PROJECT}.library.txt"

    FILELIST_APP_HDR="filelist"'__'"${PROJECT}_hdr.app.txt"
    FILELIST_APP_PURE="filelist"'__'"${PROJECT}.app.txt"

    local FILELIST_COMBINED_HDR="filelist"'__'"${PROJECT}_hdr.txt"
    FILELIST_COMBINED_PURE="filelist"'__'"${PROJECT}.txt"

    local FIND_TEMP_PATH="$(printf '%s' \
            "-path '*/scheduler/*'" \
        " -o -path '*/util/*'" \
        " -o -path '*/Part5_NetworkManagement/*'" \
        " -o -path '*/Part12_DiagnosticsServices/*' " \
        " -o -path '*/Part3_DataLink/i*can*' " \
        "${COMM_FEATURES:+ -o ${COMM_FEATURES}}" \
        "${DRIVER_FEATURES:+ -o ${DRIVER_FEATURES}}")"
    { local EXCLUDE_PATH_PART1="$(find_part '-and -not' "-path '%s'" "$APP_PATH_EXCLUDE" 3>&1 1>&9)"; } 9>&1
    : ${EXCLUDE_PATH_PART1:="-a -not -path '*/xgpl_src/build/*'"}
    eval "find ../../$ISO_AG_LIB_PATH/library/xgpl_src -follow $SRC_EXT -a \( $FIND_TEMP_PATH \) $EXCLUDE_PATH_PART1 -printf '%h/%f\n' > $FILELIST_LIBRARY_PURE"
    eval "find ../../$ISO_AG_LIB_PATH/library/xgpl_src -follow $HDR_EXT -a \( $FIND_TEMP_PATH \) $EXCLUDE_PATH_PART1 -printf '%h/%f\n' > $FILELIST_LIBRARY_HDR"

    eval "find ../../$HAL_FIND_PATH/$HAL_PREFIX_ISOAGLIB -follow $SRC_EXT -a \( $HAL_FEATURES \) $EXCLUDE_PATH_PART1 -printf '%h/%f\n' >> $FILELIST_LIBRARY_PURE"
    eval "find ../../$HAL_FIND_PATH/$HAL_PREFIX_ISOAGLIB -follow $HDR_EXT -a \( $HAL_FEATURES \) $EXCLUDE_PATH_PART1 -printf '%h/%f\n' >> $FILELIST_LIBRARY_HDR"

    if [ "$HAL_PREFIX_ISOAGLIB" != "$HAL_PREFIX_SUPPLEMENTARY" ]; then
      eval "find ../../$HAL_FIND_PATH/$HAL_PREFIX_SUPPLEMENTARY -follow $SRC_EXT -a \( $HAL_FEATURES \) $EXCLUDE_PATH_PART1 -printf '%h/%f\n' >> $FILELIST_LIBRARY_PURE"
      eval "find ../../$HAL_FIND_PATH/$HAL_PREFIX_SUPPLEMENTARY -follow $HDR_EXT -a \( $HAL_FEATURES \) $EXCLUDE_PATH_PART1 -printf '%h/%f\n' >> $FILELIST_LIBRARY_HDR"
    fi
    
    ##############################
    # find application files
    ##############################
    {
        local APP_SRC_PART="$(find_part -and "-name '%s'" "$APP_SRC_FILE" 3>&1 1>&9)"
        local EXCLUDE_PATH_PART2="$(find_part '-and -not' "-path '%s'" "$APP_PATH_EXCLUDE" 3>&1 1>&9)"
        local EXCLUDE_SRC_PART="$(find_part '-and -not' "-name '%s'" "$APP_SRC_EXCLUDE" 3>&1 1>&9)"
        # remove the joker '*' from the file type spec, as this causes only trouble
        local APP_SEARCH_SRC_CONDITION1="$(printf '%s' "$APP_SEARCH_SRC_CONDITION" | tr -d '*')"
        local APP_SEARCH_SRC_TYPE_PART="$(find_part '' "-name '*%s'" "$APP_SEARCH_SRC_CONDITION1" 3>&1 1>&9)"
        local APP_SEARCH_HDR_TYPE_PART="$(find_part '' "-name '%s'" "$APP_SEARCH_HDR_CONDITION" 3>&1 1>&9)"
    } 9>&1
    
    for EACH_REL_APP_PATH in ${REL_APP_PATH:-}; do
        eval "find $APP_INSIDE/$EACH_REL_APP_PATH/ -follow $APP_SEARCH_SRC_TYPE_PART $APP_SRC_PART $EXCLUDE_PATH_PART2 $EXCLUDE_SRC_PART -printf '%h/%f\n' >&3"
        eval "find $APP_INSIDE/$EACH_REL_APP_PATH/ -follow $APP_SEARCH_HDR_TYPE_PART $APP_SRC_PART $EXCLUDE_PATH_PART2 $EXCLUDE_SRC_PART -printf '%h/%f\n' >&4"
    done 3>"$FILELIST_APP_PURE" 4>"$FILELIST_APP_HDR"


    cat $FILELIST_LIBRARY_PURE $FILELIST_APP_PURE > $FILELIST_COMBINED_PURE
    cat $FILELIST_LIBRARY_HDR $FILELIST_APP_HDR > $FILELIST_COMBINED_HDR

    cat $FILELIST_COMBINED_HDR >> $FILELIST_COMBINED_PURE
    rm -f $FILELIST_COMBINED_HDR

    # go back to directory where config file resides
    cd "$GENERATE_FILES_ROOT_DIR"

    return
}


# function to create the project specific autogenerated
# config header isoaglib_project_config.h
create_autogen_project_config()
{
    # go to project dir - below config dir
    cd $1/$PROJECT
    PLATFORM=$(uname)
    if [ $PLATFORM = "Linux" ] ; then
        ENDLINE="\n"
    else
        ENDLINE="\r\n"
    fi

    local FIRST_REL_APP_PATH
    read -r FIRST_REL_APP_PATH DUMMY <<END_OF_PATH
${REL_APP_PATH:-}
END_OF_PATH

    CONFIG_NAME="isoaglib_project_config.h"
    {
        echo_ "// IMPORTANT: Never change this header manually, it was generated by the call of \"conf2build.sh $CONF_FILES\" " >&3
        echo_e "$ENDLINE" >&3

        echo_ "#ifndef INCLUDED_FROM_ISOAGLIB_CONFIG" >&3
        echo_ "#error Do not include this file directly!" >&3
        echo_ "#endif" >&3

        echo_ "// These defines are needed for inclusion of the correct HAL modules:" >&3
        echo_ "#define HAL_PATH_ISOAGLIB $HAL_PATH_ISOAGLIB">&3
        echo_ "#define HAL_PATH_ISOAGLIB_SYSTEM $HAL_PATH_ISOAGLIB_SYSTEM">&3
        echo_ "#define HAL_PATH_ISOAGLIB_CAN $HAL_PATH_ISOAGLIB_CAN">&3
        if [ -n "$HAL_PATH_SUPPLEMENTARY_EEPROM" ]; then 
          echo_ "#define HAL_PATH_SUPPLEMENTARY_EEPROM $HAL_PATH_SUPPLEMENTARY_EEPROM">&3
        else
          echo_ "// keep HAL_PATH_SUPPLEMENTARY_EEPROM undefined as this module is not available for the given configuration/target" >&3
        fi
        if [ -n "$HAL_PATH_SUPPLEMENTARY_RS232" ]; then 
          echo_ "#define HAL_PATH_SUPPLEMENTARY_RS232 $HAL_PATH_SUPPLEMENTARY_RS232">&3
        else
          echo_ "// keep HAL_PATH_SUPPLEMENTARY_RS232 undefined as this module is not available for the given configuration/target" >&3
        fi
        if [ -n "$HAL_PATH_SUPPLEMENTARY_OUTPUTS" ]; then
          echo_ "#define HAL_PATH_SUPPLEMENTARY_OUTPUTS $HAL_PATH_SUPPLEMENTARY_OUTPUTS">&3
        else
          echo_ "// keep HAL_PATH_SUPPLEMENTARY_OUTPUTS undefined as this module is not available for the given configuration/target" >&3
        fi
        if [ -n "$HAL_PATH_SUPPLEMENTARY_INPUTS" ]; then
          echo_ "#define HAL_PATH_SUPPLEMENTARY_INPUTS $HAL_PATH_SUPPLEMENTARY_INPUTS">&3
        else
          echo_ "// keep HAL_PATH_SUPPLEMENTARY_INPUTS undefined as this module is not available for the given configuration/target" >&3
        fi
        if [ -n "$HAL_PATH_SUPPLEMENTARY_DATASTREAMS" ]; then
          echo_ "#define HAL_PATH_SUPPLEMENTARY_DATASTREAMS $HAL_PATH_SUPPLEMENTARY_DATASTREAMS">&3
        else
          echo_ "// keep HAL_PATH_SUPPLEMENTARY_DATASTREAMS undefined as this module is not available for the given configuration/target" >&3
        fi

        echo_e "$ENDLINE" >&3
        echo_ "// Switches" >&3
        for SinglePrjDefine in $ISOAGLIB_SWITCHES ; do
            SingleDefName="${SinglePrjDefine%%=*}"
            SingleDefRemainder="${SinglePrjDefine#$SingleDefName}"
            SingleDefValue="${SingleDefRemainder#=}"
            echo_ "#define $SingleDefName $SingleDefValue" >&3
        done

        echo_e "$ENDLINE" >&3
        echo_ "// Defining number of instances and channels:" >&3
        echo_e "#define CAN_INSTANCE_CNT $CAN_INSTANCE_CNT" >&3
        echo_e "#define PRT_INSTANCE_CNT $PRT_INSTANCE_CNT" >&3
        echo_e "#define RS232_CHANNEL_CNT $RS232_CHANNEL_CNT" >&3
        echo_e "#define RS232_INSTANCE_CNT $RS232_INSTANCE_CNT" >&3    
    
        case "$USE_CAN_DRIVER" in
            (msq_server)
                echo_e "#define CAN_DRIVER_MESSAGE_QUEUE" >&3
                ;;
            (socket_server|socket_server_hal_simulator)
                echo_e "#define CAN_DRIVER_SOCKET" >&3
                ;;
        esac
 
        echo_e "$ENDLINE" >&3

        if [ "$USE_LITTLE_ENDIAN_CPU" -gt 0 ] ; then
            echo_e "#define OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN" >&3
        fi
        if [ "$USE_BIG_ENDIAN_CPU" -gt 0 ] ; then
            echo_e "#define OPTIMIZE_NUMBER_CONVERSIONS_FOR_BIG_ENDIAN" >&3
        fi
    
        if [ "$OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED" -gt 0 ] ; then
            echo_e "#define OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED" >&3
        fi
    
		if [ "$PRJ_EEPROM" -gt 0 ] ; then
            echo_e "#define USE_EEPROM_IO" >&3
        fi
    
        if [ "$PRJ_DATASTREAMS" -gt 0 -o $PRJ_ISO_VIRTUALTERMINAL_CLIENT -gt 0 -o $PRJ_TIME_GPS -gt 0 ]; then
            echo_e "#define USE_DATASTREAMS_IO" >&3
        fi
    
        if [ "$PRJ_ISO11783" -gt 0 ] ; then
            echo_e "#define USE_ISO_11783" >&3
            if [ "$PRJ_BASE" -gt 0 ] ; then
                echo_e "#define USE_BASE" >&3
            fi
            if [ "$PRJ_TRACTOR_GENERAL" -gt 0 ]; then
                echo_e "#define USE_TRACTOR_GENERAL" >&3
            fi
            if [ "$PRJ_TRACTOR_MOVE" -gt 0 ]; then
                echo_e "#define USE_TRACTOR_MOVE" >&3
            fi
            if [ "$PRJ_TRACTOR_PTO" -gt 0 ]; then
                echo_e "#define USE_TRACTOR_PTO" >&3
            fi
            if [ "$PRJ_TRACTOR_LIGHT" -gt 0 ]; then
                echo_e "#define USE_TRACTOR_LIGHT" >&3
            fi
            if [ "$PRJ_TRACTOR_FACILITIES" -gt 0 ]; then
                echo_e "#define USE_TRACTOR_FACILITIES" >&3
            fi
            if [ "$PRJ_TRACTOR_AUX" -gt 0 ]; then
                echo_e "#define USE_TRACTOR_AUX" >&3
            fi
            if [ "$PRJ_TRACTOR_GUIDANCE" -gt 0 ]; then
                echo_e "#define USE_TRACTOR_GUIDANCE" >&3
            fi
            if [ "$PRJ_TIME_GPS" -gt 0 ]; then
                echo_e "#define USE_TIME_GPS" >&3
            fi
            if [ "$PRJ_ISB_CLIENT" -gt 0 ]; then
                echo_e "#define USE_ISB_CLIENT" >&3
            fi
            if [ "$PRJ_PROPRIETARY_PGN_INTERFACE" -gt 0 ]; then
                echo_e "#define USE_ISO_PROPRIETARY_PGN" >&3
            fi
            if [ "$PRJ_ISO_VIRTUALTERMINAL_CLIENT" -gt 0 ] ; then
                echo_e "#define USE_ISO_VIRTUALTERMINAL_CLIENT" >&3
                if [ "$USE_ISO_TERMINAL_ATTRIBUTES" -ne 0 ] ; then
                    echo_e "#define USE_ISO_TERMINAL_ATTRIBUTES" >&3
                fi
                if [ "$USE_ISO_TERMINAL_GRAPHICCONTEXT" -ne 0 ] ; then
                    echo_e "#define USE_ISO_TERMINAL_GRAPHICCONTEXT" >&3
                fi
                if [ -n "$PRJ_ISO_TERMINAL_OBJECT_SELECTION1" ] ; then
                    echo_e "#define PRJ_ISO_TERMINAL_OBJECT_SELECTION1 $PRJ_ISO_TERMINAL_OBJECT_SELECTION1" >&3
                fi
                if [ -n "$PRJ_ISO_TERMINAL_OBJECT_SELECTION2" ] ; then
                    echo_e "#define PRJ_ISO_TERMINAL_OBJECT_SELECTION2 $PRJ_ISO_TERMINAL_OBJECT_SELECTION2" >&3
                fi
                if [ -n "$PRJ_ISO_TERMINAL_OBJECT_SELECTION3" ] ; then
                    echo_e "#define PRJ_ISO_TERMINAL_OBJECT_SELECTION3 $PRJ_ISO_TERMINAL_OBJECT_SELECTION3" >&3
                fi
                if [ -n "$PRJ_ISO_TERMINAL_OBJECT_SELECTION4" ] ; then
                    echo_e "#define PRJ_ISO_TERMINAL_OBJECT_SELECTION4 $PRJ_ISO_TERMINAL_OBJECT_SELECTION4" >&3
                fi
            fi
			if [ "$PRJ_ISO_TASKCONTROLLER_CLIENT" -gt 0 ] ; then
				echo_e "#define USE_ISO_TASKCONTROLLER_CLIENT" >&3
			fi
            if [ "$PRJ_ISO_FILESERVER_CLIENT" -gt 0 ] ; then
                echo_e "#define USE_ISO_FILESERVER_CLIENT" >&3
            fi
            if [ "$PRJ_MULTIPACKET_STREAM_CHUNK" -gt 0 ] ; then
                echo_e "#define DEF_Stream_h_IMPL streamchunk_c.h" >&3
                echo_e "#define DEF_Stream_c_IMPL StreamChunk_c" >&3
            else
                echo_e "#define DEF_Stream_h_IMPL streamlinear_c.h" >&3
                echo_e "#define DEF_Stream_c_IMPL StreamLinear_c" >&3
            fi
        fi

        if [ "$PRJ_RS232" -gt 0 ]; then
            echo_e "#define USE_RS232 1" >&3
        fi
 
        if [ "$PRJ_OUTPUTS" -gt 0 ]; then
            echo_e "#define USE_OUTPUTS" >&3
        fi
 
        if [ "$PRJ_INPUTS_DIGITAL" -gt 0 -o "$PRJ_INPUTS_ANALOG" -gt 0 -o "$PRJ_INPUTS_COUNTER" -gt 0 ]; then
            echo_e "#define USE_INPUTS" >&3
        fi
    } 3>"$CONFIG_NAME"

    # go back to directory where config file resides
    cd $1
}

expand_definition()
{
    # Outputting the here document (cat <<) is superior to other
    # means especially when RULE contains quotes:
    eval "cat <<END_OF_STRING
$RULE
END_OF_STRING
" || {
        printf 'ERROR when evaluating rule (%s)\n' "$RULE" >&2
        exit 1
    }
}

expand_insert()
{
    printf '$(%s)' "$1"
}

define_insert()
{
    local NAME="$1"
    RULE="$2"
    local F=expand_insert # first expand for insertion
    eval "INSERT_${NAME}=\"\$(expand_definition)\"" || {
        printf 'ERROR when trying to set variable %s\n' "INSERT_${NAME}" >&2
        exit 1
    }
}

list_source_files()
{
    local FIRST_FORMAT="$1"
    local NEXT_FORMAT="$2" 
    local SOURCE_PATTERN="$3"
    shift 3
    local FORMAT="$FIRST_FORMAT"
    cat "$@" | grep -E "$SOURCE_PATTERN" | 
    while read -r CC_FILE; do
        printf "$FORMAT" "$CC_FILE"
        FORMAT="$NEXT_FORMAT"
    done
}

omit_or_printf()
{
    [ 1 -lt $# ] || return
    FORMAT="$1"
    shift
    printf -- "$FORMAT" "$@"
}

print_cmake_definitions()
{
    omit_or_printf '\n  -D%s' $(
        echo_ $PRJ_DEFINES
        # NDEBUG and/or DEBUG will be defined later according to CMake
        # build type, so omit them here:
        sed 's| \?\<N\?DEBUG\>||g' <<EOF
$COMBINED_DEFINES
EOF
    )
}


create_cmake_winlin()
{
    CompletePrjFilelist="$1/$PROJECT/$FILELIST_COMBINED_PURE"

    IsoAglibPrjHeaderFilelist="$1/$PROJECT/$FILELIST_LIBRARY_HDR"
    IsoAglibPrjSourceFilelist="$1/$PROJECT/$FILELIST_LIBRARY_PURE"
    AppPrjHeaderFilelist="$1/$PROJECT/$FILELIST_APP_HDR"
    AppPrjSourceFilelist="$1/$PROJECT/$FILELIST_APP_PURE"    
        
    local RELATIVE_INC_PATHS="$(echo_ ${REL_APP_PATH:-} $PRJ_INCLUDE_PATH)"
    local ALL_INC_PATHS="$(echo_ ${RELATIVE_INC_PATHS:+$(printf -- "$(literal_format "$APP_INSIDE")/%s\n" $RELATIVE_INC_PATHS)})"

    local INSERT_CMAKE_PROJECT="$PROJECT"
    local INSERT_CMAKE_DEFINITIONS="$(print_cmake_definitions)"
    local INSERT_CMAKE_INCLUDE_DIRECTORIES="$(omit_or_printf '\n  %s' . $ISO_AG_LIB_INSIDE/library $ISO_AG_LIB_INSIDE/library/xgpl_src ${ALL_INC_PATHS:-} ${USE_EXTERNAL_INCLUDE_PATH:-} ${BIOS_INC:-})"
    local INSERT_CMAKE_LINK_DIRECTORIES="${USE_EXTERNAL_LIBRARY_PATH:-}"
    local INSERT_CMAKE_TARGET_LINK_LIBRARIES_LINUX="$(omit_or_printf '\n  %s' "$PROJECT" rt $USE_EXTERNAL_LIBRARIES)"
    local INSERT_CMAKE_TARGET_LINK_LIBRARIES_WIN32="$(omit_or_printf '\n  %s' "$PROJECT" odbc32 odbccp32 winmm ws2_32 $USE_EXTERNAL_LIBRARIES)"
    local INSERT_CMAKE_SOURCE_FILES="$(omit_or_printf '\n  %s' "$(cat filelist__$PROJECT.txt)" )"
    local INSERT_CMAKE_ADD_EXECUTABLE="$(
        omit_or_printf '\n  %s' "$PROJECT" $(
            grep -E '\.cc|\.cpp|\.c|\.h' <"$CompletePrjFilelist" || status_le1))"
    local INSERT_CMAKE_APP_SOURCE_GROUP="$(
        omit_or_printf '\n  %s' $(
            grep -E '\.cc|\.cpp|\.c|\.h' <"$AppPrjSourceFilelist" || status_le1))"
    local INSERT_CMAKE_APP_HEADER_GROUP="$(
        omit_or_printf '\n  %s' $(
            grep -E '\.cc|\.cpp|\.c|\.h' <"$AppPrjHeaderFilelist" || status_le1))"
    local INSERT_CMAKE_ISOAGLIB_SOURCE_GROUP="$(
        omit_or_printf '\n  %s' $(
            grep -E '\.cc|\.cpp|\.c|\.h' <"$IsoAglibPrjSourceFilelist" || status_le1))"
    local INSERT_CMAKE_ISOAGLIB_HEADER_GROUP="$(
        omit_or_printf '\n  %s' $(
            grep -E '\.cc|\.cpp|\.c|\.h' <"$IsoAglibPrjHeaderFilelist" || status_le1))"
    
    : ${CMAKE_SKELETON_FILE:=$DEV_PRJ_DIR/$ISO_AG_LIB_INSIDE/tools/project_generation/conf2build_CMakeLists.txt}
    expand_template "$CMAKE_SKELETON_FILE" >"CMakeLists.txt"
}


create_CMake()
{
    DEV_PRJ_DIR="$1/$PROJECT"
    mkdir -p $DEV_PRJ_DIR
    cd "$DEV_PRJ_DIR"

	create_cmake_winlin "$1"
}


win_paths_from_unix_paths()
{
    if [ 0 -lt $# ]; then
        printf '%s' "$*" | win_paths_from_unix_paths
    else
        sed -e 's|/|\\|g'
    fi
}

unix_lines_to_windows_lines()
{
    # New versions of SED understand \r directly, but older ones (e.g.
    # the current one of MSYS) don't. With printf it's OK for both:
    sed -e "$(printf 's|\r||g;s|$|\r|')"
}

# Expand the template file. Its path has to be passed as the single
# argument.
# Attention to the special characters \, $ and ` in the template file.
# Preceed them with \ to let them appear literally, i.e. write \\, \$
# and \`.
expand_template()
{
    TEMPLATE_FILE="$1"
    eval "cat <<END_OF_TEMPLATE
$(cat "$TEMPLATE_FILE")
END_OF_TEMPLATE
" || {
        printf 'ERROR when processing template file %s\n' "$TEMPLATE_FILE" >&2
        exit 1
    }
}

create_EdePrj()
{
    DEV_PRJ_DIR="$1/$PROJECT"
    # echo_ "Create Projekt file for EDE in $DEV_PRJ_DIR"
    mkdir -p $DEV_PRJ_DIR/Template.dir
    PROJECT_FILE_NAME="$PROJECT"'_'"$USE_TARGET_SYSTEM.pjt"
    EdePrjFilelist="$1/$PROJECT/$FILELIST_COMBINED_PURE"

    if [ $USE_EMBED_LIB_DIRECTORY = "library/commercial_BIOS/bios_esx" ] ; then
        ## adopt the BIOS file, if $USE_EMBED_LIB_DIRECTORY and
        ## and $USE_EMBED_HEADER_DIRECTORY reflect default value which doesn't match to defined target
        case "$USE_TARGET_SYSTEM" in
            (esx)
                ;;
            (pc*)
                ;;
            (*)
                USE_EMBED_LIB_DIRECTORY="library/commercial_BIOS/bios_$USE_TARGET_SYSTEM"
                USE_EMBED_HEADER_DIRECTORY="library/commercial_BIOS/bios_$USE_TARGET_SYSTEM"
                ;;
        esac
    fi

    # if no setting in the config file -> derive based on target
    : ${USE_EMBED_LIBS:=$(
            case "$USE_TARGET_SYSTEM" in
                (c2c) printf 'c2c10l.lib' ;;
                (esx) printf 'C756/Xos20l.lib Module/Xma20l.lib' ;;
                (esxu) printf 'Mos10l.lib' ;;
            esac;)}
    # if no setting in the config file -> derive based on target
    : ${USE_EMBED_BIOS_SRC:=$(
            case "$USE_TARGET_SYSTEM" in
                (c2c) printf 'c2c10cs.asm c2c10err.c  c2c10err.h c2c10osy.h' ;;
                (esx) printf 'Xos20go.asm Xos20err.c xos20esx.h XOS20EEC.H XOS20EEC.OBJ' ;;
                (esxu) printf 'MOS10ERR.C  MOS10ERR.H  MOS10GO.ASM MOS10OSY.H' ;;
            esac;)}

    # if no setting in the config file -> derive based on target
    : ${USE_EMBED_ILO:=$(
            case "$USE_TARGET_SYSTEM" in
                (c2c) printf 'c2c10osy.ilo' ;;
                (esx) printf 'Xos20lcs.ilo' ;;
                (esxu) printf 'MOS10L.ILO' ;;
            esac;)}

    path_for_ede()
    {
        echo_ "$1/$2" | sed -e 's|/[0-9a-zA-Z_+\-]+/\.\.||g' -e 's|\\[0-9a-zA-Z_+\-]+\\\.\.||g'
    }

    isoaglib_path_for_ede()
    {
        path_for_ede "$ISO_AG_LIB_INSIDE" "$1"
    }

    embedlib_path_for_ede()
    {
        path_for_ede "$USE_EMBED_LIB_DIRECTORY" "$1"
    }

    USE_APP_PATH="$(map join_semicolon isoaglib_path_for_ede ${REL_APP_PATH:-})"
    ##### debug: echo_ "USE_APP_PATH: $USE_APP_PATH"

    USE_EMBED_HEADER_DIRECTORY="$(isoaglib_path_for_ede "$USE_EMBED_HEADER_DIRECTORY")"
    USE_EMBED_LIB_DIRECTORY="$(isoaglib_path_for_ede "$USE_EMBED_LIB_DIRECTORY")"
    USE_EMBED_ILO="$(embedlib_path_for_ede "$USE_EMBED_ILO")"
    USE_DEFINES="$(join_comma __TSW_CPP_756__ $PRJ_DEFINES $COMBINED_DEFINES)"

    libline_for_ede() { printf -- ' -Wo %s' "$@"; }
    USE_TARGET_LIB_LINE="$(map libline_for_ede embedlib_path_for_ede $USE_EMBED_LIBS)"

    # avoid UNIX style directory seperator "/" as it can disturb Tasking during the link process ( during compile, everything runs fine with UNIX style - WMK seems to have problems with it durign link and hex gen )
    ISO_AG_LIB_PATH_WIN=$(win_paths_from_unix_paths "$ISO_AG_LIB_INSIDE")
    USE_EMBED_LIB_DIRECTORY=$(win_paths_from_unix_paths "$USE_EMBED_LIB_DIRECTORY")
    USE_EMBED_HEADER_DIRECTORY=$(win_paths_from_unix_paths "$USE_EMBED_HEADER_DIRECTORY")
    USE_TARGET_LIB_LINE=$(win_paths_from_unix_paths "$USE_TARGET_LIB_LINE")
    USE_EMBED_ILO=$(win_paths_from_unix_paths "$USE_EMBED_ILO")
    USE_APP_PATH=$(win_paths_from_unix_paths "$USE_APP_PATH")
    DEV_PRJ_DIR_WIN=$(win_paths_from_unix_paths "$DEV_PRJ_DIR")
    USE_DEFINES=$(win_paths_from_unix_paths "$USE_DEFINES")
    USE_EMBED_COMPILER_DIR=$(win_paths_from_unix_paths "$USE_EMBED_COMPILER_DIR")
 
    # remove some file lists, which are not used
    rm -f "$1/$PROJECT/$FILELIST_LIBRARY_PURE" "$1/$PROJECT/$FILELIST_APP_PURE"

    local INSERT_PROJECT="$PROJECT"
    local INSERT_TARGET_LIB_DIRECTORY="$USE_EMBED_LIB_DIRECTORY"
    local INSERT_ISO_AG_LIB_PATH="$ISO_AG_LIB_PATH_WIN"
    local INSERT_TARGET_HEADER_DIRECTORY="$USE_EMBED_HEADER_DIRECTORY"
    local INSERT_TARGET_LIB="$USE_TARGET_LIB_LINE"
    local INSERT_TARGET_ILO="$USE_EMBED_ILO"
    local INSERT_APP_PATH="$USE_APP_PATH"
    local INSERT_PRJ_PATH="$DEV_PRJ_DIR_WIN"
    local INSERT_DEFINES="$USE_DEFINES"
    local INSERT_EMBED_COMPILER_DIR="$USE_EMBED_COMPILER_DIR"
    # now set the target CPU
    local INSERT_TARGET_CPU=$(
        case "$USE_TARGET_SYSTEM" in
            (esxu) printf 'CpuF269' ;;
            (*) printf 'CpuC167CS' ;;
        esac;)

    with_ede_includes()
    {
        "$1" 'C:\C166\include.cpp' '.' "${INSERT_EMBED_COMPILER_DIR}\\include.cpp" "${INSERT_APP_PATH}" "${INSERT_ISO_AG_LIB_PATH}/library" "${INSERT_ISO_AG_LIB_PATH}/library/xgpl_src" "${INSERT_TARGET_HEADER_DIRECTORY}"
    }

    with_ede_libraries()
    {
        "$1" 'C:\C166\lib' "${INSERT_EMBED_COMPILER_DIR}\\lib" "${INSERT_TARGET_LIB_DIRECTORY}"
    }

    local INSERT_INCLUDES="$(with_ede_includes join_semicolon)"
    local INSERT_LIBRARIES="$(with_ede_libraries join_semicolon)"
    {
        # Build Tasking Project File by: a) first stub part; b) file list c) second stub part
        expand_template $DEV_PRJ_DIR/$ISO_AG_LIB_INSIDE/tools/project_generation/conf2build_EDE.part1.pjt >&3

        TMP_EDE="${TEMPFILE_PREFIX}ede"
        cp $EdePrjFilelist $TMP_EDE
        sed -e 's|/|\\|g' $TMP_EDE >$EdePrjFilelist
        cat $EdePrjFilelist >&3

        # insert specific BIOS/OS sources
        local FORMAT="$(literal_format "$USE_EMBED_LIB_DIRECTORY")/%s\n"
        printf -- "$FORMAT" $USE_EMBED_BIOS_SRC >&3
        expand_template $DEV_PRJ_DIR/$ISO_AG_LIB_INSIDE/tools/project_generation/conf2build_EDE.part2.pjt >&3
    } 3>"$DEV_PRJ_DIR/$PROJECT_FILE_NAME"
    cd $DEV_PRJ_DIR

    #echo_ "Converted UNIX to Windows Linebreak in $PROJECT_FILE_NAME"
    unix_lines_to_windows_lines <<EOF >"$PROJECT_FILE_NAME"
$(cat "$PROJECT_FILE_NAME")
EOF
}


create_buildfiles()
{
    local CONF_DIR="$1"
    local SCRIPT_DIR="$2"
    # verify and correct the variables
    check_set_correct_variables "$CONF_DIR"
    GENERATE_FILES_ROOT_DIR=$(echo_ "$GENERATE_FILES_ROOT_DIR" | sed -e 's|/[0-9a-zA-Z_+\-]+/\.\.||g' -e 's/\\[0-9a-zA-Z_+\-]+\\\.\.//g')
    # echo_ "Create project for $USE_TARGET_SYSTEM in $GENERATE_FILES_ROOT_DIR"


    echo_ "ISOAgLib's Project-Generator (conf2build) running..."
    echo_


    # now call the function create_filelist() which build
    # the file list based on the varibles defined above
    create_filelist "$GENERATE_FILES_ROOT_DIR" "$SCRIPT_DIR"


    # call function to create project specific config file
    create_autogen_project_config $GENERATE_FILES_ROOT_DIR "$SCRIPT_DIR"
    case "$USE_TARGET_SYSTEM" in
        # check if a win32 project file whould be created
        (ees|pc_linux|pc_win32)
            create_CMake $GENERATE_FILES_ROOT_DIR "$SCRIPT_DIR"
            ;;
        (esx|esxu|c2c)
            create_EdePrj $GENERATE_FILES_ROOT_DIR "$SCRIPT_DIR"
            ;;
        (*)
            ### PROPRIETARY HAL - to come...
            ;;
    esac

    # cleanup temporary files
    rm -f $GENERATE_FILES_ROOT_DIR/$PROJECT/directories__*.txt
    find $GENERATE_FILES_ROOT_DIR/$PROJECT -name "filelist__*.[alu]*.txt" -exec rm {} \;
    find $GENERATE_FILES_ROOT_DIR/$PROJECT -name "filelist__*-doc.txt" -exec rm {} \;
    #$GENERATE_FILES_ROOT_DIR/$PROJECT/filelist__*.txt

}





# Usage: usage
# Print the usage.
usage ()
{
    cat <<EOF
Usage: conf2build.sh [OPTIONS] project_config_file
Creates Filelist, Projectfile/Makefile and Configuration Settings for an IsoAgLib project.

-h, --help                        print this message and exit.
--IsoAgLib-root=DIR               use the given root directory instead of the entry in the selected configuration file.
--target-system=TARGET            produce the project definition files for the selected TARGET instead of the
                                  target which is specified in the configuration file
                                  --> ("pc_linux"|"pc_win32"|"esx"|"esxu"|"c2c")
--pc-can-driver=CAN_DRIVER        produce the project definition files for the selected CAN_DRIVER if the project shall run on PC
                                  --> ("simulating"|"sys"|"msq_server"|"socket_server"|"socket_server_hal_simulator")
--pc-rs232-driver=RS232_DRIVER    produce the project definition files for the selected RS232_DRIVER if the project shall run on PC
                                  --> ("simulating"|"sys"|"rte"|"hal_simulator").
--pc-eeprom-driver=EEPROM_DRIVER  produce the project definition files for the selected EEPROM_DRIVER if the project shall run on PC
                                  --> ("simulating"|"sys"|"hal_simulator").
--little-endian-cpu               select configuration for LITTLE ENDIAN CPU type
--big-endian-cpu                  select configuration for BIG ENDIAN CPU type
--with-cmake-skeleton=filename    define project specific CMakeLists skeleton file which is used for CMakeLists.txt
                                  generation (default: conf2build_CMakeLists.txt in the same directory as this script)
--debugdefgroup=GROUPNUMBER       Use group of debug defines, only use for autobuilds.
                                  GROUPNUMBER can be 0, 1 or 2;
                                  when 0: only NDEBUG is set (release build, default);
                                  when 1: NDEBUG and all DEBUG_... variants are set;
                                  when 2: only all DEBUG_... variants are set;
"conf2build.sh" parses the selected project configuration file and overwrites the default values for all contained settings.
It then collects the corresponding files which can then be imported to an individual IDE.
Additionally a project specific configuration header is created in the application source directory with the
name scheme "config_<project_name>.h". If the #define PRJ_USE_AUTOGEN_CONFIG is set to "config_<project_name>.h",
the central configuration header library/xgpl_src/IsoAgLib/isoaglib_config.h will include this header.

Report bugs to <m.wodok@osb-ag.de>.
EOF
}


abs_dir_of(){ cd "$(dirname "$1")" && pwd; }

assignments()
{
    local FORMAT="%s=$(literal_format $1)\n"
    shift
    printf -- "$FORMAT" "$@"
}

DEBUG_DEF_NAMES='DEBUG_CAN
DEBUG_CANSERVER
DEBUG_DIAGNOSTICPGN
DEBUG_EEPROM
DEBUG_FILESERVER
DEBUG_ISOMONITOR
DEBUG_MULTIRECEIVE
DEBUG_MULTISEND
DEBUG_NMEA
DEBUG_VTCOMM'

check_before_user_configuration()
{
    if [ $# -lt 1 ] ; then
        echo_ "ERROR! You must at least specify the configuration file for your project as parameter"
        echo_
        usage
        exit 1
    fi

    CONF_FILES=""
    
    # Check the arguments.
    for option in "$@"; do
        case "$option" in
            (-h | --help)
                usage
                exit 0 ;;
            ('--IsoAgLib-root='*)
                ISOAGLIB_ROOT=$(echo_ "$option" | sed 's/--IsoAgLib-root=//')
                if [ ! -f "$ISOAGLIB_ROOT/library/xgpl_src/IsoAgLib/isoaglib_config.h" ] ; then
                    echo_ "Directory $ISOAGLIB_ROOT doesn't contain valid IsoAgLib directory root. The file xgpl_src/IsoAgLib/isoaglib_config.h can't be found." 1>&2
                    usage
                    exit 1
                else
                    CALL_DIR=$PWD
                    cd $ISOAGLIB_ROOT
                    ISOAGLIB_ROOT=$PWD
                    cd $CALL_DIR
                fi ;;
            ('--target-system='*)
                PARAMETER_TARGET_SYSTEM=$(echo_ "$option" | sed 's/--target-system=//')
                ;;
            ('--pc-can-driver='*)
                PARAMETER_CAN_DRIVER=$(echo_ "$option" | sed 's/--pc-can-driver=//')
                ;;
            ('--pc-rs232-driver='*)
                PARAMETER_RS232_DRIVER=$(echo_ "$option" | sed 's/--pc-rs232-driver=//')
                ;;
            ('--pc-eeprom-driver='*)
                PARAMETER_EEPROM_DRIVER=$(echo_ "$option" | sed 's/--pc-eeprom-driver=//')
                ;;
            (--little-endian-cpu)
                PARAMETER_LITTLE_ENDIAN_CPU=1
                ;;
            (--big-endian-cpu)
                PARAMETER_LITTLE_ENDIAN_CPU=0
                USE_BIG_ENDIAN_CPU=1
                ;;
            ('--with-cmake-skeleton='*)
                RootDir=$PWD
                CMAKE_SKELETON_FILE=$RootDir/$(echo_ "$option" | sed 's/--with-cmake-skeleton=//')
                ;;
            ('--debugdefgroup=0')
                # Keep default for release build.
                ;;
            ('--debugdefgroup=1')
                DEBUG_DEFINES="$(assignments 1 $DEBUG_DEF_NAMES)"
                NDEBUG_DEFINE="$(wrap NDEBUG)"
                COMBINED_DEFINES="$NDEBUG_DEFINE $DEBUG_DEFINES"
                ;;
            ('--debugdefgroup=2')
                DEBUG_DEFINES="$(assignments 1 $DEBUG_DEF_NAMES)"
                NDEBUG_DEFINE=''
                COMBINED_DEFINES="$NDEBUG_DEFINE $DEBUG_DEFINES"
                ;;
            (-*)
                echo_ "Unrecognized option $option'" 1>&2
                usage
                exit 1
                ;;
            (*)
                CONF_FILES="$CONF_FILES ./$option"
                ;;
        esac
    done
    
    # check if configuration file exists
    for CONF_FILE in $CONF_FILES
    do
      if [ ! -f $CONF_FILE ] ; then
          echo_ "ERROR! configuration file $CONF_FILE does not exist!"
          exit 1
      elif [ ! -r $CONF_FILE ] ; then
          echo_ "ERROR! configuration file $CONF_FILE is not readable!"
          exit 1
      fi
    done
    
    SCRIPT_DIR="$(abs_dir_of "$0")"
    FIRSTFILE=`echo $CONF_FILES | sed s/\ .*//`
    CONF_DIR="$(abs_dir_of "$FIRSTFILE")"
    
    # echo_ "StartDir $START_DIR"
    # echo_ "ScriptDir $SCRIPT_DIR"
    # echo_ "ConfDir $CONF_DIR"
}

check_after_user_configuration()
{
    # perform some checks based on user input
    # check for correct target system setting
    if [ $PARAMETER_TARGET_SYSTEM != "UseConfigFile" ] ; then
        USE_TARGET_SYSTEM=$PARAMETER_TARGET_SYSTEM
    fi
    case "$USE_TARGET_SYSTEM" in
        (pc_linux | pc_win32 | esx | esxu | c2c | p1mc | src9)
            ;;
        (*)
            if [ -z $USE_HAL_PATH ] ; then 
                echo_ "No HAL Path (USE_HAL_PATH) specified for proprietary HAL $USE_TARGET_SYSTEM" 1>&2
                usage
                exit 1
            fi
            ;;
    esac
    
    # check for correct CAN driver - and automatically adapt to embedded targets
    if [ $PARAMETER_CAN_DRIVER != "UseConfigFile" ] ; then
        USE_CAN_DRIVER=$PARAMETER_CAN_DRIVER
    fi

    if [ $PARAMETER_EEPROM_DRIVER != "UseConfigFile" ] ; then
        USE_EEPROM_DRIVER=$PARAMETER_EEPROM_DRIVER
    fi

    if [ $PARAMETER_RS232_DRIVER != "UseConfigFile" ] ; then
        USE_RS232_DRIVER=$PARAMETER_RS232_DRIVER
    fi

    : ${USE_EMBED_LIB_DIRECTORY:="library/commercial_BIOS/bios_${USE_TARGET_SYSTEM}"}
    : ${USE_EMBED_HEADER_DIRECTORY:="library/commercial_BIOS/bios_${USE_TARGET_SYSTEM}"}

    # check for little/big endian setting
    if [ -n "${PARAMETER_LITTLE_ENDIAN_CPU:-}" ] ; then
        USE_LITTLE_ENDIAN_CPU=$PARAMETER_LITTLE_ENDIAN_CPU
    fi
}

report_summary()
{
    if [ "$USE_LITTLE_ENDIAN_CPU" -lt 1 ] ; then
        echo_  "Endianess:     Big Endian CPU"
    else
        echo_  "Endianess:     Little Endian CPU"
    fi
    echo_  "Target:        $IDE_NAME - (The settings below are already set up therefore)"
    echo_  "Defines:       $PRJ_DEFINES (set in the project-file)"
    echo_  "Switches:      $ISOAGLIB_SWITCHES (set in the generated isoaglib_project_config.h)"
    echo_n "Include Path:  "
    for EACH_REL_APP_PATH in ${REL_APP_PATH:-} ; do
        echo_n "$APP_INSIDE/$EACH_REL_APP_PATH ";
    done
    echo_
    echo_
    echo_ "Generation successful."
    if [ -n "${TMP_REPORTFILE:-}" ]; then
        cat "$TMP_REPORTFILE"
    fi
}

on_exit()
{
    local STATUS="$?"
    set +o errexit # otherwise Bash may override the exit status due to another error
    set -- "${TMP_REPORTFILE:-}" "${TMP_CONF:-}" "${TMP_CONFIG1:-}" "${TMP_MAKEFILE:-}" "${TMP_EDE:-}" "${TMP_INTERNAL_FILELIST:-}" "${TMP_INTERFACE_FILELIST:-}" "${TMP_HAL_FEATURES:-}"
    # omit empty filenames:
    for FILE in "$@"; do
        shift
        [ -n "$FILE" ] && set -- "$@" "$FILE"
    done
    # delete temporary files:
    [ $# -gt 0 ] && rm -f -- "$@"
    [ "$STATUS" -eq 0 ] || printf 'ERROR (exit status %s).\n' "$STATUS" >&2
}

trap on_exit 0
trap 'exit 255' HUP INT QUIT PIPE TERM

main "$@"
