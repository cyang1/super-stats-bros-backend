add_executable(superstatsbros main.cpp)
target_link_libraries(superstatsbros imgproc ${Tesseract_LIBRARIES} ${OpenCV_LIBRARIES})

install(TARGETS superstatsbros DESTINATION ${PROJECT_SOURCE_DIR}/..)
