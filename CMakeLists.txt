project(AtmCorr)
cmake_minimum_required(VERSION 2.6)
#find_package(Qt4 REQUIRED)

#set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${AtmCorr_SOURCE_DIR}")#"${CMAKE_CURRENT_SOURCE_DIR}")
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})
find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR})
set(LIBS ${LIBS} ${GDAL_LIBRARY})

#include_directories(/usr/include/gdal)
#link_libraries(/usr/lib)

find_package(Qt3 REQUIRED) # find and setup Qt3 for this project

add_definitions(${QT_DEFINITIONS})
#link_directories(${QT_LIB_DIR})

# tell cmake where to search for Qt/KDE headers:
include_directories(${QT_INCLUDE_DIR})



include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml ${CMAKE_CURRENT_SOURCE_DIR}/include) # ${QT_INCLUDES})

set(tinyXMLSRCS tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)

set(AtmCorr_SRCS src/auxtable.cpp src/main.cpp  src/sensorparam.cpp src/stl.cpp)
#qt4_automoc(${AtmCorr_SRCS})
add_executable(AtmCorr ${AtmCorr_SRCS} ${tinyXMLSRCS})
target_link_libraries(AtmCorr ${LIBS} ${QT_LIBRARIES})
#target_link_libraries(AtmCorr ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
