add_library(stb_image STATIC stb_image.cpp stb_image_write.cpp)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_LIST_DIR})