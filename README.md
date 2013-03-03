Filesystem_DB
=============

Written by Oskari Rauta.
License: expat ( check copying.txt )

Filesystem DB Library for Arduino/Maple/etc..

This is a library for easy access/manipulation of 2 linked list style databases,
one for filesystem's folders and one for files.

I am/was (future reference) working with a project, that has a "server" system running
on a raspberry pi, and a "client" system running on Arduino (Due) - this client system
is connected "server" with rs485 and is equipped with a SD card. Server can update files
and folders on that SD card over the rs485. Now I needed a system that could check if
and file on SD card is outdated. So I decided that server should send a md5sum of file
when it sends it. This way we can have a database. I store this database on that sd card
also as a file not belonging to this database. Server can now retrieve folder and file
databases from client to see if required file or folder is missing or file on server has
been updated.

I made it to store entries in the database in alphabetical order, so list would be easier
to read and compare.

This doesn't use malloc and free. This uses new and delete - so it's maple safe.
Haven't tested it on maple, but should work, although the example propably needs a bit
tweaking.. It also needs a change from Serial to Serial1 if you use it on Arduino Uno/Mega/etc - other than Due..

Usage documentation is on Filesystem_DB.h and there is a example in examples folder.
Remember to read mentioned warnings in Filesystem_DB.h