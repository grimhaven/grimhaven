Grimhaven MUD
=============

This is the source code to the Grimhaven MUD server. It is derived from the SneezyMUD 5.2
source code and game data release. It is tested on Debian GNU/Linux, but will probably
build and run on any Unix-like system meeting the prerequisites.

Building
========

You will need the following available to build:

* Unix-like system (tested with Ubuntu 12.10 "quantal")
* glibc (tested with eglibc 2.15)
* GNU C++ compiler (tested with g++ 4.7)
* scons build tool (tested with 2.2.0 on Python 2.7)
* MySQL client C library (tested with MySQL 5.5.27)
* c-ares library (tested with 1.9.1)
* Boost C++ libraries (tested with v1.50), including the following modules:
  * date-time
  * program-options
  * regex
  * serialization

Once these are met, simply run the `scons` command in this directory.

Setup
=====

TODO

Running
=======

TODO

.. vim: tw=79 ft=rst
