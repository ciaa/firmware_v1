#!/usr/bin/perl

# Copyright 2008, 2009, Mariano Cerdeiro
#
# This file is part of FreeOSEK.
#
# FreeOSEK is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Linking FreeOSEK statically or dynamically with other modules is making a
# combined work based on FreeOSEK. Thus, the terms and conditions of the GNU
# General Public License cover the whole combination.
#
# In addition, as a special exception, the copyright holders of FreeOSEK give
# you permission to combine FreeOSEK program with free software programs or
# libraries that are released under the GNU LGPL and with independent modules
# that communicate with FreeOSEK solely through the FreeOSEK defined interface.
# You may copy and distribute such a system following the terms of the GNU GPL
# for FreeOSEK and the licenses of the other code concerned, provided that you
# include the source code of that other code when and as the GNU GPL requires
# distribution of source code.
#
# Note that people who make modified versions of FreeOSEK are not obligated to
# grant this special exception for their modified versions; it is their choice
# whether to do so. The GNU General Public License gives permission to release
# a modified version without this exception; this exception also makes it
# possible to release a modified version which carries forward this exception.
#
# FreeOSEK is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.
#
use Switch;
use File::Copy;
use Data::Dumper;

$errors = 0;
$warnings = 0;
$fatalerrors = 0;

sub htons
{
	$val = 0;
	$mul = 1;
	foreach (split(//,@_[0]))
	{
		$val += $mul * ord($_);
		$mul *= 256;
	}

	return $val;
}


sub getTestcaseName
{
	my $index = @_[0];
	my @testcasename;

	open TESTCASES, $TESTCASES or die "$TESTCASES can not be openned: $!";
	@testcasename = <TESTCASES>;
	close (TESTCASES);
	chomp(@testcasename[$index]);
	return @testcasename[$index];
}

sub GetTestCases
{
	open TC, "<@_[0]" or die "@_[0] can not be openned: $!";
	my $val;
	my @ret;
	read(TC, $val, 35, 0);
	close(TC);
	foreach (split(//,$val))
	{
		$v = ord($_);
		$tc = ( ( $v >> 0 ) & 3 );
		push(@ret, $tc);
		$tc = ( ( $v >> 2 ) & 3 );
		push(@ret, $tc);
		$tc = ( ( $v >> 4 ) & 3 );
		push(@ret, $tc);
		$tc = ( ( $v >> 6 ) & 3 );
		push(@ret, $tc);
	}

	return @ret;
}

sub GetTestSequences
{
	my $file = @_[0];
	my @tc = ();

	open TSF, "<" . $file;

	while (my $line = <TSF>)
	{
		chomp($line);
		if ($line ne "")
		{
			$tabcount = ($line =~ tr/\t//);
			if ($tabcount == 0)
			{
				push(@tc, $line);
			}
		}
	}

	close(TSF);

	return @tc;
}

sub GetTestSequencesConfigs
{
	my $file = @_[0];
	my $tc = @_[1];
	my @tcs = ();
	my $startcount = 0;

	open TSF, "<" . $file;

	while (my $line = <TSF>)
	{
		chomp($line);
		if ($line ne "")
		{
			$tabcount = ($line =~ tr/\t//);
			$line =~ s/\t+//;
			if ($tabcount == 0)
			{
				if ($tc eq $line)
				{
					#print "LINE: $line\n";
					$startcount = 1;
				}
				else
				{
					#print "LINE END: $line\n";
					$startcount = 0;
				}
			}
			if ( ($tabcount == 1) && ( $startcount == 1 ) )
			{
				#print "$line\n";
				push(@tcs, $line);
			}
		}
	}

	close(TSF);

	return @tcs;
}

sub GetTestSequencesCon
{
	my $file = @_[0];
	my $tc = @_[1];
	my $tcc = @_[2];
	my @ret = ();
	my $stc1 = 0;
	my $stc2 = 0;

	open TSF, "<" . $file;

	while (my $line = <TSF>)
	{
		chomp($line);
		if ($line ne "")
		{
			$tabcount = ($line =~ tr/\t//);
			$line =~ s/\t+//;
			if ($tabcount == 0)
			{
				if ($tc eq $line)
				{
					#print "LINE: $line\n";
					$stc1 = 1;
				}
				else
				{
					#print "LINE END: $line\n";
					$stc1 = 0;
				}
			}
			if ( ($tabcount == 1) && ( $stc1 == 1 ) )
			{
				if ($line eq $tcc)
				{
					$stc2 = 1;
				}
				else
				{
					$stc2 = 0;
				}
			}
			if ( ($tabcount == 2) && ( $stc1 == 1 ) && ( $stc2 == 1 ) )
			{
				#print "LINE YES: $line\n";
				push(@ret, $line);
			}
		}
	}

	close(TSF);

	return @ret;
}

sub searchandreplace
{
	$file = @_[0];
	$s = @_[1];
	$r = @_[2];

	`perl -pi -e 's/$s/$r/' $file`;

	close(OUT);
}

sub EvaluateResults
{
	my $status = "OK";
	my $failed = 0;
	my $failedtotal = 0;

	$tsseqfile = "out/dbg/SequenceCounter.bin";
	$tsseqfileok = "out/dbg/SequenceCounterOk.bin";
	open SC, "<$tsseqfile" or die "$tsseqfile can not be openned: $!";
	read(SC, $sc, 4, 0);
	close(SC);
	open SC, "<$tsseqfileok" or die "$tsseqfileok can not be openned: $!";
	read(SC, $scok, 4, 0);
	close(SC);

	`rm $tsseqfile`;
	`rm $tsseqfileok`;

	$scerror = htons($sc) >> 31;
	$sc = ( htons($sc) & 0x7fffffff );
	$scok = htons($scok);
	if ( ($sc == $scok) && ($scerror == 0) )
	{
		$sctc = "OK";
	}
	else
	{
		$failed = 1;
		$failedtotal = 1;
		$sctc = "FAILED";
		$status = "FAILED";
	}
	results("Test Result: $sctc - Sequence: $scerror-$sc - SequenceOk: $scok");

	$failed = 0;
	$failedcounter = 0;
	$tsfile = "out/dbg/TestResults.bin";
	$tsfileok = "out/dbg/TestResultsOk.bin";
	@ts = GetTestCases($tsfile);
	@tsok = GetTestCases($tsfileok);
	#print "@ts\n";
	#print "@tsok\n";
	`hexdump -C $tsfile`;
	`hexdump -C $tsfileok`;
	`rm $tsfile`;
	`rm $tsfileok`;


	for($loopi = 0; $loopi < @ts; $loopi++)
	{
		if(@ts[$loopi] != @tsok[$loopi])
		{
			$failed = 1;
			$failedtotal = 1;
			$failedcounter++;
			$status = "FAILED";
			$sctc = "FAILED";
			results("Test Result: $sctc - Test Case: " . getTestcaseName($loopi) . " - Result: " . @ts[$loopi] . " - ResultOk: " . @tsok[$loopi]);
		}
		elsif ( (@ts[$loopi] == @tsok[$loopi]) && (@ts[$loopi] == 3) )
		{
			$sctc = "OK";
			results("Test Result: $sctc - Test Case: " . getTestcaseName($loopi) . " - Result: " . @ts[$loopi] . " - ResultOk: " . @tsok[$loopi]);
		}
	}

	return $status;
}

sub readparam
{
	open CFG, "<@_[0]" or die "Config file @_[0] can not be openned: $!";
	while (my $line = <CFG>)
	{
		chomp($line);
		($var,$val) = split(/:/,$line);
		switch ($var)
		{
			case "GDB" { $GDB = $val; }
			case "BINDIR" { $BINDIR = $val; }
			case "ARCH" { $ARCH = $val; }
			case "CPUTYPE" { $CPUTYPE = $val; }
			case "CPU" { $CPU = $val; }
			case "DIR" { $DIR = $val; }
			case "LOG" { $logfile = $val; }
			case "LOGFULL" { $logfilefull = $val; }
			case "TESTS" { $TESTS = $val; }
			case "RES" { $RES = $val; }
			case "TESTCASES" { $TESTCASES = $val; }
			else { }
		}
	}

	close CFG;
}

sub results
{
	print RESFILE "@_[0]\n";
	info(@_[0]);
}

sub info
{
	print "INFO: " . @_[0] . "\n";
	logf("INFO: " . @_[0]);
}

sub warning
{
	print "WARNING: " . @_[0] . "\n";
	logf("WARNING: " . @_[0]);
	$warnings++;
}

sub error
{
	print "ERROR " . @_[0] . "\n";
	logf("INFO: " . @_[0]);
	$errors++;
}

sub halt
{
	print "FATAL ERROR: " . @_[0] . "\n";
	logf("FATAL ERROR: " . @_[0]);
	$errors;
	$fatalerrors++;
	finish();
}

sub finish
{
	info("Warnings: $warnings - Errors: $errors");
	if ( ($errors > 0) || ($fatalerrors > 0) )
	{
		exit(1);
	}
	exit(0);
}

sub logf
{
	($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime(time);
	printf LOGFILE "%4d-%02d-%02d %02d:%02d:%02d %s\n",$year+1900,$mon+1,$mday,$hour,$min,$sec,@_[0];
	printf LOGFILEFULL "%4d-%02d-%02d %02d:%02d:%02d %s\n",$year+1900,$mon+1,$mday,$hour,$min,$sec,@_[0];
}

sub logffull
{
	($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime(time);
	printf LOGFILEFULL "%4d-%02d-%02d %02d:%02d:%02d %s\n",$year+1900,$mon+1,$mday,$hour,$min,$sec,@_[0];
}


print "FreeOSEK Conformance Test Runner - Copyright 2008-2009, Mariano Cerdeiro - http://opensek.sf.net\n\n";
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

if ($#ARGV + 1 < 2)
{
	info("ctest.pl -f ctest.cfg [ctest_tm_01] [--debug SEQUENCE]");
}

$onlytc = $ARGV[2];

$cfgfile = $ARGV[1];

print "Configuration file: " . $cfgfile . "\n";

if ($ARGV[3] eq "--debug")
{
	$debug = 1;
	$subtestcase = $ARGV[4];
	print "Debugging modes for: " . $subtestcase . "\n";
}
else
{
	$debug = 0;
	$subtestcase = -1;
}

readparam($cfgfile);

open LOGFILE, "> $logfile" or die "can not open $logfile for append: $!";
open LOGFILEFULL, "> $logfilefull" or die "can not open $logfile for append: $!";
open RESFILE, "> $RES" or die "can not open $RES for append: $!";

info("Starting FreeOSEK Conformance Test Runner");

@tests = GetTestSequences($TESTS);

if($onlytc ne "")
{
	print "Running only one Test Configuration: " . $onlytc . "\n";
	@tmptests = @tests;
	@tests = ();

	foreach (@tmptests)
	{
		if(index($_,$onlytc)>-1)
		{
			push(@tests, $_);
		}
	}
}

foreach $testfn (@tests)
{
	@test = split(/:/,$testfn);
	$test = @test[0];

	info("Testing $test");

	@configs = GetTestSequencesConfigs($TESTS, $testfn);

	foreach $config (@configs)
	{
		$runthistestcase = 1;

		$testcasecount++;

		if($subtestcase>0)
		{
			if($subtestcase == $testcasecount)
			{
				$runthistestcase = 1;
			}
			else
			{
				$runthistestcase = 0;
			}
		}

		if ($runthistestcase)
		{
			print "Config: $config\n";

			$error = "";

			info("make clean of $test");
			$outmakeclean = `make clean`;
			$outmakecleanstatus = $?;
			info("make clean status: $outmakecleanstatus");
			logffull("make clean output:\n$outmakeclean");

			mkdir("out/gen/etc/");

			$org = "FreeOSEK/Os/tst/ctest/etc/" . $test . ".oil";
			$dst = "out/gen/etc/" . $test . ".oil";
			copy($org, $dst) or die "file can not be copied from $org to $dst: $!";

			@replace = GetTestSequencesCon($TESTS, $testfn, $config);
			foreach $rep (@replace)
			{
				info("Replacing: $rep");
				@rep = split (/:/,$rep);
				searchandreplace($dst,@rep[0],@rep[1]);
			}

			if ($outmakecleanstatus == 0)
			{
				info("make generate of $test");
            info("running \"make generate PROJECT=$test\"");
				$outmakegenerate = `make generate PROJECT=$test`;
				$outmakegeneratestatus = $?;
				info("make generate status: $outmakegeneratestatus");
				logffull("make generate output:\n$outmakegenerate");
            if ($debug)
            {
               print "$outmakegenerate";
            }
				if ($outmakegeneratestatus == 0)
				{
					info("make of $test");
					$outmake = `make PROJECT=$test`;
					$outmakestatus = $?;
					info("make status: $outmakestatus");
					logffull("make output:\n$outmake");
               if ($debug)
               {
                  print "$outmake";
               }
					if ($outmakestatus == 0)
					{
						$out = $BINDIR . "/" . $test;
						info("debug of $test");
						$dbgfile = "FreeOSEK/Os/tst/ctest/dbg/" . $ARCH . "/gcc/debug.scr";
						info("$GDB $out -x $dbgfile");
						`rm /dev/mqueue/*`;
						if($debug == 0)
						{
							#$outdbg = `$GDB $out -x $dbgfile`;
							system("$GDB $out -x $dbgfile");
						}
						else
						{
							exec("$GDB $out");
						}
						`rm /dev/mqueue/*`;
						`killall $test`;
						$outdbg = "";
						$outdbgstatus = $?;
						info("debug status: $outdbgstatus");
						logffull("debug output:\n$outdbg");
						$outdbgstatus = 0;
						if ($outdbgstatus == 0)
						{
							results("Test: $test - Config: $config");
							$status = EvaluateResults();
							results("Test: $test - Config: $config - Status: $status");
						}
					}
				}
				else
				{
					exit();
				}
			}
		}
	}
}

close(LOGFILE);
close(LOGFILEFULL);
close(RESFILE);


