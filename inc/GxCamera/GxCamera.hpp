/**
 * @file    GxCamera.hpp
 * @brief   Class declaration for DaHeng usb3.0 camera
 * @author  Jing Yonglin
 * @mail:   11712605@mail.sustech.edu.cn
 *          yonglinjing7@gmail.com
*/

#ifndef _GXCAMERA_HPP_
#define _GXCAMERA_HPP_

#include <opencv2/opencv.hpp>

#include "GxIAPI.h"
#include "DxImageProc.h"

namespace GxCamera {

enum class ImageFormatConvertStatus;
class GxException;
struct CameraParam;

class Camera{

public:

    GX_STATUS CameraInit(const CameraParam &param);
    GX_STATUS CameraInit(bool enable_soft_trigger);
    GX_STATUS CameraInit(char *serial_num, bool enable_soft_trigger);

    GX_STATUS SetExposureTime(double exposure_time);
    GX_STATUS SetFrameRate(double frame_rate);
    GX_STATUS SetWhileBalance();

    double GetCurrentExpTime();
    double GetCurrentFrameRate();

    GX_STATUS CameraStreamOn();
    GX_STATUS CameraStreamOff();

    GX_STATUS CameraClose();

    GX_STATUS SendSoftTrigger();
    GX_STATUS GetLatestColorImg(cv::Mat &color_img);

    static bool is_lib_init_;

private:
    GX_STATUS CameraOpenDevice();  // Init as the first camera in device list
    GX_STATUS CameraOpenDevice(char *serial_num);  // Init with serial number

    GX_STATUS PrintCameraInfo();

    GX_STATUS CheckBasicProperties();
    GX_STATUS SetWorkingProperties();
    GX_STATUS SetWorkingProperties(const CameraParam &params);

    GX_STATUS CameraCloseDevice();

    ImageFormatConvertStatus ImageFormatConvert(PGX_FRAME_BUFFER &frame_buffer);

    void AllocImgBufferMem();
    void DeallocImgBufferMem();

    bool is_soft_trigger_mode_ = false;

    int64_t color_filter_ = GX_COLOR_FILTER_NONE;
    int64_t payload_size_ = 0;

    // PGX_FRAME_BUFFER frame_buffer_ = NULL;

    unsigned char* raw8_img_buff_ = NULL;
    unsigned char* rgb24_img_buff_ = NULL;

    GX_DEV_HANDLE device_handle_ = NULL;
};

struct CameraParam{
    // Serial number option
    bool if_open_by_serialnum = false;  // If use serial number for opening device
    std::string serial_num = "";        // Serial number

    // Software trigger option
    bool enable_software_trigger = true;
    
    // Auto exposure option
    bool enable_auto_exposure = false;
    // Unit: us
    double exposure_time_no_auto = 2000.0;// Expouser time used when auto exposure is disabled
    double auto_exposure_min = 500.0;     // Exposure time lower limit in auto exposure
    double auto_exposure_max = 10000.0;   // Exposure time upeer limit in auto exposure

    // Auto gain option
    bool enable_auto_gain = true;
    double gain_no_auto = 6;     // Gain used when auto gain is disabled (<=16)
    double auto_gain_min = 5;    // Gain lower limit in auto gain
    double auto_gain_max = 10;   // Gain upper limit in auto gain

    // White balance option
    bool enable_auto_white_balance = true;
    int64_t expected_gray_value = 200; 
    GX_AWB_LAMP_HOUSE_ENTRY auto_wb_light_src = GX_AWB_LAMP_HOUSE_ADAPTIVE;  // Auto white balance light source
};

enum class ImageFormatConvertStatus{
    kImageFormatConvertFail = -1,
    kImageFormatConvertSuccess = 0
};

// Used to handle driver status exception
class GxException : public std::exception {

public:
    GxException(GX_STATUS error_status) : error_status_(error_status){};
    virtual const char* what() const throw();

private:
    const char* GetErrorString(GX_STATUS emErrorStatus) const;

    GX_STATUS error_status_;

};

}

#endif
