Where is it ?
-------------
https://github.com/berilevi/SamKnows

What Is This?
-------------
SamKnows Technical Test (C/C++)
This is the technical test for our Test and Metrics (C/C++) positions at SamKnows. 
If you just stumbled across this and you are interested in joining our team, go to our careers page to see if we are hiring. 
Thank you for your interest in becoming part of SamKnows. We would like you to complete the technical test below; we're excited to see your solution! 
Before we start, we want to give you an idea of what we are looking for and what should be considered for your solution. 
This should roughly take you two hours, depending on how familiar you are with libcurl. 
If you'd like to do more, but don't have time then please mention it in your README.

Dont’s
Do not limit yourself to old versions of ISO C
Do’s
Do write POSIX / ISO C conformant code (feel free to comment when this limits you)
Do add a Makefile or something that creates a Makefile (GNU Make extensions are OK)
Do document or automate if you have done something to verify the quality of your code that we can't see
Do write a README
Do elaborate on what you would do, given the time
Do pay an absurd amount of attention to detail. This is a small program on purpose, we want you to have time to aim for perfection.
The Challenge
We are interested in knowing how good the connectivity to http://www.google.com/ is. If you aren't already, we would expect you to become familiar with the (really simple) https://curl.haxx.se/libcurl/c/ "easy" interface. Your task is to create a library that will execute one or more HTTP GET requests to http://www.google.com/, read the responses, and extract statistics from the responses (e.g. the TCP connection establishment time). You should also write a program that will use this library.

You are free to design your library interface. But it should not leak the libcurl interface to its user, it should allow an alternative implementation that doesn't use libcurl.

Your program should accept the following arguments at a minimum (you are free to suggest additional arguments):

-H "Header-name: Header-value": can be used multiple times, each time specifying an extra HTTP header to add to your request
-n <integer>: number of HTTP requests to make (i.e. the number of samples you will have to take the median of)
The output of your program should be a semicolon delimited list of values in this format:

SKTEST;<IP address of HTTP server>;<HTTP response code>;<median of CURLINFO_NAMELOOKUP_TIME>;<median of CURLINFO_CONNECT_TIME>;<median of CURLINFO_STARTTRANSFER_TIME>;<median of CURLINFO_TOTAL_TIME>
Test Considerations
As mentioned above, we are particularly interested in the details around your submission. This includes the commits you made, checking for consistent coding style, the safety checks your code uses, how portable your code is, etc. We really want to get a good idea of how you structure your code and how clean and efficient your code is.

Last but not least, please make sure you test your code and make it as easy to run as possible.

Good luck!

Files in this release:
----------------------
Readme.txt - This file.
SamClient/main.cpp
SamLib/samlib.cpp
SamLib/samlib.hpp

To build the software:
----------------------

 - Untar the source code.
 - Type 'make'

Following files will be created in the current directory.
 -client
    This is the test application for the libsam library.
 -libsam.so
    Shared library used to collect median statistics from an url by doing http get request. 

How To Run 
----------
You may want to copy libsam.so to your system shared library folder or
LD_LIBRARY_PATH=. ./client
or even running ./client  should work too as i used rpath to store the library path during link time

Command Line Arguments
----------------------
./client
Usage:
-H "Header-name: Header-value": can be used multiple times, each time specifying an extra HTTP header to add to your request
-n <integer>: number of HTTP requests to make (i.e. the number of samples you will have to take the median of)
-u Url to connect

Some Examples
-----------------------
./client -u google.com
./client -u google.com -n 5
./client -n 10 -H "Another: yes" -H "Another: " -H "Host: example.com" -H "Beri: Levi" -u http://www.google.com

Future Additions
----------------
CAn create a wrapper enum on CURLcode to expose all the error codes curl library support instead of simple "true" or "false" states to report API state.
Can rewrite it in ansi C or C11. As is it has a very tiny wrapper class on top of curl interface as I thought this make my life easier to implement and use curl library.
Can impement other HTTP methods like "PUT" etc.
