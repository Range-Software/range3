TEMPLATE = subdirs

SUBDIRS = \
    TetGen \
    RangeBase \
    RangeAuth \
    RangeModel \
    RangeSolverLib \
    RangeLicense \
    RangeSolver \
    Range

RangeModel.depends = RangeBase TetGen
RangeSolverLib.depends = RangeModel
RangeLicense.depends = RangeAuth
RangeSolver.depends = RangeSolverLib RangeAuth
Range.depends = RangeSolverLib RangeAuth

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered
CONFIG += depend_includepath
