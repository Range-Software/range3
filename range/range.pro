TEMPLATE = subdirs

SUBDIRS = \
    TetGen \
    RangeBase \
    RangeModel \
    RangeSolverLib \
    RangeSolver \
    Range \
    RangeTests

RangeModel.depends = RangeBase TetGen
RangeSolverLib.depends = RangeModel
RangeSolver.depends = RangeSolverLib
Range.depends = RangeSolverLib

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered
CONFIG += depend_includepath