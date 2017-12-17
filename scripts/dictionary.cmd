echo off
cd C:\projects\pcre2port
set path=%path%;C:\perl
set BASE=C:\projects
set BIN=C:\projects\pcre2port
set INLIB=pcre2-10.30
echo on
perl dictionary.pl --dict=dictionary.txt newdictionary.txt
rem perl dictionary.pl -- newdictionary.txt