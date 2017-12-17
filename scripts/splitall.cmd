rem set BINPATH=C:\projects\pcreport
set BINPATH=\\SUSIE-PC\pcre2port
set DELIMITER=\
rem set BASE=C:\projects\pcreport
set BASE=\\SUSIE-PC\pcre2port
set LIB=SRCE1
perl %BINPATH%%DELIMITER%splitupdte.pl %BINPATH%%DELIMITER%zos.load.txt
