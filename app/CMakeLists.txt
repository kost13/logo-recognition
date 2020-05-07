
set(APP_NAME logo_recognition)

set(SRC_LIST
    main.cc
)

add_executable(${APP_NAME} ${SRC_LIST})
target_compile_features(${APP_NAME} PRIVATE cxx_std_11)

target_link_libraries(${APP_NAME} PRIVATE logo_recognition_lib)
