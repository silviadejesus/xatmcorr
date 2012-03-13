project(XAtmCorr)
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE Debug)

#find_package(Qt4 REQUIRED)

#set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${AtmCorr_SOURCE_DIR}")#"${CMAKE_CURRENT_SOURCE_DIR}")
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})
find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/i.atcorr)
set(LIBS ${LIBS} ${GDAL_LIBRARY})
add_subdirectory(i.atcorr)


#include_directories(/usr/include/gdal)
#link_libraries(/usr/lib)

find_package(Qt3 REQUIRED) # find and setup Qt3 for this project
add_definitions(${QT_DEFINITIONS})
# tell cmake where to search for Qt/KDE headers:



SET(project_UI ui/batchDialog.ui )

set(tinyXMLSRCS tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)

set(Ui_Implem include/batchcor.h)

#HEADERS just so that qtcreator can find the headers also
set(AtmCorr_HEADERS include/auxtable.h include/sensorparam.h include/stl.h  include/batchcor.h include/dntotoa.h)

#SOURCE
set(AtmCorr_SRCS src/auxtable.cpp src/main.cpp  src/sensorparam.cpp src/stl.cpp  src/batchcor.cpp src/dntotoa.cpp)


# Make ui files from the designer to header files
QT_WRAP_UI(XAtmCorr project_UI_HDRS project_UI_SRCS ${project_UI})
QT_WRAP_CPP(XAtmCorr project_UI_MOC ${Ui_Implem} )

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 

include_directories( ${CMAKE_CURRENT_BINARY_DIR} i.atcor ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml ${CMAKE_CURRENT_SOURCE_DIR}/include) # ${QT_INCLUDES})
include_directories(${QT_INCLUDE_DIR})


#kde3_automoc(AtmCorr_SRCS)

add_executable(XAtmCorr ${AtmCorr_SRCS} ${tinyXMLSRCS}  ${project_UI_MOC} ${project_UI_SRCS} ${AtmCorr_HEADERS})
target_link_libraries(XAtmCorr ${LIBS} ${QT_LIBRARIES} gdal6slib)
#target_link_libraries(AtmCorr ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
