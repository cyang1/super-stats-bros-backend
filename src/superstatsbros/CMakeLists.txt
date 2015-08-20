add_executable(superstatsbros main.cpp localization_params.cpp image_util.cpp)
target_link_libraries(superstatsbros ${OpenCV_LIBRARIES})

install(TARGETS superstatsbros DESTINATION ${PROJECT_SOURCE_DIR}/..)
