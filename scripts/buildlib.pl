use strict;
use warnings;
use Data::Dumper;

#my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
my $inlib=$ENV{'INLIB'};

my $outfile=$ARGV[0];

$outfile = $bin ."\\". $outfile;
print "Writing $outfile\n";
open our $OUT, "> $outfile" or die "cannot open $outfile $!\n";
process_files ($bin . "\\" . $inlib);
close $OUT;
exit 0;

# Accepts one argument: the full path to a directory.
# Returns: nothing.
sub process_files 
{
    my $path = shift;

    # Open the directory.
    opendir (DIR, $path)
        or die "PF001-Unable to open $path: $!";
    my @files = grep { !/^\.{1,2}$/ } readdir (DIR);
    closedir (DIR);
    @files = map { $path . "\\" . $_ } @files;

    for (@files) 
	{
        # If the file is a directory
        if (-d $_) 
		{
            my $lib = $_;
			$lib =~ s/.*\\([^\\]*)$/$1/;
			process_files ($_);
        } 
		else 
		{
            my $ext = $_;
			$ext =~ s/.*\.([^\.\\]*)$/$1/;
			if ($ext ne 'OLD')
			{		
				copy_file ($_);
			}
        }
    }
}

sub copy_file
{
	my $infile = shift;
	my $member = $infile;
	$member =~ s/\.[^\.\\]*$//; 
	$member =~ s/^.*\\//;
	$member =~ s/_//g;
	$member =~ s/(.{1,8}).*/$1/;
	$member = uc($member);
	print $OUT "./ ADD NAME=$member\n";
	open MEM, "< $infile" or die "cannot open $infile $!\n";
	while (<MEM>)
	{
		chomp;
		my $s = $_;
		$s=~ s/\t/ /g;
		$s=~ s/\xff/\\xFF/g;
		print $OUT "$s\n";
	}
	close MEM;
}
