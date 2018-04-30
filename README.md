# Range Software
Finite Element Analysis

![Range Software](http://www.range-software.com/new/files/common/Range3-CFD.png)

## Based on

* Qt - https://www.qt.io/
* TetGen - http://tetgen.org

## Build
```
$ ./scripts/build.sh --clean
```
To create an RPM package 
```
$ ./scripts/create_package.sh --version=3.0.0 --distro=<linux_distribution>
```
Where **<linux_distribution>** needs to be replaced with short string identifying linux distribution on which the software was built.

## Download
To download already built binaries please visit http://range-software.com
