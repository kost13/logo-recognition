
set(TEST_TARGET logo_recognition_tests)

set(SRC_LIST 
    test.cc
)

add_executable(${TEST_TARGET} ${SRC_LIST})
target_compile_features(${TEST_TARGET} PRIVATE cxx_std_11)
target_link_libraries(${TEST_TARGET} PRIVATE logo_recognition_lib Catch2::Catch)
add_test(NAME testlibtest COMMAND ${TEST_TARGET}) 
