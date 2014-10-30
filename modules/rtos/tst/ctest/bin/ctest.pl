#!/usr/bin/perl
# Copyright 2008, 2009, 2014 Mariano Cerdeiro
# Copyright 2014, Juan Cecconi
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

use feature "switch";
#use Switch;
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
      # removes carry return + line-feed
	  $line =~ tr/\r\n//d;
      if ($line ne "")
      {
         # Count TABs
         $tabcount = ($line =~ tr/\t//);
         # removes carry return + line-feed
         $line =~ s/\t+//;
         if ($tabcount == 0)
         {
            # Line without Tabs => Start of Test Secuence
            if ($tc eq $line)
            {
               #Start of searched "Test Sequence"
               #print("LINE: $line\n");
               $stc1 = 1;
            }
            else
            {
               #End of searched "Test Sequence"
               #print("LINE END: $line\n");
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

   $tsseqfile = "out/rtos/SequenceCounter.bin";
   $tsseqfileok = "out/rtos/SequenceCounterOk.bin";
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
   $tsfile = "out/rtos/TestResults.bin";
   $tsfileok = "out/rtos/TestResultsOk.bin";
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
      given ($var)
      {
         when ("GDB") { $GDB = $val; }
         when ("BINDIR") { $BINDIR = $val; }
         when ("ARCH") { $ARCH = $val; }
         when ("CPUTYPE") { $CPUTYPE = $val; }
         when ("CPU") { $CPU = $val; }
         when ("DIR") { $DIR = $val; }
         when ("DEBUG_CTESTS") { $debug = $val; }
         when ("LOG") { $logfile = $val; }
         when ("LOGFULL") { $logfilefull = $val; }
         when ("CLEAN_GENERATE") { $clean_generate = $val; }
         when ("TESTS") { $TESTS = $val; }
         when ("RES") { $RES = $val; }
         when ("TESTCASES") { $TESTCASES = $val; }
         default { }
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

#** \brief Creates a Project for a test
#
# \param[in] test test to be generated
# \param[in] config configuration of the test to be generated
#
#*
sub CreateTestProject
{
  my $test = shift;
  my $config = shift;
  my $base = "out/rtos/$test/$config";
  info("Creating Test: $test - Config: $config under $base/$test/$config");
  # create needed directories
  `mkdir -p $base/etc`;
  `mkdir -p $base/src`;
  `mkdir -p $base/mak`;
  `mkdir -p $base/inc`;
  `mkdir -p $base/inc/$ARCH`;
  # get configuration file for this project
  $org = "modules/rtos/tst/ctest/etc/" . $test . ".oil";
  $dst = "$base/etc/$test-$config.oil";
  copy($org, $dst) or die "file can not be copied from $org to $dst: $!";
  # prepare the configuration for this project
  # removes carry return + line-feed
  $testfn =~ tr/\r\n//d;
  @replace = GetTestSequencesCon($TESTS, $testfn, $config);
  foreach $rep (@replace)
  {
    info("Replacing: $rep");
    @rep = split (/:/,$rep);
    searchandreplace($dst,@rep[0],@rep[1]);
  }
  # create makefile for this project
  open FILE, "> $base/mak/Makefile" or die "can not open: $!";
  print FILE "project = $test-$config\n\n";
  print FILE "\$(project)_PATH = \$(ROOT_DIR)\$(DS)$base\n\n";
  print FILE "\$(project)_SRC_PATH += \$(\$(project)_PATH)\$(DS)src\$(DS) \\\n";
  print FILE " modules\$(DS)rtos\$(DS)tst\$(DS)ctest\$(DS)src\$(DS)\n\n";
  print FILE "INCLUDE += \$(\$(project)_PATH)\$(DS)inc \\\n";
  print FILE " \$(\$(project)_PATH)\$(DS)inc\$(DS)$ARCH\\\n";
  print FILE " modules/posix/inc\n";
  print FILE "SRC_FILES += \$(wildcard \$(\$(project)_PATH)\$(DS)src\$(DS)*.c) \\\n";
  print FILE " modules\$(DS)rtos\$(DS)tst\$(DS)ctest\$(DS)src\$(DS)ctest_rst.c\n\n";
  print FILE "OIL_FILES += \$(\$(project)_PATH)\$(DS)etc\$(DS)\$(project).oil\n\n";
  print FILE "MODS = modules\$(DS)bsp \\\n";
  print FILE " modules\$(DS)platforms \\\n";
  print FILE " modules\$(DS)libs \\\n";
  print FILE " modules\$(DS)posix \\\n";
  print FILE " modules\$(DS)config \\\n";
  print FILE " modules\$(DS)ciaak \\\n";
  print FILE " modules\$(DS)rtos\n\n";
  print FILE "rtos_GEN_FILES += modules\$(DS)rtos\$(DS)tst\$(DS)ctest\$(DS)gen\$(DS)inc\$(DS)ctest_cfg.h.php\n\n";
  print FILE "CFLAGS += -D$test\n";
  close FILE;
  #copy needed files
  copy("modules/rtos/tst/ctest/src/$test.c","$base/src/$test.c");
  copy("modules/rtos/tst/ctest/inc/$test.h","$base/inc/$test.h");
  copy("modules/rtos/tst/ctest/inc/ctest.h","$base/inc/ctest.h");
  copy("modules/rtos/tst/ctest/inc/$ARCH/ctest_arch.h","$base/inc/$ARCH/ctest_arch.h");
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


print "ciaaFirmware RTOS Generator - Copyright 2008, 2009, 2014 Mariano Cerdeiro\n";
print "                              Copyright 2014, Juan Cecconi\n";
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

if ($#ARGV + 1 < 2)
{
   info("ctest.pl -f ctest.cfg [ctest_xx_yy] [subtestcase]");
}

#Example: 'full-preemptive' or empty
$subtestcase = $ARGV[3];
#Example: 'ctest_tm_01:Test Sequence 1' or empty
$onlytc = $ARGV[2];

$cfgfile = $ARGV[1];

info("Configuration file: $cfgfile");
info("Removing old files");
system("rm -rf out/rtos/*");

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

if ($subtestcase ne "")
{
   print "Sub Test Case: " . $subtestcase . "\n";
}
if($debug != 0)
{
   print "Debug Mode: Enabled!\n";
}

foreach $testfn (@tests)
{
   @test = split(/:/,$testfn);
   $test = @test[0];

   info("Testing $test");

   @configs = GetTestSequencesConfigs($TESTS, $testfn);

   foreach $config (@configs)
   {
	  $config =~ tr/\r\n//d;
      $runthistestcase = 1;

      $testcasecount++;

      if($subtestcase)
      {
         if($subtestcase eq $config)
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
         info("Config: $config");

         CreateTestProject($test, $config);

         $error = "";
		 
         if($clean_generate != 0)
         {
            info("make clean of $test");
            $outmakeclean = `make clean`;
            $outmakecleanstatus = $?;
            info("make clean status: $outmakecleanstatus");
            logffull("make clean output:\n$outmakeclean");
         }
         else
         {
            info("WARNING: skipping make clean of $test");
			$outmakecleanstatus = 0;
         }

         mkdir("out/gen/etc/");

         if ($outmakecleanstatus == 0)
         {
            if($clean_generate != 0)
            {		 
               info("make generate of $test");
               info("running \"make generate PROJECT=out/rtos/$test/$config");
               $outmakegenerate = `make generate PROJECT=out/rtos/$test/$config`;
               $outmakegeneratestatus = $?;
               info("make generate status: $outmakegeneratestatus");
               logffull("make generate output:\n$outmakegenerate");
               if ($debug)
               {
                  print "$outmakegenerate";
               }
            }
            else
            {
               info("WARNING: skipping make generate of $test");
			   $outmakegeneratestatus = 0;
            }
            if ($outmakegeneratestatus == 0)
            {
               info("make of $test");
               $outmake = `make PROJECT=out/rtos/$test/$config`;
               $outmakestatus = $?;
               info("make status: $outmakestatus");
               logffull("make output:\n$outmake");
               if ($debug)
               {
                  print "$outmake";
               }
               if ($outmakestatus == 0)
               {
                  if ($ARCH eq "cortexM4")
                  {
                     $out = $BINDIR . "/" . $test . "-" . $config . ".axf";
                  }
                  else
                  {
                     $out = $BINDIR . "/" . $test . "-" . $config . ".exe";
                  }
                  info("debug of $test in $out");
                  $dbgfile = "modules/rtos/tst/ctest/dbg/" . $ARCH . "/gcc/debug.scr";
                  info("$GDB $out -x $dbgfile");
                  if($debug == 0)
                  {
                     #$outdbg = `$GDB $out -x $dbgfile`;
                     system("$GDB $out -x $dbgfile");
                  }
                  else
                  {
                     exec("$GDB $out");
                  }
                  `pkill ctest_`;
                  $outdbg = "";
                  $outdbgstatus = $?;
                  info("debug status: $outdbgstatus");
                  info("debug output:\n$outdbg");
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


