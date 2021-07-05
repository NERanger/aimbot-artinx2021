# Camera driver installation

This repo is using Daheng Galaxy USB3.0 camera, the product list can be found [here](https://www.daheng-imaging.com/en/news/index.aspx?nodeid=247). 

The official download page can be found [here](https://www.daheng-imaging.com/en/member/login.aspx).

## Quick setup

A copy of official driver is provided, you can click [here](https://github.com/Artinx-Algorithm-Group/Galaxy-USB3.0-API/releases/download/officialLib/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.1911.9122.tar.gz) to download it.

The following shell script gives a way to quickly setup camera driver for Galaxy USB3.0 cameras, which are used in this repo.

```shell
# Download the zipped driver file
wget https://github.com/Artinx-Algorithm-Group/Galaxy-USB3.0-API/releases/download/officialLib/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.1911.9122.tar.gz
# Unzip the file
tar zxvf Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.1911.9122.tar.gz
# Driver installation
cd Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.1911.9122
sudo ./Galaxy_camera.run
```

## Troubleshooting

* After installing the driver, remember to re-plug the camera to make the driver functional.

* You may want to re-install the driver if the camera is not behaving as intended.