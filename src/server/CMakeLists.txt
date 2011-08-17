include_directories(${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}/../lib
                    ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set(OCEANVISSERVER_SRCS OceanVisServer.cpp
                        main.cpp)
qt4_automoc(${OCEANVISSERVER_SRCS})
add_executable(oceanvisserver ${OCEANVISSERVER_SRCS})
target_link_libraries(oceanvisserver ${QT_QTCORE_LIBRARY}
                                     ${QT_QTGUI_LIBRARY}
                                     ${QT_QTNETWORK_LIBRARY}
                                     oceanvis)