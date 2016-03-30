
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
                    ${CMAKE_SOURCE_DIR}/src/cli)      

OPTION(CLI "Enable CLI boost parser" OFF)
IF(CLI)
    ADD_DEFINITIONS(-DCLI)
ENDIF(CLI)

OPTION(GUI "Enable Qt Gui" OFF)
IF(GUI)
    ADD_DEFINITIONS(-DGUI)
ENDIF(GUI)

OPTION(CANOPY "Enable Canopy Flow Test" OFF)
IF(CANOPY)
    ADD_DEFINITIONS(-DCANOPY)
ENDIF(CANOPY)

SET(SOURCE
    src/behave/behave.cpp
    src/behave/fuelModels.cpp
    src/behave/newext.cpp
    src/behave/palmettoGallberry.cpp
    src/behave/randfuel.cpp
    src/behave/randthread.cpp
    src/behave/surface.cpp
    src/behave/surfaceFireReactionIntensity.cpp
    src/behave/surfaceFuelbedIntermediates.cpp
    src/behave/surfaceInputs.cpp
    src/behave/surfaceFireSpread.cpp
    src/behave/surfaceTwoFuelModels.cpp
    src/behave/westernAspen.cpp
    src/behave/windAdjustmentFactor.cpp)

SET(HEADERS
    src/behave/behave.h
    src/behave/fuelModels.h
    src/behave/newext.h
    src/behave/palmettoGallberry.h
    src/behave/randfuel.h
    src/behave/randthread.h
    src/behave/surface.h
    src/behave/surfaceEnums.h
    src/behave/surfaceFireReactionIntensity.h
    src/behave/surfaceFuelbedIntermediates.h
    src/behave/surfaceInputs.h
    src/behave/surfaceFireSpread.h
    src/behave/surfaceTwoFuelModels.h
    src/behave/westernAspen.h
    src/behave/windAdjustmentFactor.h)

ADD_EXECUTABLE(behave 
    ${SOURCE} 
    src/behave/client.cpp 
    ${HEADERS})

IF(CANOPY)
    ADD_EXECUTABLE(canopy_flow 
        ${SOURCE} 
        src/canopyFlow/canopy.cpp
        src/canopyFlow/canopyFlow.cpp 
        src/canopyFlow/canopy_normal_distribution.cpp
        src/canopyFlow/canopy_triangle_distribution.cpp
        src/canopyFlow/massman_distribution.cpp
        src/canopyFlow/measured_distribution.cpp
        src/canopyFlow/main.cpp
        ${HEADERS}
        src/canopyFlow/canopy.h
        src/canopyFlow/canopyFlow.h
        src/canopyFlow/canopy_normal_distribution.h
        src/canopyFlow/canopy_triangle_distribution.h
        src/canopyFlow/massman_distribution.h
        src/canopyFlow/measured_distribution.h)
ENDIF(CANOPY)

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

    SET(BEHAVE_QT_SOURCE
        src/behave_qt/behave_qt_main_window.cpp
        src/behave_qt/behave_qtree_widget.cpp
        src/behave_qt/main.cpp)

    SET(BEHAVE_QT_HEADERS
       src/behave_qt/behave_qt_main_window.h
        src/behave_qt/behave_qt_main_window_enums.h
        src/behave_qt/behave_qtree_widget.h)

    QT5_WRAP_UI(UIS_HDRS 
        src/behave_qt/behave_qt_main_window.ui)

    SOURCE_GROUP("UI Files"  FILES
        src/behave_qt/behave_qt_main_window.ui)

    SOURCE_GROUP("Behave Qt GUI Source Files" FILES ${BEHAVE_QT_SOURCE})

    SOURCE_GROUP("Behave Qt GUI Header Files" FILES ${BEHAVE_QT_HEADERS})

    SOURCE_GROUP("Behave Core Source Files" FILES ${SOURCE})

    SOURCE_GROUP("Behave Core Header Files" FILES ${HEADERS})

    ADD_EXECUTABLE(behave_qt
        ${SOURCE}
        ${HEADERS}
        ${BEHAVE_QT_SOURCE}
        ${BEHAVE_QT_HEADERS}
        ${UIS_HDRS})

    TARGET_LINK_LIBRARIES(behave_qt Qt5::Widgets)
ENDIF(GUI)
