
# ****************************************************************************/
# This is set to 2.8.11 to use the target_link_libraries command. In CMake 
# 2.8.11 and later versions, this command automatically adds appropriate 
# include directories, compile definitions, the position-independent-code flag, 
# and links to the qtmain.lib library on Windows.
# *****************************************************************************

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)
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
    ${PROJECT_SOURCE_DIR}/src/behave.cpp
    ${PROJECT_SOURCE_DIR}/src/fuelModels.cpp
    ${PROJECT_SOURCE_DIR}/src/newext.cpp
    ${PROJECT_SOURCE_DIR}/src/palmettoGallberry.cpp
    ${PROJECT_SOURCE_DIR}/src/randfuel.cpp
    ${PROJECT_SOURCE_DIR}/src/randthread.cpp
    ${PROJECT_SOURCE_DIR}/src/surface.cpp
    ${PROJECT_SOURCE_DIR}/src/surfaceFirePropogatingFlux.cpp
    ${PROJECT_SOURCE_DIR}/src/surfaceFireReactionIntensity.cpp
    ${PROJECT_SOURCE_DIR}/src/surfaceFuelbedIntermediates.cpp
    ${PROJECT_SOURCE_DIR}/src/surfaceInputs.cpp
    ${PROJECT_SOURCE_DIR}/src/surfaceFireSpread.cpp
    ${PROJECT_SOURCE_DIR}/src/surfaceTwoFuelModels.cpp
    ${PROJECT_SOURCE_DIR}/src/westernAspen.cpp
    ${PROJECT_SOURCE_DIR}/src/windAdjustmentFactor.cpp)

SET(HEADERS
    ${PROJECT_SOURCE_DIR}/src/behave.h
    ${PROJECT_SOURCE_DIR}/src/fuelModels.h
    ${PROJECT_SOURCE_DIR}/src/newext.h
    ${PROJECT_SOURCE_DIR}/src/palmettoGallberry.h
    ${PROJECT_SOURCE_DIR}/src/randfuel.h
    ${PROJECT_SOURCE_DIR}/src/randthread.h
    ${PROJECT_SOURCE_DIR}/src/surface.h
    ${PROJECT_SOURCE_DIR}/src/surfaceEnums.h
    ${PROJECT_SOURCE_DIR}/src/surfaceFirePropogatingFlux.h
    ${PROJECT_SOURCE_DIR}/src/surfaceFireReactionIntensity.h
    ${PROJECT_SOURCE_DIR}/src/surfaceFuelbedIntermediates.h
    ${PROJECT_SOURCE_DIR}/src/surfaceInputs.h
    ${PROJECT_SOURCE_DIR}/src/surfaceFireSpread.h
    ${PROJECT_SOURCE_DIR}/src/surfaceTwoFuelModels.h
    ${PROJECT_SOURCE_DIR}/src/westernAspen.h
    ${PROJECT_SOURCE_DIR}/src/windAdjustmentFactor.h)

ADD_EXECUTABLE(behave 
    ${SOURCE} 
    src/client.cpp 
    ${HEADERS})

IF(CANOPY)
    ADD_EXECUTABLE(canopy_flow 
        ${SOURCE} 
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopy.cpp
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopyFlow.cpp 
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopy_normal_distribution.cpp
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopy_triangle_distribution.cpp
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/massman_distribution.cpp
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/measured_distribution.cpp
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/main.cpp
        ${HEADERS}
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopy.h
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopyFlow.h
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopy_normal_distribution.h
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/canopy_triangle_distribution.h
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/massman_distribution.h
        ${PROJECT_SOURCE_DIR}/src/canopyFlow/src/measured_distribution.h)
ENDIF(CANOPY)

IF(CLI)
    ADD_EXECUTABLE(behave-cli 
        ${SOURCE}
        ${PROJECT_SOURCE_DIR}/src/cli/src/behaveCli.cpp
        ${PROJECT_SOURCE_DIR}/src/behave.h
        ${PROJECT_SOURCE_DIR}/src/fuelModels.h
        ${PROJECT_SOURCE_DIR}/src/newext.h
        ${PROJECT_SOURCE_DIR}/src/palmettoGallberry.h
        ${PROJECT_SOURCE_DIR}/src/randfuel.h
        ${PROJECT_SOURCE_DIR}/src/randthread.h
        ${PROJECT_SOURCE_DIR}/src/surface.h
        ${PROJECT_SOURCE_DIR}/src/surfaceEnums.h
        ${PROJECT_SOURCE_DIR}/src/surfaceFirePropogatingFlux.h
        ${PROJECT_SOURCE_DIR}/src/surfaceFireReactionIntensity.h
        ${PROJECT_SOURCE_DIR}/src/surfaceFuelbedIntermediates.h
        ${PROJECT_SOURCE_DIR}/src/surfaceInputs.h
        ${PROJECT_SOURCE_DIR}/src/surfaceFireSpread.h
        ${PROJECT_SOURCE_DIR}/src/surfaceTwoFuelModels.h
        ${PROJECT_SOURCE_DIR}/src/westernAspen.h
        ${PROJECT_SOURCE_DIR}/src/windAdjustmentFactor.h)
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
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.cpp
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/behave_qtree_widget.cpp
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/main.cpp)

    SET(BEHAVE_QT_HEADERS
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.h
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window_enums.h
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/behave_qtree_widget.h)

    QT5_WRAP_UI(UIS_HDRS 
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.ui)

    SOURCE_GROUP("UI Files"  FILES
        ${PROJECT_SOURCE_DIR}/src/behave_qt/src/behave_qt_main_window.ui)

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
