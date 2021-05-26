add_library(gxcamera GxCamera.cc)
target_link_libraries(
    gxcamera 
        ${OpenCV_LIBS}
        libgxiapi.so  # /usr/lib/libgxiapi.so
)