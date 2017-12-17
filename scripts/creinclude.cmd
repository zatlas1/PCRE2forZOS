rem improve this
set BIN=C:\projects\pcre2port
rem set BIN=\\SUSIE-PC\pcreport
perl %BIN%\creinclude.pl %BIN%\lkedxref.txt %BIN%\zos.lked.txt >%BIN%\creinclude.log 
rem 2>&1
rem copy /b zos.lked.txt+empty.txt+zos.REXX.txt zos.lked.txt