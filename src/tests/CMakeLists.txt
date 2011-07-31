include_directories(${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}/../lib
                    ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

oceanvis_add_test(TestMapGeometry)
oceanvis_add_test(TestMapGeometryParser)
oceanvis_add_test(TestDataLayer)
oceanvis_add_test(TestDimensionSubset)
oceanvis_add_test(TestDimensionTrim)
oceanvis_add_test(TestDimensionSlice)
oceanvis_add_test(TestCoordinateAxis)
oceanvis_add_test(TestGridCoverage)
