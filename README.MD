# hashScan:
hashScan is a simple tool for checking whether a file is malicious or not. It is meant
to speed up the process of finding malicious files by providing a simple interface to services
such as [threatExperts](http://threatexpert.com/).

Usage:
````bash
test@Test-PC $ ./hashScan file1.exe
  Scanning [file1.exe]
  md5: <randomhash>
  entropy: <randomShannonentropy>
  Malicious: <probably>
````

As a side note , I'm making a point of checking all my code with -Wall && -Werror enabled. I'm also running every program through valgrind to check for memory errors. I'd really recommend this to anyone starting out with C.
