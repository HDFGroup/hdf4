## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(CTest)
SET (CTEST_PROJECT_NAME "HDF4")
SET (CTEST_NIGHTLY_START_TIME "20:00:00 EST")

SET (CTEST_DROP_METHOD "http")
SET (CTEST_DROP_SITE "nei.hdfgroup.uiuc.edu")
SET (CTEST_DROP_LOCATION "/cdash/submit.php?project=HDF4")
SET (CTEST_DROP_SITE_CDASH TRUE)
