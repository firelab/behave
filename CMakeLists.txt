
# ****************************************************************************/
# This is set to 2.8.11 to use the target_link_libraries command. In CMake 
# 2.8.11 and later versions, this command automatically adds appropriate 
# include directories, compile definitions, the position-independent-code flag, 
# and links to the qtmain.lib library on Windows.
# *****************************************************************************

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

# Make use of c++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

PROJECT(behave)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/behave
                    ${CMAKE_SOURCE_DIR}/src/cli)      

OPTION(CLI "Enable Behave CLI" OFF)
IF(CLI)
    ADD_DEFINITIONS(-DCLI)
ENDIF(CLI)

OPTION(RAWS_BATCH "Enable Behave RAWS Data Batch Reader" OFF)
IF(RAWS_BATCH)
    ADD_DEFINITIONS(-DRAWS_BATCH)
ENDIF(RAWS_BATCH)

OPTION(TEST_BEHAVE "Enable Testing" OFF)
IF(TEST_BEHAVE)
    ADD_DEFINITIONS(-DTEST_BEHAVE)
ENDIF(TEST_BEHAVE)

# Commented out OpenMP requirement for now
#Make sure OpenMP is supported by compiler
#FIND_PACKAGE(OpenMP REQUIRED)
#IF(OPENMP_FOUND)
#    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#ENDIF(OPENMP_FOUND)

SET(SOURCE
    src/behave/behaveRun.cpp
    src/behave/behaveVector.cpp
	src/behave/behaveUnits.cpp
    src/behave/crown.cpp
    src/behave/crownInputs.cpp
    src/behave/fuelModelSet.cpp
	src/behave/ignite.cpp
    src/behave/newext.cpp
    src/behave/palmettoGallberry.cpp
    src/behave/randfuel.cpp
    src/behave/randthread.cpp
    src/behave/spot.cpp
    src/behave/spotInputs.cpp
    src/behave/surface.cpp
    src/behave/surfaceFireReactionIntensity.cpp
    src/behave/surfaceFuelbedIntermediates.cpp
    src/behave/surfaceInputs.cpp
    src/behave/surfaceFire.cpp
    src/behave/surfaceTwoFuelModels.cpp
    src/behave/westernAspen.cpp
    src/behave/windAdjustmentFactor.cpp
    src/behave/windSpeedUtility.cpp)

SET(HEADERS
    src/behave/behaveRun.h
    src/behave/behaveVector.h
	src/behave/behaveUnits.h
    src/behave/crown.h
    src/behave/crownEnums.h
    src/behave/crownInputs.h
    src/behave/fuelModelSet.h
	src/behave/ignite.h
	src/behave/igniteEnums.h
    src/behave/newext.h
    src/behave/palmettoGallberry.h
    src/behave/randfuel.h
    src/behave/randthread.h
    src/behave/spot.h
    src/behave/spotEnums.h
    src/behave/spotInputs.h
    src/behave/surface.h
    src/behave/surfaceEnums.h
    src/behave/surfaceFireReactionIntensity.h
    src/behave/surfaceFuelbedIntermediates.h
    src/behave/surfaceInputs.h
    src/behave/surfaceFire.h
    src/behave/surfaceTwoFuelModels.h
    src/behave/westernAspen.h
    src/behave/windAdjustmentFactor.h
    src/behave/windSpeedUtility.h)

SOURCE_GROUP("Behave Core Source Files" FILES ${SOURCE})

SOURCE_GROUP("Behave Core Header Files" FILES ${HEADERS})

ADD_EXECUTABLE(behave 
    ${SOURCE} 
    src/behave/client.cpp 
    ${HEADERS})

IF(TEST_BEHAVE)
    SET(Boost_USE_STATIC_LIBS       ON) # only find static libs
    SET(Boost_USE_MULTITHREADED     OFF)
    FIND_PACKAGE(Boost REQUIRED)
    IF(Boost_FOUND)
        FIND_PACKAGE(Boost 1.61.0 
            COMPONENTS unit_test_framework REQUIRED)
        INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
        SET(LINK_LIBS ${Boost_LIBRARIES})
        SET(BOOST_TEST_SOURCE
            src/testBehave/testBehave.cpp)
        ADD_EXECUTABLE(testBehave 
            ${SOURCE}
            ${BOOST_TEST_SOURCE}
            ${HEADERS})
        TARGET_LINK_LIBRARIES(testBehave ${Boost_LIBRARIES})
    ENDIF()
ENDIF(TEST_BEHAVE)

IF(RAWS_BATCH)
    ADD_EXECUTABLE(behave-raws-batch
        ${SOURCE}
        src/rawsBatch/behaveRawsBatch.cpp
        ${HEADERS})
ENDIF(RAWS_BATCH)

IF(CLI)
    ADD_EXECUTABLE(behave-cli 
        ${SOURCE}
        src/cli/behaveCli.cpp
        ${HEADERS})
ENDIF(CLI)
