add_executable(
    show_capture_image
        ShowCaptureImage.cc
)

target_link_libraries(
    show_capture_image 
        gxcamera
)