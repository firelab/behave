
# ****************************************************************************/
# This is set arbitrarily at 2.6.  If there is proof that it works better, 
# set it.  Also set all sub CMakeLists.txt
# *****************************************************************************
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(behave)

OPTION(CLI "Enable CLI boost parser" OFF)
IF(CLI)
    ADD_DEFINITIONS(-DCLI)
ENDIF(CLI)

SET(SOURCE
    src/behave.cpp
    src/fuelModels.cpp
    src/newext.cpp
    src/palmettoGallberry.cpp
    src/randfuel.cpp
    src/randthread.cpp
    src/surface.cpp
    src/surfaceFirePropogatingFlux.cpp
    src/surfaceFireReactionIntensity.cpp
    src/surfaceFuelbedIntermediates.cpp
    src/surfaceInputs.cpp
    src/surfaceFireSpread.cpp
    src/surfaceTwoFuelModels.cpp
    src/westernAspen.cpp
    src/windAdjustmentFactor.cpp)

SET(HEADERS
    src/behave.h
    src/fuelModels.h
    src/newext.h
    src/palmettoGallberry.h
    src/randfuel.h
    src/randthread.h
    src/surface.h
    src/surfaceEnums.h
    src/surfaceFirePropogatingFlux.h
    src/surfaceFireReactionIntensity.h
    src/surfaceFuelbedIntermediates.h
    src/surfaceInputs.h
    src/surfaceFireSpread.h
    src/surfaceTwoFuelModels.h
    src/westernAspen.h
    src/windAdjustmentFactor.h)
	
ADD_EXECUTABLE(behave ${SOURCE} src/client.cpp ${HEADERS})

IF(CLI)
    ADD_EXECUTABLE(behave-cli ${SOURCE} src/cli.cpp ${HEADERS})
ENDIF(CLI)