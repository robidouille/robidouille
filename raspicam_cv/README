To compile:

First make sure you have cmake and git installed:
--------------------------------------------------
sudo apt-get install cmake git

You might also need gcc/g++ in case it's not already installed, along with some other libraries:
sudo apt-get install gcc g++ libx11-dev libxt-dev libxext-dev libgraphicsmagick1-dev libcv-dev libhighgui-dev


You need the raspberry pi userland libraries:
--------------------------------------------------
mkdir -p ~/git/raspberrypi
cd ~/git/raspberrypi
git clone https://github.com/raspberrypi/userland.git
cd userland
./buildme


To build the raspicam library:
--------------------------------------------------
mkdir -p ~/git
cd ~/git
git clone https://github.com/robidouille/robidouille.git
cd robidouille/raspicam_cv
mkdir objs
make

This will create:
-libraspicamcv.a: the raspberry cam library which provides an opencv like interface
-raspicamtest: a small test app which uses the library

To use it in your own project:
--------------------------------------------------
add the include path: -I$(HOME)/git/robidouille/raspicam_cv
link with the raspicamcv library: -L$(HOME)/git/robidouille/raspicam_cv -lraspicamcv
link with the userland libraries: -L$(HOME)/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host

In your code add a #include "RaspiCamCV.h", and replace:
-cvCreateCameraCapture -> raspiCamCvCreateCameraCapture
-cvQueryFrame -> raspiCamCvQueryFrame
-CvCapture -> RaspiCamCvCapture

Credits:
--------------------------------------------------
RaspiCamCV.c/h is the library source.
It's converted from camcv_vid0.c from Pierre Raufast.
https://thinkrpi.wordpress.com/2013/05/22/opencv-and-camera-board-csi/
https://thinkrpi.wordpress.com/

More info:
--------------------------------------------------
Website: http://www.robidouille.com
Google+: https://www.google.com/+Robidouille
