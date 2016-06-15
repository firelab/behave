
# ****************************************************************************/
# This is set to 2.8.11 to use the target_link_libraries command. In CMake 
# 2.8.11 and later versions, this command automatically adds appropriate 
# include directories, compile definitions, the position-independent-code flag, 
# and links to the qtmain.lib library on Windows.
# *****************************************************************************
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(behave)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/behave
                    ${CMAKE_SOURCE_DIR}/src/behave_qt
                    ${CMAKE_SOURCE_DIR}/src/canopy_flow
                    ${CMAKE_SOURCE_DIR}/src/cli
                    ${CMAKE_SOURCE_DIR}/src/qcustomplot)      

OPTION(CLI "Enable CLI boost parser" OFF)
IF(CLI)
    ADD_DEFINITIONS(-DCLI)
ENDIF(CLI)

OPTION(GUI "Enable Qt Gui" OFF)
IF(GUI)
    ADD_DEFINITIONS(-DGUI)
ENDIF(GUI)

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
    src/behave/crown.cpp
    src/behave/crownInputs.cpp
    src/behave/fuelModelSet.cpp
    src/behave/newext.cpp
    src/behave/palmettoGallberry.cpp
    src/behave/randfuel.cpp
    src/behave/randthread.cpp
    src/behave/spot.cpp
    src/behave/surface.cpp
    src/behave/surfaceFireReactionIntensity.cpp
    src/behave/surfaceFuelbedIntermediates.cpp
    src/behave/surfaceInputs.cpp
    src/behave/surfaceFireSpread.cpp
    src/behave/surfaceTwoFuelModels.cpp
    src/behave/westernAspen.cpp
    src/behave/windAdjustmentFactor.cpp
    src/behave/windSpeedUtility.cpp)

SET(HEADERS
    src/behave/behaveRun.h
    src/behave/behaveVector.h
    src/behave/crown.h
    src/behave/crownEnums.h
    src/behave/crownInputs.h
    src/behave/fuelModelSet.h
    src/behave/newext.h
    src/behave/palmettoGallberry.h
    src/behave/randfuel.h
    src/behave/randthread.h
    src/behave/spot.h
    src/behave/surface.h
    src/behave/surfaceEnums.h
    src/behave/surfaceFireReactionIntensity.h
    src/behave/surfaceFuelbedIntermediates.h
    src/behave/surfaceInputs.h
    src/behave/surfaceFireSpread.h
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

IF(CLI)
    ADD_EXECUTABLE(behave-cli 
        ${SOURCE}
        src/cli/behaveCli.cpp
        ${HEADERS})
ENDIF(CLI)

IF(GUI)
    # Set up Qt
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)

    SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

    SET(CMAKE_AUTOMOC ON)
    
    FIND_PACKAGE(Qt5Core REQUIRED)

    GET_TARGET_PROPERTY(QtCore_location Qt5::Core LOCATION)

    # Find the QtWidgets library
    FIND_PACKAGE(Qt5Widgets REQUIRED)

    FIND_PACKAGE(Qt5PrintSupport REQUIRED)
    
    SET(BEHAVE_QT_SOURCE
        src/behave_qt/behave_qt_main_window.cpp
        src/behave_qt/behave_qtree_widget.cpp
        src/behave_qt/main.cpp
        src/behave_qt/qcustomplotTest.cpp)

    SET(BEHAVE_QT_HEADERS
        src/behave_qt/behave_qt_main_window.h
        src/behave_qt/behave_qt_main_window_enums.h
        src/behave_qt/behave_qtree_widget.h
        src/behave_qt/qcustomplotTest.h)

    SET(QCUSTOMPLOT
        src/qcustomplot/qcustomplot.h
        src/qcustomplot/qcustomplot.cpp)

    QT5_WRAP_UI(UIS_HDRS 
        src/behave_qt/behave_qt_main_window.ui
        src/behave_qt/qcustomplot_test.ui)

    SOURCE_GROUP("UI Files"  FILES
        src/behave_qt/behave_qt_main_window.ui
        src/behave_qt/qcustomplot_test.ui)

    SOURCE_GROUP("Behave Qt GUI Source Files" FILES ${BEHAVE_QT_SOURCE})

    SOURCE_GROUP("Behave Qt GUI Header Files" FILES ${BEHAVE_QT_HEADERS})

    SOURCE_GROUP("qcustomplot Files" FILES ${QCUSTOMPLOT})

    ADD_EXECUTABLE(behave_qt
        ${SOURCE}
        ${HEADERS}
        ${BEHAVE_QT_SOURCE}
        ${BEHAVE_QT_HEADERS}
        ${QCUSTOMPLOT}
        ${UIS_HDRS})

    TARGET_LINK_LIBRARIES(behave_qt Qt5::Widgets Qt5::PrintSupport)
ENDIF(GUI)
