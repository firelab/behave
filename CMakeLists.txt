
# ****************************************************************************/
# This is set arbitrarily at 2.6.  If there is proof that it works better, 
# set it.  Also set all sub CMakeLists.txt
# *****************************************************************************
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(behave_cli)

ADD_EXECUTABLE(behave src/behavePlus.cpp
                           src/client.cpp
                           src/fuelModels.cpp
                           src/surface.cpp
                           src/surfaceFirePropogatingFlux.cpp
                           src/surfaceFireReactionIntensity.cpp
			   src/surfaceFuelbedIntermediates.cpp
                           src/surfaceInputs.cpp
                           src/surfaceFireSpread.cpp)
TARGET_LINK_LIBRARIES(canopy_flow ${PLPLOT_LIB} ${PLPLOT_LIB_CXX})