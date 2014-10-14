#!/usr/bin/perl
# Copyright 2008, 2009 Mariano Cerdeiro
# Copyright 2014, ACSE & CADIEEL
#      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
#      CADIEEL: http://www.cadieel.org.ar
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
#    and/or other materials provided with the distribution.
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
#
#

#use Switch;
use feature "switch";

sub info
{
   print "INFO: " . @_[0] . "\n";
}

print "ciaaFirmware RTOS Generator - Copyright 2008, 2009, Mariano Cerdeiro\n";
print "                              Copyright 2014, ACSE & CADIEEL\n";
print "         ACSE : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/\n";
print "         CADIEEL: http://www.cadieel.org.ar\n\n";
print "INFO: ------ LICENSE START ------\n";
print "INFO: This file is part of CIAA Firmware.\n";
print "INFO: Redistribution and use in source and binary forms, with or without\n";
print "INFO: modification, are permitted provided that the following conditions are met:\n";
print "INFO: 1. Redistributions of source code must retain the above copyright notice,\n";
print "INFO: this list of conditions and the following disclaimer.\n";
print "INFO: 2. Redistributions in binary form must reproduce the above copyright notice,\n";
print "INFO: this list of conditions and the following disclaimer in the documentation\n";
print "INFO: and/or other materials provided with the distribution.\n";
print "INFO: 3. Neither the name of the copyright holder nor the names of its\n";
print "INFO: contributors may be used to endorse or promote products derived from this\n";
print "INFO: software without specific prior written permission.\n";
print "INFO: THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n";
print "INFO: AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n";
print "INFO: IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n";
print "INFO: ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE\n";
print "INFO: LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n";
print "INFO: CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n";
print "INFO: SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n";
print "INFO: INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n";
print "INFO: CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n";
print "INFO: ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n";
print "INFO: POSSIBILITY OF SUCH DAMAGE.\n";
print "INFO: ------- LICENSE END -------\n";

$buffer = "";
@testresult = ();

open(FILE, "<out.bin");
binmode(FILE);
read(FILE, $buffer, 35, 0);
close(FILE);

foreach (split(//, $buffer)) {

	#printf("%02x ", ord($_));
	
	for($loopi = 0; $loopi < 4; $loopi++)
	{
		$test = ( ord($_) >> (2 * $loopi) ) & 0x3;
		#print "$loopi: $test\n";
		push(@testresult, $test);
	}
	#print"\n";

}

print "\n";

foreach (@testresult)
{
	switch ($_)
	{
		case 0 {	print "INIT"; }
		case 1 { print "FAILED"; } 
		case 2 { print "OTHER"; }
		case 3 { print "OK"; }
	}
	print " - ";
}

