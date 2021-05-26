#include <csignal>

#include <opencv2/opencv.hpp>

#include <unistd.h>

#include"Aimbot/General.h"
#include"Aimbot/Armor.h"
#include"Aimbot/AngleSolver.h"

#include "GxCamera/GxCamera.hpp"

using GxCamera::Camera;
using GxCamera::CameraParam;

namespace{
    Color kEnemyColor = BLUE;
    int kTargetNum = 2;
    bool stop_flag = false;
}

void SigintHandler(int sig) {
    stop_flag = true;
    cout << "SIGINT received, exiting" << endl;
}

int main(int argc, char const *argv[]){
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

    // Config detection
    ArmorDetector detector;
    AngleSolver angleSolver;

    //Set armor detector prop
    detector.loadSVM("/home/artinx-007/JingYL/aimbot2021/config/123svm.xml");

    //Set angle solver prop
    angleSolver.setCameraParam("/home/artinx-007/JingYL/aimbot2021/config/camera_params.xml", 1);
    angleSolver.setArmorSize(SMALL_ARMOR,135,125);
    angleSolver.setArmorSize(BIG_ARMOR,230,127);
    angleSolver.setBulletSpeed(15000);

    double timer_start,timer_span;
    do{
        timer_start = getTickCount();
        detector.setEnemyColor(kEnemyColor); //here set enemy color

        // Acquire image
        Mat img;
        cam.GetLatestColorImg(img);
        cvtColor(img, img, cv::COLOR_RGB2BGR);

        // Pass to detector
        detector.setImg(img);
        detector.run(img);

        double yaw=0,pitch=0,distance=0;
        Point2f centerPoint;
        if(detector.isFoundArmor()){
            vector<Point2f> contourPoints;
            ArmorType type;
            detector.getTargetInfo(contourPoints, centerPoint, type);
            angleSolver.getAngle(contourPoints,centerPoint,SMALL_ARMOR,yaw,pitch,distance);
        }

        //串口在此获取信息 yaw pitch distance，同时设定目标装甲板数字
        // Serial(yaw,pitch,true,detector.isFoundArmor());
        //操作手用，实时设置目标装甲板数字
        // detector.setTargetNum(targetNum);

        timer_span=(getTickCount()-timer_start)/getTickFrequency();
        printf("Armor Detecting FPS: %f\n",1/timer_span);
        if(detector.isFoundArmor()){
            printf("Found Target! Center(%d,%d)\n",centerPoint.x,centerPoint.y);
            cout << "Yaw: " << yaw << "Pitch: " << pitch 
                 << "Distance: " << distance <<endl;
        }

#ifdef DEBUG_MODE
        //********************** DEGUG **********************//
        //装甲板检测识别调试参数是否输出
        //param:
        //		1.showSrcImg_ON,		  是否展示原图
        //		2.bool showSrcBinary_ON,  是否展示二值图
        //		3.bool showLights_ON,	  是否展示灯条图
        //		4.bool showArmors_ON,	  是否展示装甲板图
        //		5.bool textLights_ON,	  是否输出灯条信息
        //		6.bool textArmors_ON,	  是否输出装甲板信息
        //		7.bool textScores_ON	  是否输出打击度信息
        //					   1  2  3  4  5  6  7
        detector.showDebugInfo(1, 1, 1, 1, 0, 0, 0);

        if(detector.isFoundArmor())
        {
            //角度解算调试参数是否输出
            //param:
            //		1.showCurrentResult,	  是否展示当前解算结果
            //		2.bool showTVec,          是否展示目标坐标
            //		3.bool showP4P,           是否展示P4P算法计算结果
            //		4.bool showPinHole,       是否展示PinHole算法计算结果
            //		5.bool showCompensation,  是否输出补偿结果
            //		6.bool showCameraParams	  是否输出相机参数
            //					      1  2  3  4  5  6
            angleSolver.showDebugInfo(1, 0, 0, 0, 0, 0);
        }

        char chKey = waitKey(1);
        switch (chKey) {
        case '1':
            kTargetNum = 1;
            break;
        case '2':
            kTargetNum = 2;
            break;
        case '3':
            kTargetNum = 3;
            break;
        case 'b':
        case 'B':
            kEnemyColor = BLUE;
            break;
        case 'r':
        case 'R':
            kEnemyColor = RED;
            break;
        case 'q':
        case 'Q':
        case 27:
            stop_flag = true;
            break;
        default:
            break;
        }

#endif

    } while (!stop_flag);
    
    cam.CameraStreamOff();
    cam.CameraClose();

    return EXIT_SUCCESS;
}
