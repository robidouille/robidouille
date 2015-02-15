###Prerequisites###
First make sure you have cmake and git installed:

    sudo apt-get install cmake git

You might also need gcc/g++ in case it's not already installed, along with some other libraries:

    sudo apt-get install gcc g++ libx11-dev libxt-dev libxext-dev libgraphicsmagick1-dev libcv-dev libhighgui-dev


You need the raspberry pi userland libraries:

    mkdir -p ~/git/raspberrypi
    cd ~/git/raspberrypi
    git clone https://github.com/raspberrypi/userland.git
    cd userland
    ./buildme
    

### Build the raspicam library ###

    mkdir -p ~/git
    cd ~/git
    git clone https://github.com/robidouille/robidouille.git
    cd robidouille/raspicam_cv
    mkdir objs
    make

This will create:

- **libraspicamcv.a**: A static raspberry cam library which provides an opencv like interface
- **libraspicamcv.so**: A shared library of the above
- **raspicamtest**: A small test app which uses the static library. Execute with `./raspicamtest`. Press Esc to exit.

### Using the static library ###

Add the include path

    -I$(HOME)/git/robidouille/raspicam_cv
Link with the raspicamcv library

    -L$(HOME)/git/robidouille/raspicam_cv -lraspicamcv
Link with the userland libraries: 

    -L$(HOME)/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host

In your code add **#include "RaspiCamCV.h"** and replace:

- CvCapture -> RaspiCamCvCapture
- cvCreateCameraCapture -> raspiCamCvCreateCameraCapture
- cvQueryFrame -> raspiCamCvQueryFrame
- cvReleaseCapture -> raspiCamCvReleaseCapture
- cvGetCaptureProperty -> raspiCamCvGetCaptureProperty

cvSetCaptureProperty does not currently work. Use the `raspiCamCvCreateCameraCapture2` method to specify width, height, framerate, bitrate and monochrome settings.

### Using the shared library ###
Example C# can be found [here](https://github.com/neutmute/PiCamCV/blob/master/source/LibPiCamCV/PInvoke/CvInvokeRaspiCamCV.cs) which made [this](https://www.youtube.com/watch?v=MWK55A0RH0U).
 
### Credits ###
RaspiCamCV.c/h is converted from camcv_vid0.c from [Pierre Raufast](https://thinkrpi.wordpress.com/2013/05/22/opencv-and-camera-board-csi/ "Pierre Raufast"), which in turn is based on [RaspiVid.c](https://github.com/raspberrypi/userland/blob/master/host_applications/linux/apps/raspicam/RaspiVid.c).

### More info ###
- [http://www.robidouille.com](https://robidouille.wordpress.com/2013/10/19/raspberry-pi-camera-with-opencv/)
- [https://www.google.com/+Robidouille](https://www.google.com/+Robidouille)


### Troubleshooting ###
#### No rule to make target 'objs/RaspiCamControl.o' ####
If your userland library is in another directory than as specified above, you need to update the USERLAND_ROOT variable in the Makefile.

#### RaspiCamCV.c:38:16: fatal error: cv.h: No such file or directory  
You didn't apt-get the suggested libraries at the top of this file

#### (RaspiCamTest:14072): Gtk-WARNING **: cannot open display ####
You need to run raspicamtest within an X server. Type `startx` first, open a terminal window, and launch raspicamtest from there.

#### raspicamtest causes the entire raspberry pi to crash ####
Your pi power supply may not be up to the task. Capturing images in a loop makes the pi work hard.