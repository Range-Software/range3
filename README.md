[![Build Status](https://travis-ci.org/Range-Software/range3.svg?branch=master)](https://travis-ci.org/Range-Software/range3)
[![CircleCI](https://circleci.com/gh/Range-Software/range3.svg?style=svg)](https://circleci.com/gh/Range-Software/range3)

# Range Software
Finite Element Analysis

![Range Software](http://www.range-software.com/files/common/Range3-CFD.png)

## Prepare build environment
Following command will download and install all required packages, therefore it must be executed under priviledged (root) user.
```
$ sudo ./scripts/prereqs.sh
```
## Build & Install
```
$ ./scripts/build.sh --clean && ./scripts/create_package.sh
```
## Running the software
```
$ $HOME/bin/range-3.2.3/bin/Range
```

## Download
To download already built binaries please visit http://range-software.com

## Powered by

* Qt - https://www.qt.io/
* TetGen - http://tetgen.org
