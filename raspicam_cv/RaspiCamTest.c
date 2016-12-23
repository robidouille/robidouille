/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

*/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <unistd.h>
#include "RaspiCamCV.h"

int main(int argc, char *argv[ ]){

	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
	config->width=320;
	config->height=240;
	config->bitrate=0;	// zero: leave as default
	config->framerate=0;
	config->monochrome=0;

	int opt;

	while ((opt = getopt(argc, argv, "lxm")) != -1)
	{
		switch (opt)
		{
			case 'l':					// large
				config->width = 640;
				config->height = 480;
				break;
			case 'x':	   				// extra large
				config->width = 960;
				config->height = 720;
				break;
			case 'm':					// monochrome
				config->monochrome = 1;
				break;
			default:
				fprintf(stderr, "Usage: %s [-x] [-l] [-m] \n", argv[0], opt);
				fprintf(stderr, "-l: Large mode\n");
				fprintf(stderr, "-x: Extra large mode\n");
				fprintf(stderr, "-l: Monochrome mode\n");
				exit(EXIT_FAILURE);
		}
	}

	/*
	Could also use hard coded defaults method: raspiCamCvCreateCameraCapture(0)
	*/
    RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config); 
	free(config);
	
	CvFont font;
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);

	cvNamedWindow("RaspiCamTest", 1);
	int exit =0;
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
		
		sprintf(text, "Press ESC to exit");
		cvPutText (image, text, cvPoint(05, 80), &font, cvScalar(255, 255, 0, 0));
		
		cvShowImage("RaspiCamTest", image);
		
		char key = cvWaitKey(10);
		
		switch(key)	
		{
			case 27:		// Esc to exit
				exit = 1;
				break;
			case 60:		// < (less than)
				raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FPS, 25);	// Currently NOOP
				break;
			case 62:		// > (greater than)
				raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FPS, 30);	// Currently NOOP
				break;
		}
		
	} while (!exit);

	cvDestroyWindow("RaspiCamTest");
	raspiCamCvReleaseCapture(&capture);
	return 0;
}
