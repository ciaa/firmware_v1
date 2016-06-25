# Copyright 2015, Carlos Pantelides
# All rights reserved.
#
# This file is part of CIAA Firmware.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and${DS}or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

testNoFileOutput() {
   local OUTPUT=$(php modules${DS}tools${DS}generator${DS}tests${DS}ftest${DS}stdoutWriterLauncher.php  -DARCH=x86 -DCPUTYPE=ia32 -DCPU=none \
      -c ${FIXTURES}${DS}blinking.oil \
      -t ${FIXTURES}${DS}gen${DS}inc${DS}Os_Internal_Cfg.h.php \
      -H modules${DS}rtos${DS}gen${DS}ginc${DS}Multicore.php \
      -b ${DS}gen${DS} \
      -o ${TMP})

   assertEquals "${TMP}${DS}" $(find ${TMP}${DS} -not -name .gitignore)
   rm -rf ${TMP}${DS}*
}


SHUNIT=$1
TESTS=$2
DS=$3
FIXTURES=$4
EXPECTED=$5
TMP=$6

shift $#

. ${SHUNIT}
