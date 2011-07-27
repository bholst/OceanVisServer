add_subdirectory(lib)

include_directories(${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}/lib
                    ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(OCEANVISSERVER_SRCS OceanVisServer.cpp
                        ColorMapWidget.cpp
                        main.cpp)
qt4_automoc(${OCEANVISSERVER_SRCS})
add_executable(OceanVisServer ${OCEANVISSERVER_SRCS})
target_link_libraries(OceanVisServer ${QT_QTCORE_LIBRARY}
                                     ${QT_QTGUI_LIBRARY}
                                     ${QT_QTNETWORK_LIBRARY}
                                     oceanvis)

add_subdirectory(tests)
