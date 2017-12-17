echo off
cd C:\projects\pcre2port
set path=%path%;C:\perl
set BASE=C:\projects
set BIN=C:\projects\pcre2port
set INLIB=pcre2-10.30\testdata
echo on
perl copytest.pl zos.test.txt 
rem
set INLIB=TESTING
perl buildlib.pl zos.%INLIB%.txt
rem
copy zos.test.txt+empty.txt+zos.%INLIB%.txt zos.test.txt