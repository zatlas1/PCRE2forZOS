echo off
cd C:\projects\pcre2port
set path=%path%;C:\perl
set BASE=C:\projects
set BIN=C:\projects\pcre2port
set INLIB=pcre2-10.30
echo on
perl generatepcre2h.pl configure.ac src\config.h.generic src\pcre2.h.in pcre2.h config.h config.h.new