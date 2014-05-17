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

use Switch;

sub info
{
   print "INFO: " . @_[0] . "\n";
}

print "FreeOSEK Conformance Test Result - Copyright 2008-2009, Mariano Cerdeiro - http://opensek.sf.net\n\n";
info("------ LICENSE START ------");
info("FreeOSEK Conformance Test Result is part of FreeOSEK.");
info("");
info("FreeOSEK is free software: you can redistribute it and/or modify");
info("it under the terms of the GNU General Public License as published by");
info("the Free Software Foundation, either version 3 of the License, or");
info("(at your option) any later version.");
info("");
info("Linking FreeOSEK statically or dynamically with other modules is making a");
info("combined work based on FreeOSEK. Thus, the terms and conditions of the GNU");
info("General Public License cover the whole combination.");
info("");
info("In addition, as a special exception, the copyright holders of FreeOSEK give");
info("you permission to combine FreeOSEK program with free software programs or");
info("libraries that are released under the GNU LGPL and with independent modules");
info("that communicate with FreeOSEK solely through the FreeOSEK defined interface.");
info("You may copy and distribute such a system following the terms of the GNU GPL");
info("for FreeOSEK and the licenses of the other code concerned, provided that you");
info("include the source code of that other code when and as the GNU GPL requires");
info("distribution of source code.");
info("");
info("Note that people who make modified versions of FreeOSEK are not obligated to");
info("grant this special exception for their modified versions; it is their choice");
info("whether to do so. The GNU General Public License gives permission to release");
info("a modified version without this exception; this exception also makes it");
info("possible to release a modified version which carries forward this exception.");
info("");
info("FreeOSEK is distributed in the hope that it will be useful,");
info("but WITHOUT ANY WARRANTY; without even the implied warranty of");
info("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
info("GNU General Public License for more details.");
info("You should have received a copy of the GNU General Public License");
info("along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.");
info("------- LICENSE END -------");

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

