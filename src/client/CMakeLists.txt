include_directories(${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}/../lib
                    ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set(OCEANVISCLIENT_UI DimensionSliders.ui)
set(OCEANVISCLIENT_SRCS ColorMapWidget.cpp
                        CoverageComboBox.cpp
                        DimensionSliders.cpp
                        MainWindow.cpp
                        MapWidget.cpp
                        main.cpp)
qt4_wrap_ui(OCEANVISCLIENT_SRCS ${OCEANVISCLIENT_UI})
qt4_automoc(${OCEANVISCLIENT_SRCS})

add_executable(OceanVisClient ${OCEANVISCLIENT_SRCS})
target_link_libraries(OceanVisClient ${QT_QTCORE_LIBRARY}
                                     ${QT_QTGUI_LIBRARY}
                                     ${QT_QTNETWORK_LIBRARY}
                                     oceanvis)

