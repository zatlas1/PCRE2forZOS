#!/usr/local/bin/perl
use strict;
use warnings;
use Data::Dumper;
use DBI;
#use File::Find::Rule; 
my $sw = 0;
 
my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
#my $inlib=$ENV{'INLIB'};
my $outfile=$ARGV[0];
my $outrel= $ARGV[1];
my $outpragma = $ARGV[2];
$outfile = $bin . "\\" . $outfile;
$outrel = $bin . "\\" . $outrel;
# here add manually unsourced (manually dealt with) original files
# If we do not want a file - add an element 'filename 0'
# files that we control their source will appear as 'filename 1'
my %nosource =qw(
pcre2_chartables.c 1
pcre2.h 1
config.h 1
);

# "Creating database handle"
my $dbh = DBI->connect ("dbi:CSV:",  "", "", {
    f_dir => $bin,
    })  or
    die "Cannot connect: $DBI::errstr";

# I need to put the whole dictionary in some hash and mark which components are relevant
# all relevant components will be replaced and report produced
# all irrelevant components must be reported in exception report

my $pat = "\(";
my $pats = "\(";

# Selecting
my %dict = ();
my %dicts = ();
my %except = ();
my %reverse=();
$dbh->{RaiseError} = 1;
# confine the treatment below to header files only.  For C modules, create pragma file - PCRPRGMA.H
my $query = "select distinct component, member, lib from dictionary\.txt where \(lib = \'pcre2\' or lib= \'POSIX\'\) and change = \'Y\'";
my $sth   = $dbh->prepare ($query);
#print Dumper (\%$sth); exit;
$sth->execute ();
while (my $row = $sth->fetchrow_hashref) 
{
	my $typ = $row->{component};
	$typ=~s/.*(\.(cc|c|h))$/$1/;
	$dict {$row->{component}}= $row-> {member} . $typ;
	$pat .= "\\b" . $row->{component} . "\\b\|";
	if ($row->{component} =~ /(.*)\.(cc|c)$/ or $row->{lib} eq 'POSIX')
	{
		my $x = $1;
		$x = $row->{component} if !$x;
		$dicts {$x}= $row-> {member};
		$pats .= "\\b" . $x  . "\\b\|";
		$except{$x}= $row-> {member};
		$reverse {$x} = $row->{component};
	}
	$except{$row->{component}}= $row-> {member};
}
$sth->finish ();
my $patposix = '(regfree|regerror|regexec|regcomp)';

open OUTP, "> $outpragma";
foreach my $c (keys %except)
{
	#pragma map (native_old_name, "SHRTNAME")
	if ($c !~ /\./ and $c !~/$patposix/) 
		{print OUTP "\#pragma map \($c, \"$except{$c}\"\)\n";}
}
close OUTP;

$pat =~ s/\|$/\)/;
$pats =~ s/\|$/\)/;
#print $pat, "\n";
#print $pats, "\n";
#exit;

open OUT, "> $outfile";
open my $REL, "> $outrel";
print $REL "SOURCE_LOC,SOURCE,DEPENDENCY\n";

#/\/\/\/\/\/\/ work on it
# Refine the conversion to not touch anything but .h names
# Selecting
$dbh->{RaiseError} = 1;
$query = "select distinct component, type, member, source_loc, change from dictionary\.txt where lib = \'pcre2\' or lib= \'POSIX\' and \(change = \'Y\' or change = \'C\'\)";
$sth   = $dbh->prepare ($query);
$sth->execute ();
while (my $row = $sth->fetchrow_hashref) 
{
	auto_convert ($row->{component}, $row-> {member}, $row->{source_loc}, $row->{change});
}
$sth->finish ();
$dbh->disconnect;
#$sw = 1; 
auto_copy ("config.h", "config", $bin);
auto_copy ("pcre2.h", "pcre2", $bin);
auto_copy ("pcre2_chartables.c", "chartab2", $bin);

close OUT;
exit 0;

sub auto_convert
{
	my $component = shift;
	my $member=shift;
	my $source_loc = shift;
	my $change = shift;
	#if ($sw == 0)
	{
		return if (exists ($nosource{$component}));
	}
	if ($component !~ /$patposix/)
	{
		my $infile= $source_loc . "\\" . $component;
		my $typ = $component;
		
		my @info =();
		my $i = 0;
		open LDREP, "< $infile" or die "cannot open $infile $!\n";
		print OUT "\.\/ ADD NAME=" , uc($member) , " \n";
		push @info, "\/\*autoconv-0001 $infile converted to $member\*\/\n";
	
		while (<LDREP>)
		{
			$i++;
			my $s = $_;
			if ($change =~ /^C/i)
			{
				print OUT $s;
			}
			else
			{
				# "#include \"pcre_internal.h\"\n\n");
				#if (/^\s*\#\s*include/)
				if (/^(\s*|\s*\"\s*)\#\s*include/)
				{
					my $p = 0;
					while (/$pat/og)
					{
						my $x = $1;
						my $y = $dict{$x};
						if (exists ($except{$x}))
						{
							push @info, "\/\*autoconv-0011 $member line: $i $x replaced by $y\*\/\n";
						}
						else
						{
							push @info, "\/\*autoconv-0012 $member line: $i $x not yet converted\*\/\n";
						}
						my $xa=$x;
						my $xb=$infile;
						$x = "\\b" .$x . "\\b";
						#$x=quotemeta($x);
						$s = replace ($s, $x, $y);
						relate ($xa, $xb, $REL);
					}
				}
# All this ts replaced with the #pragma map mechanism
# However, the POSIX internal name still have to be changed to prevent conflict
				else
				{
					my $p = 0;
					#while (/$pats/og)
					while (/$patposix/og)
					{
						my $x = $1;			
						my $y = $dicts{$x};
						if (exists ($except{$x}))
						{
							push @info, "\/\*autoconv-0013 $member line: $i $x replaced by $y\*\/\n";
						}
						else
						{
							push @info, "\/\*autoconv-0014 $member line: $i $x not yet converted\*\/\n";
						}
						my $xa=$x;
						my $xb=$infile;
						$x = "\\b" .$x . "\\b";
						#$x=quotemeta($x);
						$s = replace ($s, $x, $y);
						relate ($xa, $xb, $REL);			
					}
				}
				if (/\bfopen\b(.*?)\"(r|w)b\"/)
				{
					my $rw=$2;
					$s =~ s/\"(r|w)b\"/\"$1\"/;
					push @info, "\/\*autoconv-0015 $member line: $i fopen \"${rw}b\" replaced by \"$rw\"\*\/\n";
				}
				if (/\\(\ÿ|\^)/)
				{
					my $ofend = $1;
					print "$member $i $ofend\n";
				}
				if (/\<(pcre2\.h)\>/i)
				{
					my $pcreh = $1;
					$s=~ s/\<(pcre2.h)\>/\"$1\"/i;
					push @info, "\/\*autoconv-0016 $member line: $i <$pcreh> replaced by \"$pcreh\"\*\/\n"; 
				}
				$s=~ s/\t/   /g;
				$s=~ s/\ÿ/\\xFF/g;
				$s=~ s/\^/\xAC/g;  # ^ ==> ¬ ;in FTP ac ==> 5f		
				
# In first versions I did not make use of OPTFILE (akin to -D in Linux).  This necessitated			
# turning the HAVE_CONFIG_H option in artificial manner.  OPTFILE is now in the JCL, minimizing
# source code intervention.			
				#if ($s =~ /^\#\s*ifdef\s*HAVE_CONFIG_H/)
				#{
				#	print OUT "\#ifndef HAVE_CONFIG_H\n";
				#	$i++;
				#	push @info, "\/\*autoconv-0010 $member line: $i added HAVE_CONFIG_H\*\/\n";
				#	print OUT "\#   define HAVE_CONFIG_H 1\n";
				#	$i++;
				#	print OUT "\#endif\n";
				#	$i++;
				#}
				print OUT $s;
			}
		}
		close LDREP;
		for my $repline (@info)
		{
			print OUT $repline;
		}
	}
}

sub auto_copy
{
	my $component = shift;
	my $member=shift;
	my $source_loc = shift;
	#if ($sw == 0)
	{
		return if (exists ($nosource{$component}) && $nosource{$component} == 0);
	}
	my $infile= $source_loc . "\\" . $component;
	
	my @info =();
	my $i = 0;
	open LDREP, "< $infile" or die "cannot open $infile $!\n";
	print OUT "\.\/ ADD NAME=" , uc($member) , " \n";
	push @info, "\/\*autoconv-0021 $infile copied to $member\*\/\n";

	while (<LDREP>)
	{
		$i++;
		my $s = $_;
		print OUT $s;
	}
	close LDREP;
	for my $repline (@info)
	{
		print OUT $repline;
	}
}

sub replace
{
	my $s = shift;
	my $x = shift;
	my $y = shift;
	$s =~ s/$x/$y/;
	return $s;
}
sub relate
{
	my $xa = shift;
	my $xb = shift;
	my $REL = shift;
	$xb=~s/(\\([^\\]*))$/,$2/;
	$xa=$reverse{$xa} if exists ($reverse{$xa});
	print $REL "$xb,$xa\n";	
}