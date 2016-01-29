
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

# *****************************************************************************
# Find boost libs, 
# *****************************************************************************
INCLUDE(FindBoost)
IF(Boost_FOUND)
	SET(Boost_DEBUG ON)
	SET(Boost_USE_MULTITHREAD OFF)
	FIND_PACKAGE(Boost 1.41.1 COMPONENTS program_options REQUIRED)

	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}) 
ENDIF(Boost_FOUND)

SET(SOURCE
	src/behave.cpp
	src/fuelModels.cpp
	src/surface.cpp
	src/surfaceFirePropogatingFlux.cpp
	src/surfaceFireReactionIntensity.cpp
	src/surfaceFuelbedIntermediates.cpp
	src/surfaceInputs.cpp
	src/surfaceFireSpread.cpp
)

SET(HEADERS
	src/behave.h
	src/fuelModels.h
	src/surface.h
	src/surfaceFirePropogatingFlux.h
	src/surfaceFireReactionIntensity.h
	src/surfaceFuelbedIntermediates.h
	src/surfaceInputs.h
	src/surfaceFireSpread.h
)
	
ADD_EXECUTABLE(behave ${SOURCE} src/client.cpp ${HEADERS})
IF(Boost_FOUND)
	ADD_EXECUTABLE(behave_cli ${SOURCE} src/cli.cpp ${HEADERS})
ENDIF(Boost_FOUND)

IF(Boost_FOUND)
	TARGET_LINK_LIBRARIES(behave ${Boost_LIBRARIES})
ENDIF(Boost_FOUND)