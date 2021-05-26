add_library(
    aimbot
        AngleSolver.cc
        ArmorBox.cc
        ArmorDetector.cc
        ArmorNumClassifier.cc
        findLights.cc
        LightBar.cc
        matchArmors.cc)

target_link_libraries(
    aimbot 
        ${OpenCV_LIBS}
)