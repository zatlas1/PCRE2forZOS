echo off
cd C:\projects\pcre2port
set path=%path%;C:\perl
set BASE=C:\projects
set BIN=C:\projects\pcre2port
rem set INLIB=pcre-8.33
echo on
perl autoconv.pl zos.load.txt relate.txt pcrprgma.h