#include <csignal>

#include <opencv2/opencv.hpp>

#include "GxCamera/GxCamera.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using cv::Mat;
using cv::imshow;
using cv::waitKey;
using cv::cvtColor;

using GxCamera::Camera;
using GxCamera::CameraParam;

namespace {
    bool stop_flag = false;
}

void SigintHandler(int sig) {
    stop_flag = true;
    cout << "SIGINT received, exiting" << endl;
}

int main(){

    // Set camera parameters
    CameraParam cam_param;
    // Serial number
    cam_param.if_open_by_serialnum = true;
    cam_param.serial_num = string("KE0200080462");
    // Software trigger
    cam_param.enable_software_trigger = false;
    // Auto exposure
    cam_param.enable_auto_exposure = true;
    cam_param.auto_exposure_max = 3000.0;
    cam_param.auto_exposure_min = 1000.0;
    cam_param.exposure_time_no_auto = 2000.0;
    // Auto gain
    cam_param.enable_auto_gain = false;
    cam_param.auto_gain_max = 16.0;
    cam_param.auto_gain_min = 5.0;
    cam_param.gain_no_auto = 12.0;

    GX_STATUS status;
    Camera cam;
    status = cam.CameraInit(cam_param);
    if(status != GX_STATUS_SUCCESS){
        cerr << "CameraInit fail " << endl;
        return EXIT_FAILURE;
    }

    status = cam.CameraStreamOn();
    if(status != GX_STATUS_SUCCESS){
        cerr << "Turn on camera stream fail" << endl;
        return EXIT_FAILURE;
    }

    cout << "Current expusore time: " << cam.GetCurrentExpTime() << endl;
    cout << "Current acquisition frame rate: " << cam.GetCurrentFrameRate() << endl;

    signal(SIGINT, SigintHandler);
    Mat img;
    while (!stop_flag){
        cam.GetLatestColorImg(img);

        cvtColor(img, img, cv::COLOR_RGB2BGR);

        imshow("test", img);
        waitKey(1);
    }

    cam.CameraStreamOff();
    cam.CameraClose();
    
    return EXIT_SUCCESS;
}
