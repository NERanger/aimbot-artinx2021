# Aimbot2021

Aimbot implementation for RM2021

## Features

* Armor detection with number recognition
* Asynchronized serial communication

## Prerequisites

* OpenCV (Tested with 3.4.14)

* Boost

  ```shell
  sudo apt install libboost-all-dev
  ```

## Build

```shell
mkdir build
cd build
cmake ..
make
```

After the building process is done, the generated executables will be put in `bin/` and static libraries will be put in `lib/`.

## Framework

File tree:

```text
aimbot-artinx2021/
|--config/
     |--123svm.xml (Pre-trained SVM for number recognition)
     |--camera_params.xml (Camera instrinsics and distortion parameters)
|--inc/ (Project headers)
|--sample/ (Provided application samples)
     |--CMakeLists.txt (cmake for samples)
     |--SerialTest.cc (Test serial commnication)
     |--ShowCaptureImge.cc (Test camera)
     |--SysAllTest.cc (All system integration app)
     |--TestDetection.cc (Test detection algorithm with camera images)
|--src/ (Project source code)
     |--Aimbot/ (Core detection algorithm implementation)
          |--AngleSolver.cc
          |--ArmorBox.cc
          |--ArmorDetector.cc
          |--ArmorNumClassifier.cc
          |--LightBar.cc
          |--findLights.cc
          |--matchArmors.cc
          |--CMakeLists.txt
     |--AsyncSerial/ (Asynchronized serial communication implementation)
          |--AsyncSerial.cc
          |--BufferedAsyncSerial.cc
          |--CMakeLists.txt
     |--GxCamera/ (Galaxy camera driver)
          |--GxCamera.cc
          |--CMakeLists.txt
     |--PacketManager/
     |--utils/
```

