project(gdal6s)
cmake_minimum_required(VERSION 2.6)
#find_package(Qt4 REQUIRED)

SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})
find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR})
set(LIBS ${LIBS} ${GDAL_LIBRARY})


set(gdal6s_INCLUDE Transform.h 6s.h Abstra.h AerosolConcentration.h AerosolModel.h Altitude.h AtmosModel.h common.h grass/gis.h grass/glocale.h grass/datetime.h GeomCond.h Iwave.h Gauss.h Interp.h Output.h)
set(gdal6s_SRCS Transform.cpp 6s.cpp Abstra.cpp AerosolConcentration.cpp AerosolModel.cpp Altitude.cpp AtmosModel.cpp common.cpp GeomCond.cpp Iwave.cpp gauss.cpp Interp.cpp computations.cpp Output.cpp)


include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_SOURCE_DIR}/include) # ${QT_INCLUDES})


ADD_LIBRARY( gdal6slib STATIC ${gdal6s_SRCS} ${gdal6s_INCLUDE} )
TARGET_LINK_LIBRARIES( gdal6slib  ${LIBS} )

add_executable(gdal6s gdal6s.cpp )
target_link_libraries(gdal6s ${LIBS} gdal6slib)
#target_link_libraries(gdal6s ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
 
 
