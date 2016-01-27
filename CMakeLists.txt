
# ****************************************************************************/
# This is set arbitrarily at 2.6.  If there is proof that it works better, 
# set it.  Also set all sub CMakeLists.txt
# *****************************************************************************
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(behave_cli)

OPTION(CLI "Enable CLI boost parser" OFF)
IF(CLI)
    ADD_DEFINITIONS(-DCLI)
ENDIF(CLI)

# *****************************************************************************
# Find boost libs, 
# *****************************************************************************
INCLUDE(FindBoost)
SET(Boost_DEBUG ON)
SET(Boost_USE_MULTITHREAD OFF)
FIND_PACKAGE(Boost 1.41.1 COMPONENTS program_options REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}) 

ADD_EXECUTABLE(behave src/behavePlus.cpp
                           src/client.cpp
                           src/fuelModels.cpp
                           src/surface.cpp
                           src/surfaceFirePropogatingFlux.cpp
                           src/surfaceFireReactionIntensity.cpp
			   src/surfaceFuelbedIntermediates.cpp
                           src/surfaceInputs.cpp
                           src/surfaceFireSpread.cpp)
                       
TARGET_LINK_LIBRARIES(behave ${Boost_LIBRARIES})
