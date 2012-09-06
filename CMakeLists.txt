project(XAtmCorr)
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE Debug)

find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/i.atcorr)
set(LIBS ${LIBS} ${GDAL_LIBRARY})
add_subdirectory(i.atcorr)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# this command will generate rules that will run rcc on all files from SAMPLE_RCS
# in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
#QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )




SET(project_UI ui/batchDialog.ui )

set(tinyXMLSRCS tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)

set(Ui_Implem include/batchcor.h)

#HEADERS just so that qtcreator can find the headers also
set(AtmCorr_HEADERS include/auxtable.h include/sensorparam.h include/stl.h  include/batchcor.h include/dntotoa.h)

#SOURCE
set(AtmCorr_SRCS src/auxtable.cpp src/main.cpp  src/sensorparam.cpp src/stl.cpp  src/batchcor.cpp src/dntotoa.cpp resources/resources.res)


# Make ui files from the designer to header files
QT4_WRAP_UI(project_UI_HDRS ${project_UI})
QT4_WRAP_CPP(project_UI_MOC ${Ui_Implem})

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 

include_directories( ${CMAKE_CURRENT_BINARY_DIR} i.atcorr ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml ${CMAKE_CURRENT_SOURCE_DIR}/include) # ${QT_INCLUDES})
include_directories(${QT_INCLUDE_DIR})


#kde3_automoc(AtmCorr_SRCS)

add_executable(XAtmCorr ${AtmCorr_SRCS} ${tinyXMLSRCS}  ${project_UI_MOC} ${project_UI_HDRS} ${AtmCorr_HEADERS} )
target_link_libraries(XAtmCorr ${LIBS} ${QT_LIBRARIES} gdal6slib)
#target_link_libraries(AtmCorr ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
