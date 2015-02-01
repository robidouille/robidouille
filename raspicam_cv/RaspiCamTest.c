/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

*/

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "RaspiCamCV.h"

int main(int argc, const char** argv){

	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
	config->width=640;
	config->height=480;
	config->bitrate=17000000;
	config->framerate=25;
	config->monochrome=1;

    RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config);
	
	//raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FRAME_WIDTH, 640);
	//raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FRAME_HEIGHT, 480);
	//raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FPS, 1);	
	
	CvFont font;
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;

	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth, 8);

	cvNamedWindow("RaspiCamTest", 1);
	do {
		IplImage* image = raspiCamCvQueryFrame(capture);
		
		char text[200];
	sprintf(
			text
			, "w=%.0f h=%.0f fps=%.0f bitrate=%.0f monochrome=%.0f"
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_FRAME_WIDTH)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_FRAME_HEIGHT)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_FPS)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_BITRATE)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_MONOCHROME)
		);
		cvPutText (image, text, cvPoint(05, 40), &font, cvScalar(255, 255, 0, 0));	
		
		cvShowImage("RaspiCamTest", image);
	} while (cvWaitKey(10) < 0);

	cvDestroyWindow("RaspiCamTest");
	raspiCamCvReleaseCapture(&capture);
	return 0;
}
