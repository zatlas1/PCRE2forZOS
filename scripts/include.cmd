echo off
cd C:\projects\pcre2port
set path=%path%;C:\perl
set BASE=C:\projects
set BIN=C:\projects\pcre2port
set INLIB=pcre2-10.30
set IGNORELIB=doc,testdata,maint,.svn,cmake,m4
set IGNOREEXT=dll,com,exe
echo on
perl include.pl 