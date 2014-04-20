@rem This script expects the following parameters:
@rem %1 = Full path name of the .fzm file
@rem %2 = Full path name of an intermediate file (Perl-executable output, FZM-Parser input)
@rem
@rem Determine the full path to the executables.
@set SCT_TOOL_SCRIPT="%~dp0%fzm2script.exe"
@set SCT_TOOL_PARSER="%~dp0%fzmparser"
@rem
@%SCT_TOOL_SCRIPT% %1 -output "%2"
@%SCT_TOOL_PARSER% < "%2"