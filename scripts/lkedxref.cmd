rem echo off
cd C:\projects\pcre2port
set path=%path%;C:\perl
set BIN=C:\projects\pcre2port
echo on
perl %BIN%\lkedxref.pl %BIN%\compile0.txt ZATLAS1.PCRE2.SRCE >%BIN%\lkedxref.txt