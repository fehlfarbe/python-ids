# python-ids
[SWIG](http://www.swig.org/) generated Python wrapper for [IDS cameras](https://de.ids-imaging.com/).

Supports master-slave trigger via external triggercable.

Installation
------------
1. install the IDS uEye SDK
2. install swig: `sudo apt-get install swig` for Debian/Ubuntu like systems
3. run `./swigbuild.sh` to compile the shared library and build wrappers
4. run `sudo python setup.py install` to install the wrappers
