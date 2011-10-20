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
# tell cmake where to search for Qt/KDE headers:



SET(project_UI ui/batchDialog.ui )

set(tinyXMLSRCS tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)

set(Ui_Implem include/batchcor.h)

set(AtmCorr_SRCS src/auxtable.cpp src/main.cpp  src/sensorparam.cpp src/stl.cpp  src/batchcor.cpp)


# Make ui files from the designer to header files
QT_WRAP_UI(AtmCorr project_UI_HDRS project_UI_SRCS ${project_UI})
QT_WRAP_CPP(AtmCorr project_UI_CPP ${Ui_Implem} )

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 

include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml ${CMAKE_CURRENT_SOURCE_DIR}/include) # ${QT_INCLUDES})
include_directories(${QT_INCLUDE_DIR})


#kde3_automoc(AtmCorr_SRCS)

add_executable(AtmCorr ${AtmCorr_SRCS} ${tinyXMLSRCS}  ${project_UI_CPP} ${project_UI_SRCS})
target_link_libraries(AtmCorr ${LIBS} ${QT_LIBRARIES})
#target_link_libraries(AtmCorr ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
