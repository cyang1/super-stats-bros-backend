add_executable(superstatsbros main.cpp)
target_link_libraries(superstatsbros imgproc ${Tesseract_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(tess_ocr_example tess_ocr_example.cpp)
target_link_libraries(tess_ocr_example imgproc ${Tesseract_LIBRARIES} ${OpenCV_LIBRARIES})

install(TARGETS superstatsbros tess_ocr_example DESTINATION ${PROJECT_SOURCE_DIR}/..)
