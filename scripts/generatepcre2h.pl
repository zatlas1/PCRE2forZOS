#!/usr/local/bin/perl
use strict;
use warnings;
use Data::Dumper;

my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
my $inlib=$ENV{'INLIB'};

my $acfile=$ARGV[0];
my $conffile=$ARGV[1];
my $pcre2in=$ARGV[2];
my $pcre2out=$ARGV[3];
my $pcre2cin=$ARGV[4];
my $pcre2cout=$ARGV[5];
my $pcre2_major;
my $pcre2_minor;
my $pcre2_prerelease;
my $pcre2_date;
my $package_string;
my $package_version;
my $version;

# add code to analyse C:\projects\%INLIB%\configure.ac and use these strings:
#m4_define(pcre2_major, [10])
#m4_define(pcre2_minor, [20])
#m4_define(pcre2_prerelease, [])
#m4_define(pcre2_date, [2015-06-30])
# to replace in pcre2.h.in
#@PCRE2_MAJOR@
#@PCRE2_MINOR@
#@PCRE2_PRERELEASE@
#@PCRE2_DATE@
# and generate pcre2.h

#In config.h.in, look for 
#PACKAGE_STRING 
#PACKAGE_VERSION
#VERSION 
# and replace values in config.h

my $configac = $base . "\\" . $inlib . "\\" . $acfile;
open AC, "< $configac" or die "cannot open $configac $!\n";
while (<AC>)
{
	if (/m4_define\(pcre2_major\,\s*\[(\S*)\]/)
		{$pcre2_major = $1;}
	if (/m4_define\(pcre2_minor\,\s*\[(\S*)\]/)
		{$pcre2_minor = $1;}	
	if (/m4_define\(pcre2_prerelease\,\s*\[(\S*)\]/)
		{$pcre2_prerelease = $1;}
	if (/m4_define\(pcre2_date\,\s*\[(\S*)\]/)
		{$pcre2_date = $1;}
}
close AC;
my $configh = $base . "\\" . $inlib . "\\" . $conffile;
open ACC, "< $configh" or die "cannot open $configh $!\n";
while (<ACC>)
{
	if (/\bPACKAGE_STRING\s+\"([^"]*)\"/)
		{$package_string = $1;}
	if (/\bPACKAGE_VERSION\s+\"([^"]*)\"/)
		{$package_version = $1;}	
	if (/\bVERSION\s+\"([^"]*)\"/)
		{$version = $1;}
}
close ACC;

my $inpcre2 = $base . "\\" . $inlib . "\\" . $pcre2in;
my $outpcre2 = $bin . "\\" . $pcre2out;
open IN, "< $inpcre2" or die "cannot open $inpcre2 $!\n";
open OUT, "> $outpcre2" or die "cannot open $outpcre2 $!\n";
while (<IN>)
{
	my $line = $_;
	$line =~ s/\@PCRE2_MAJOR\@/       $pcre2_major/x;
	$line =~ s/\@PCRE2_MINOR\@/       $pcre2_minor/x;
	$line =~ s/\@PCRE2_PRERELEASE\@/  $pcre2_prerelease/x;
	$line =~ s/\@PCRE2_DATE\@/        $pcre2_date/x;
	print OUT $line
}
close IN;
close OUT;

my $inpconf = $bin . "\\" . $pcre2cin;
my $outconf = $bin . "\\" . $pcre2cout;
open INC, "< $inpconf" or die "cannot open $inpconf $!\n";
open OUTC, "> $outconf" or die "cannot open $outconf $!\n";
while (<INC>)
{
	my $line = $_;
	$line =~ s/\b(PACKAGE_STRING\s+\")([^"]*)(\")/$1$package_string$3/x;
	$line =~ s/\b(PACKAGE_VERSION\s+\")([^"]*)(\")/$1$package_version$3/;  
	$line =~ s/\b(VERSION\s+\")([^"]*)(\")/$1$version$3/;
	print OUTC $line
}
close INC;
close OUTC;


