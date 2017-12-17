# Script to split a IEBUPDTE flat file to individual files in directory
# Split marker './ ADD|REPL NAME=membname' in COL1
# Ignore COL1
# Out directory = inputfilename . timestamp in supplied base directory
# ---------------------------
# Usage perl splitndvr.pl [dir/]inputfile baseoutdir
# C:\projects\download>perl c:\projects\splitndvr.pl --nl --punch INFILE OUTDIR

use strict;
use warnings;
use File::Path qw(make_path remove_tree);

my $binpath = $ENV{'BINPATH'};
my $delimiter = $ENV{'DELIMITER'};
my $del = quotemeta ($delimiter);
my $punchlen = 80; # defaults to no NL
my $outdir;        # the output directory name
my $i = 0;         # loop index
my $infile;        # the input file name
my $status = 0;
our $opout = 0;     # no outfile in queue
our $OFILE;
$infile = $ARGV[0];  # fully qualified
my $baselib = $ENV{'BASE'}; 
$outdir = $baselib;
my $lib = $ENV{'LIB'};
$outdir .= "${delimiter}" . $lib;
#print "$outdir\n";
#exit;
print "START SPLITNDVR $infile\n";
if (-d $outdir)
{
	remove_tree($outdir) or die $!;
}
make_path $outdir or die $!;
open IFILE, "$infile" or die $!;
   while (<IFILE>)
   {
      my $data = $_;
	  chop $data;
	  zasplit ($data, $outdir);
   }      
	  
close(IFILE);

print "END SPLITNDVR $infile\n";
exit 0;

sub zasplit
{
     my $outfile;       # the output file name
	 my $data = shift;
	 my $outdir = shift;
	 if ($data =~ /^\.\/\s+(?:ADD|REPL)\s+NAME=([\@\$\#A-Za-z0-9]+)\s*$/)
     {
        $outfile = $1;
		$status = 0;
	    if ($opout == 1) 
	    {  
		   close $OFILE or decease $!;
	    }
	    else 
	    {
	      $opout = 1;
        }
        open $OFILE, ">$outdir${delimiter}$outfile"
			or decease $!;
     }
	 elsif ($data =~ /^(\+|\.|V|)MEMBER\s+NAME\s+(\S+)/)
	 {
		print "illegal member name ignored: $data\n";
		if ($opout == 1) 
	    {  
		   close $OFILE or decease $!;
		   $opout = 0;
	    }
		$status = 1;
	 }
	 elsif ($data =~ /^\.\/\s+ADD\s+NAME=(\S+)(\s+|$)/)
	 {
        $outfile = $1;
		$status = 0;
	    if ($opout == 1) 
	    {  
		   close $OFILE or decease $!;
	    }
	    else 
	    {
	      $opout = 1;
        }
        open $OFILE, ">$outdir${delimiter}$outfile"
			or decease $!;
     }
     else
     {	
		  print $OFILE "$data\n" if $status == 0;
     }
}
#sub decease
#{
#	# die/LOGDIE REPLACEMENT
#	my $errmsg = shift;
#	print "LOGDIE $errmsg\n";
#	exit 61;
#}