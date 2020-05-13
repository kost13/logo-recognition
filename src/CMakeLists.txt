
set(RECOGNITION_LIB_NAME logo_recognition_lib)

set(SRC_LIST
    LogoRecognizer.cc
    LogoRecognizer.h
    Image.cc
    Image.h
    Segment.cc
    Segment.h
    Colors.cc
    Colors.h
    Parameters.h
)

add_library(${RECOGNITION_LIB_NAME} ${SRC_LIST})

target_include_directories(${RECOGNITION_LIB_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS}
                                                          ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${RECOGNITION_LIB_NAME} PUBLIC ${OpenCV_LIBS})
target_compile_features(${RECOGNITION_LIB_NAME} PUBLIC cxx_std_11)
