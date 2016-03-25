
# ****************************************************************************/
# This is set to 2.8.11 to use the target_link_libraries command. In CMake 
# 2.8.11 and later versions, this command automatically adds appropriate 
# include directories, compile definitions, the position-independent-code flag, 
# and links to the qtmain.lib library on Windows.
# *****************************************************************************

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(behave)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src
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

ADD_EXECUTABLE(behave 
    ${SOURCE} 
    src/client.cpp 
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
