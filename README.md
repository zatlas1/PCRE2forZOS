# PCRE2forZOS
README
------
This is a port (not a fork) of the PCRE2 library (available also on GitHUub 
here: https://github.com/luvit/pcre2)
The original project and thus also this port is licensed under bsd-3-clause.
This port works with EBCDIC rather than ASCII or Unicode.  It is meant to
compile and run under the IBM z/OS operating system, using the IBM C/C++
compiler, IBM COBOL compiler, HLASM Assembler and TSO Rexx langauge with
JCL scripts rather then Unix/Linux shell or make scripts.

While IBM supplies the basic standard Posix compliant regular expression 
functions, it does not provide any proper header files for the mainstay 
languages COBOL and PL/I.  In addition, these functions provide the basic
Posix functionality, while most languages strive to provide Perl like 
functionality  By porting the PCRE2 library, providing a proper COBOL API
and Rexx API we allow both LE languages and Rexx to enjoy the benefits of
a standard, widely used and popular regular eepression library.

The binaries and source code in EBCDIC are provided in http://cbttape.org
file 939 (http://cbttape.org/ftp/cbt/CBT939.zip).  The GitHub version would
try to provide the ASCII version with proper instructions how to load that 
into EBCDIC.  This is meant basically as a backup for the EBCDIC installation
package on the CBTTAPE.

This GitHub site does not contain any of the original dource code or any 
other component of the original.  Those materials are available as was 
mentioned above in https://github.com/luvit/pcre2.  If you examine the 
original, you will find in few places, some refernce to 'NATIVE_ZOS'.  _Those 
references were added to the original code on my request to help accomodate 
this port.  The most invasive intervention in the original code is pcre2grep 
to accomodate z/OS file types that do not exist in other environments.

As I learn more about GIT, I will see if I can provide the whole thing here.

Contributing and help resources:
--------------------------------
PCRE2 is managed by Phil Hazel (University of Cambridge) and Zoltán Hherczeg
(University of Szeged, Hungary).  Any contribution to the original as well as
requests for help and bug reports should be coordinated with them via 
pcre-dev@exim.org.
If you wish to contribute to the z/OS port or have a PCRE2 for z/OS specific 
question or bug report, you may coordinate with me via zatlas1@yahoo.com.  
Please add the words PCRE2 BUG REPORT or PCRE2 QUESTION to your email subject.
