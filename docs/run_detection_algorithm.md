# Run detection algorithm

Now we need to run the detection algorithm on the acquired camera image. A functional sample is provided [here](../sample/TestDetection.cc).

The algorithm we used is from this [repo](https://github.com/QunShanHe/JLURoboVision). Thanks for the open-source code from TARS-GO team in Jilin University.

First we need to include necessary header files for detection algorithm.

```cpp
#include "Aimbot/General.h"
#include "Aimbot/Armor.h"
#include "Aimbot/AngleSolver.h"
```

Then we need two objects of type `ArmorDetector` and `AngleSolver` and config their parameters. **Note the parameters of `loadSVM` and `setCameraParam` are hard coded in the sample. Remember to change them.**

```cpp
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
```

Then we need a loop for detection.

```cpp
do{
    detector.setEnemyColor(kEnemyColor); //here set enemy color

    // Acquire image
    Mat img;
    cam.GetLatestColorImg(img);
    cvtColor(img, img, cv::COLOR_RGB2BGR);

    // Pass image to detector
    detector.setImg(img);
    
    // Run detector
    detector.run(img);

    // Extract detetion info
    double yaw=0,pitch=0,distance=0;
    Point2f centerPoint;
    if(detector.isFoundArmor()){
        vector<Point2f> contourPoints;
        ArmorType type;
        detector.getTargetInfo(contourPoints, centerPoint, type);
        angleSolver.getAngle(contourPoints,centerPoint,SMALL_ARMOR,yaw,pitch,distance);
    }

    // Check if found armor
    if(detector.isFoundArmor()){
        printf("Found Target! Center(%d,%d)\n",centerPoint.x,centerPoint.y);
        cout << "Yaw: " << yaw << "Pitch: " << pitch 
             << "Distance: " << distance <<endl;
    }
}while (1);
```

