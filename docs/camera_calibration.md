# Camera Calibration

The detection algorithm requires camera intrinsics and distortion parameters to function properly. The intrinsics matrix and distortion parameter will be load from [config/camera_params.xml](../config/camera_params.xml).

```xml
<!--Camera instrinics matrix-->
<CAMERA_MATRIX_1 type_id="opencv-matrix">
  <rows>3</rows>
  <cols>3</cols>
  <dt>d</dt>
  <data>
    1298.0 0.0    639.8 
    0.0    1299.7 499.4 
    0.0    0.0    1.0
  </data>
</CAMERA_MATRIX_1>

<!--Camera distortion parameters-->
<DISTORTION_COEFF_1 type_id="opencv-matrix">
  <rows>4</rows>
  <cols>1</cols>
  <dt>d</dt>
  <data>
    -0.2222 0.2093 0.0002 -0.0019
  </data>
</DISTORTION_COEFF_1>
```

Matlab provides a [Single Camera Calibrator App](https://ww2.mathworks.cn/help/vision/ug/single-camera-calibrator-app.html) for camera calibration. (Easy to use)

**Caution: the resulted instrinics matrix from Matlab need to be transposed.** The key is that the last row of instrinics matrix is always `0.0 0.0 1.0`.