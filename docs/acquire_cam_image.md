# Acquire camera image

After successfully installing the camera driver, it's time to acquire image from the camera. This repo includes a simple library for acquiring camera image. The source file can be found in `src/GxCamera/` and the header file is located in `inc/GxCamera/`.

> This library has a standalone github repo [here](https://github.com/Artinx-Algorithm-Group/Galaxy-USB3.0-API), which is exactly the same as the one used in this repo.

## Provided Features

This library provides multiple features for camera control, including:

* Open camera by serial number (useful when using multiple cameras)
* Software trigger (useful when you want multiple cameras to take the picture at the same time)
* Exposure setting
  * Auto exposure time with upper and lower limit
  * Fixed exposure time
* Gain setting
  * Auto gain with upper and lower limit
  * Fixed gain

## Using the library

In this library, we config the camera with a struct

```cpp
// Camera config struct
CameraParam cam_param;
```

 all the configuration can be achieved with this struct

```cpp
// Serial number
cam_param.if_open_by_serialnum = true; // If false, will open the first camera
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
```

After setting up the config struct, simply pass it into the `CameraInit` function.

```cpp
Camera cam; // Camera object

GX_STATUS status;
status = cam.CameraInit(cam_param);
if(status != GX_STATUS_SUCCESS){
    cerr << "CameraInit fail " << endl;
    return EXIT_FAILURE;
}
```

Then, turn on the camera stream.

```cpp
status = cam.CameraStreamOn();
if(status != GX_STATUS_SUCCESS){
    cerr << "Turn on camera stream fail" << endl;
    return EXIT_FAILURE;
}
```

By default, the camera will run at max frame rate with current setting. You can get real-time exposure time and frame rate with the following code.

```cpp
cout << "Current expusore time: " << cam.GetCurrentExpTime() << endl;
cout << "Current acquisition frame rate: " << cam.GetCurrentFrameRate() << endl;
```

We use OpenCV Mat to store the image.

```cpp
cv::Mat img;
cam.GetLatestColorImg(img);
```

**Note that the acquired image is in RGB format, but the default format of OpenCV is BGR.** Therefore, to correctly display the image with `cv::imshow`, you need to convert the color format with `cv::cvtColor`.

```cpp
cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
imshow("test", img);
waitKey(1);
```

After the application exit, you can release the camera resource with the following code.

```cpp
cam.CameraStreamOff();
cam.CameraClose();
```

> An example is provided [here](https://github.com/NERanger/aimbot-artinx2021/blob/main/sample/ShowCaptureImage.cc).

