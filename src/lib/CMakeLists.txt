include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
set(OCEANVISSERVER_LIB_SRCS MapGeometry.cpp
                            MapGeometryParser.cpp
                            DataLayer.cpp
                            DimensionSubset.cpp
                            DimensionTrim.cpp
                            DimensionSlice.cpp
                            BadDimensionTypeException.cpp
                            BadDimensionString.cpp
                            CoordinateAxis.cpp
                            Dimension.cpp
                            GridCoverage.cpp
                            ConfigurationParser.cpp
                            RequestBase.cpp
                            GetCoverage.cpp
                            GetMap.cpp
                            RequestParser.cpp
                            ResponseWriter.cpp
                            BoundingBox.cpp)
qt4_automoc(${OCEANVISSERVER_LIB_SRCS})
add_library(oceanvis ${OCEANVISSERVER_LIB_SRCS})
target_link_libraries(oceanvis ${QT_QTCORE_LIBRARY})
