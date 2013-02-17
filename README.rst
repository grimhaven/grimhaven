Grimhaven MUD
=============

This is the source code and game data of the Grimhaven `MUD <http://en.wikipedia.org/wiki/MUD>`_.

It is derived from the SneezyMUD 5.2 source code and game data release, as found at `https://bitbucket.org/peel/sneezymud/ <https://bitbucket.org/peel/sneezymud/>`_ and `https://bitbucket.org/cizra/sneezymud <https://bitbucket.org/cizra/sneezymud>`_.

License / Copyright / Credits
=============================

Insofar as a fork of a licenseless MUD that was authored on a somewhat casual basis by tons of people can have a license, Grimhaven is being made available under the terms of the `GNU Affero General Public License version 3 <http://www.gnu.org/licenses/>`_. Please see LICENSE.txt and CREDITS.txt for details.

Building
========

You will need the following software installed to build:

* Linux-based operating system (tested on Debian unstable / Ubuntu 12.10 quantal with kernel v3.7)
* glibc (tested with eglibc 2.15)
* GNU C++ compiler with C++11 support (tested with g++ 4.7)
* scons build tool (tested with 2.2.0 on Python 2.7)
* MySQL client C library (tested with MySQL 5.5.27)
* c-ares library (tested with 1.9.1)
* Boost C++ libraries (tested with v1.50), including the following modules:
  * date-time
  * program-options
  * regex
  * serialization

Once these requirements are met, simply run the `scons` command in the root of this source tree, which will by default build the MUD server binary `build/grimhaven`.

Setup
=====

TODO

Running
=======

TODO

Coding Guildelines
==================

C++
---

All new C++ code shall conform to the `Google C++ Style Guide <http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml>`_.

Highlights
__________

* All header files should have `#define` guards to prevent multiple inclusion. The format of the symbol name should be `<PROJECT>_<PATH>_<FILE>_H_`.
* Indentation is 2 spaces (no tabs).
* Open curly brace is never on its own line.
* Strongly avoid use of `#define` macros.
* All parameters passed by reference must be labeled const.
* Use overloaded functions (including constructors) only if a reader looking at a call site can get a good idea of what is happening without having to first figure out exactly which overload is being called.
* Use streams only for logging.

Exceptions
__________

* The use of C++ exceptions is permitted.

Other Guidelines
----------------

* Avoid use of external libraries apart from Boost modules.
* Anything not written in C++ is written in Python.

.. -*- tab-width: 2; -*- vim: ft=rst:sw=2:sts=2:ts=8:et
