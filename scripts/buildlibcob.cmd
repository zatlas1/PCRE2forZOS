echo off
cd C:\projects\pcre2port
set path=%path%;C:\perl
set BASE=C:\projects
set BIN=C:\projects\pcre2port
set INLIB=COB
echo on
perl buildlib.pl zos.%INLIB%.txt
