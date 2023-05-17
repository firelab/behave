
# ****************************************************************************/
# This is set to 2.8.11 to use the target_link_libraries command. In CMake 
# 2.8.11 and later versions, this command automatically adds appropriate 
# include directories, compile definitions, the position-independent-code flag, 
# and links to the qtmain.lib library on Windows.
# *****************************************************************************

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

# Make use of c++11
IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std=c++14")
ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
ENDIF(MSVC)

PROJECT(behave)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/behave)      

# optional test executable
OPTION(TEST_BEHAVE "Enable Testing" ON)
OPTION(TEST_MORTALITY "Enable Mortality Testing" ON)

# optional stand-alone executables
OPTION(EXAMPLE_APP "Example client application" ON)
OPTION(RAWS_BATCH "Enable Behave RAWS Data Batch Reader" OFF)
OPTION(COMPUTE_SPOT_PILE "Build pile spot fire distance calculator" OFF)
OPTION(COMPUTE_SPOT_SURFACE "Build surface spot fire distance calculator" OFF)
OPTION(COMPUTE_SPOT_TORCHING_TREES "Build torching tree spot fire distance calculator" OFF)

IF(TEST_BEHAVE)
    ADD_DEFINITIONS(-DTEST_BEHAVE)
ENDIF()

IF(TEST_MORTALITY)
    ADD_DEFINITIONS(-DTEST_MORTALITY)
ENDIF()

IF(EXAMPLE_APP)
    ADD_DEFINITIONS(-DEXAMPLE_APP)
ENDIF()

IF(RAWS_BATCH)
    ADD_DEFINITIONS(-DRAWS_BATCH)
ENDIF()

IF(COMPUTE_SPOT_PILE)
    ADD_DEFINITIONS(-DCOMPUTE_SPOT_PILE)
ENDIF()

IF(COMPUTE_SPOT_SURFACE)
    ADD_DEFINITIONS(-DCOMPUTE_SPOT_SURFACE)
ENDIF()

IF(COMPUTE_SPOT_TORCHING_TREES)
    ADD_DEFINITIONS(-DCOMPUTE_SPOT_TORCHING_TREES)
ENDIF()

SET(SOURCE
    src/behave/behaveRun.cpp
    src/behave/behaveUnits.cpp
    src/behave/canopy_coefficient_table.cpp
    src/behave/Contain.cpp
    src/behave/ContainAdapter.cpp
    src/behave/ContainForce.cpp
    src/behave/ContainForceAdapter.cpp
    src/behave/ContainResource.cpp
    src/behave/ContainSim.cpp
    src/behave/crown.cpp
    src/behave/crownInputs.cpp
    src/behave/fireSize.cpp
    src/behave/fuelModels.cpp
    src/behave/ignite.cpp
    src/behave/igniteInputs.cpp
    src/behave/moistureScenarios.cpp
    src/behave/mortality.cpp
    src/behave/mortality_equation_table.cpp
    src/behave/mortality_inputs.cpp
    src/behave/newext.cpp
    src/behave/palmettoGallberry.cpp
    src/behave/randfuel.cpp
    src/behave/randthread.cpp
    src/behave/safety.cpp
    src/behave/species_master_table.cpp
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
    src/behave/behaveUnits.h
    src/behave/canopy_coefficient_table.h
    src/behave/Contain.h
    src/behave/ContainAdapter.h
    src/behave/ContainForce.h
    src/behave/ContainForceAdapter.h
    src/behave/ContainResource.h
    src/behave/ContainSim.h
    src/behave/crown.h
    src/behave/crownInputs.h
    src/behave/fireSize.h
    src/behave/fuelModels.h
    src/behave/ignite.h
    src/behave/igniteInputs.h
    src/behave/mortality.h
    src/behave/mortality_equation_table.h
    src/behave/mortality_inputs.h
    src/behave/newext.h
    src/behave/palmettoGallberry.h
    src/behave/randfuel.h
    src/behave/randthread.h
    src/behave/safety.h
    src/behave/species_master_table.h
    src/behave/spot.h
    src/behave/spotInputs.h
    src/behave/surface.h
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

IF(TEST_BEHAVE)
        SET(BOOST_TEST_SOURCE
            src/testBehave/testBehave.cpp)
        ADD_EXECUTABLE(testBehave 
            ${SOURCE}
            ${BOOST_TEST_SOURCE}
            ${HEADERS})
        TARGET_LINK_LIBRARIES(testBehave)
ENDIF()

IF(TEST_MORTALITY)
    SET(BOOST_TEST_SOURCE
            src/testMortality/mortality_client.cpp)
    ADD_EXECUTABLE(testMortality
            ${SOURCE}
            ${BOOST_TEST_SOURCE}
            ${HEADERS})
    TARGET_LINK_LIBRARIES(testMortality)
ENDIF()

IF(EXAMPLE_APP)
    ADD_EXECUTABLE(behave 
        ${SOURCE} 
        src/behave/client.cpp 
        ${HEADERS})
ENDIF()

IF(RAWS_BATCH)
    ADD_EXECUTABLE(behave-raws-batch
        ${SOURCE}
        src/rawsBatch/behaveRawsBatch.cpp
        ${HEADERS})
ENDIF()

IF(COMPUTE_SPOT_PILE)
    ADD_EXECUTABLE(compute_spot_distance_pile
        ${SOURCE}
        src/spotDistancePile/computePileSpottingDistance.cpp
        ${HEADERS})
ENDIF()

IF(COMPUTE_SPOT_SURFACE)
    ADD_EXECUTABLE(compute_spot_distance_surface
        ${SOURCE}
        src/spotDistanceSurface/computeSurfaceSpottingDistance.cpp
        ${HEADERS})
ENDIF()

IF(COMPUTE_SPOT_TORCHING_TREES)
    ADD_EXECUTABLE(compute_spot_distance_trees
        ${SOURCE}
        src/spotDistanceTorchingTrees/computeTorchingTreesSpottingDistance.cpp
        ${HEADERS})
ENDIF()
