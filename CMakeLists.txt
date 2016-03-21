
# ****************************************************************************/
# This is set to 2.8.11 to use the target_link_libraries command. In CMake 
# 2.8.11 and later versions, this command automatically adds appropriate 
# include directories, compile definitions, the position-independent-code flag, 
# and links to the qtmain.lib library on Windows.
# *****************************************************************************

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

PROJECT(behave)

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
    ${CMAKE_SOURCE_DIR}/src/behave.cpp
    ${CMAKE_SOURCE_DIR}/src/fuelModels.cpp
    ${CMAKE_SOURCE_DIR}/src/newext.cpp
    ${CMAKE_SOURCE_DIR}/src/palmettoGallberry.cpp
    ${CMAKE_SOURCE_DIR}/src/randfuel.cpp
    ${CMAKE_SOURCE_DIR}/src/randthread.cpp
    ${CMAKE_SOURCE_DIR}/src/surface.cpp
    ${CMAKE_SOURCE_DIR}/src/surfaceFirePropogatingFlux.cpp
    ${CMAKE_SOURCE_DIR}/src/surfaceFireReactionIntensity.cpp
    ${CMAKE_SOURCE_DIR}/src/surfaceFuelbedIntermediates.cpp
    ${CMAKE_SOURCE_DIR}/src/surfaceInputs.cpp
    ${CMAKE_SOURCE_DIR}/src/surfaceFireSpread.cpp
    ${CMAKE_SOURCE_DIR}/src/surfaceTwoFuelModels.cpp
    ${CMAKE_SOURCE_DIR}/src/westernAspen.cpp
    ${CMAKE_SOURCE_DIR}/src/windAdjustmentFactor.cpp)

SET(HEADERS
    ${CMAKE_SOURCE_DIR}/src/behave.h
    ${CMAKE_SOURCE_DIR}/src/fuelModels.h
    ${CMAKE_SOURCE_DIR}/src/newext.h
    ${CMAKE_SOURCE_DIR}/src/palmettoGallberry.h
    ${CMAKE_SOURCE_DIR}/src/randfuel.h
    ${CMAKE_SOURCE_DIR}/src/randthread.h
    ${CMAKE_SOURCE_DIR}/src/surface.h
    ${CMAKE_SOURCE_DIR}/src/surfaceEnums.h
    ${CMAKE_SOURCE_DIR}/src/surfaceFirePropogatingFlux.h
    ${CMAKE_SOURCE_DIR}/src/surfaceFireReactionIntensity.h
    ${CMAKE_SOURCE_DIR}/src/surfaceFuelbedIntermediates.h
    ${CMAKE_SOURCE_DIR}/src/surfaceInputs.h
    ${CMAKE_SOURCE_DIR}/src/surfaceFireSpread.h
    ${CMAKE_SOURCE_DIR}/src/surfaceTwoFuelModels.h
    ${CMAKE_SOURCE_DIR}/src/westernAspen.h
    ${CMAKE_SOURCE_DIR}/src/windAdjustmentFactor.h)

ADD_EXECUTABLE(behave 
    ${SOURCE} 
    src/client.cpp 
    ${HEADERS})

IF(CANOPY)
    ADD_EXECUTABLE(canopy_flow 
        ${SOURCE} 
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopy.cpp
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopyFlow.cpp 
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopy_normal_distribution.cpp
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopy_triangle_distribution.cpp
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/massman_distribution.cpp
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/measured_distribution.cpp
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/main.cpp
        ${HEADERS}
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopy.h
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopyFlow.h
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopy_normal_distribution.h
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/canopy_triangle_distribution.h
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/massman_distribution.h
        ${CMAKE_SOURCE_DIR}/src/canopyFlow/src/measured_distribution.h)
ENDIF(CANOPY)

IF(CLI)
    ADD_EXECUTABLE(behave-cli 
        ${SOURCE}
        ${PROJECT_SOURCE_DIR}/src/cli/src/behaveCli.cpp
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
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.cpp
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/behave_qtree_widget.cpp
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/main.cpp)

    SET(BEHAVE_QT_HEADERS
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.h
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window_enums.h
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/behave_qtree_widget.h)

    QT5_WRAP_UI(UIS_HDRS 
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.ui)

    SOURCE_GROUP("UI Files"  FILES
        ${CMAKE_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.ui)

    SOURCE_GROUP("Behave Qt Source Files" FILES ${BEHAVE_QT_SOURCE})

    SOURCE_GROUP("Behave Qt Header Files" FILES ${BEHAVE_QT_HEADERS})

    SOURCE_GROUP("Behave Source Files" FILES ${SOURCE})

    SOURCE_GROUP("Behave Header Files" FILES ${HEADERS})

    ADD_EXECUTABLE(behave_qt
        ${SOURCE}
        ${HEADERS}
        ${BEHAVE_QT_SOURCE}
        ${BEHAVE_QT_HEADERS}
        ${UIS_HDRS})

    TARGET_LINK_LIBRARIES(behave_qt Qt5::Widgets)
ENDIF(GUI)
