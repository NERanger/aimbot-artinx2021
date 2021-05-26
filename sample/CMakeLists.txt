add_executable(
    show_capture_image
        ShowCaptureImage.cc
)

target_link_libraries(
    show_capture_image 
        gxcamera
)

add_executable(
    test_detection
        TestDetection.cc
)

target_link_libraries(
    test_detection 
        gxcamera
        aimbot
)

add_executable(
    serial_test
        SerialTest.cc
)

target_link_libraries(
    serial_test
        serial_packet_manager
        ${OpenCV_LIBS} 
)

add_executable(
    sys_all_test
        SysAllTest.cc   
)

target_link_libraries(
    sys_all_test
        serial_packet_manager
        gxcamera
        aimbot
)