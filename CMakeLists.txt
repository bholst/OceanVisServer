project(OceanVisServer)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

macro(oceanvis_add_test TEST_NAME)
    set(${TEST_NAME}_SRCS ${TEST_NAME}.cpp ${ARGN})

    qt4_generate_moc(${TEST_NAME}.cpp ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.moc)
    include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
    set(${TEST_NAME}_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.moc ${${TEST_NAME}_SRCS})

    add_executable(${TEST_NAME} ${${TEST_NAME}_SRCS})

    target_link_libraries(${TEST_NAME} ${QT_QTCORE_LIBRARY}
                                       ${QT_QTTEST_LIBRARY}
                                       oceanvis)
    add_test( ${TEST_NAME} ${TEST_NAME} )
endmacro( oceanvis_add_test TEST_NAME )

add_subdirectory(src)