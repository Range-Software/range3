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
### Linux
```
$ ./scripts/build.sh --clean && ./scripts/create_package.sh
```
### MacOS
```
$ ./scripts/build.sh --clean && ./scripts/create_mac_app.sh
```
Above command will produce `./build-Release/packages/range-3.2.5.dmg` which can be installed with following command:
```
$ open './build-Release/packages/range-3.2.5.dmg'
```
## Running the software
```
$ $HOME/bin/range-3.2.5/bin/Range
```

## Download
To download already built binaries please visit http://range-software.com

## Powered by

* Qt - https://www.qt.io/
* TetGen - http://tetgen.org
