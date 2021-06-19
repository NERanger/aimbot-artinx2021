add_library(async_serial BufferedAsyncSerial.cc AsyncSerial.cc)
target_link_libraries(
    async_serial 
        ${Boost_LIBRARIES}
)