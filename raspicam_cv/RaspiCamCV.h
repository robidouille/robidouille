#ifndef __RaspiCamCV__
#define __RaspiCamCV__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _RASPIVID_STATE RASPIVID_STATE;

typedef struct
{
	int width;              
	int height;             
	int bitrate;            
	int framerate;          
	int monochrome;			
} RASPIVID_CONFIG;

typedef struct {
	RASPIVID_STATE * pState;
} RaspiCamCvCapture;

typedef struct _IplImage IplImage;

// Mirror of CV_CAP_PROP_* properties in opencv's highgui_c.h
enum
{
    RPI_CAP_PROP_FRAME_WIDTH    =3,
    RPI_CAP_PROP_FRAME_HEIGHT   =4,
    RPI_CAP_PROP_FPS            =5,
    RPI_CAP_PROP_MONOCHROME		=19,
    RPI_CAP_PROP_BITRATE		=37   // no natural mapping here - used CV_CAP_PROP_SETTINGS
};

RaspiCamCvCapture * raspiCamCvCreateCameraCapture2(int index, RASPIVID_CONFIG* config);
RaspiCamCvCapture * raspiCamCvCreateCameraCapture(int index);
void raspiCamCvReleaseCapture(RaspiCamCvCapture ** capture);
double raspiCamCvGetCaptureProperty(RaspiCamCvCapture * capture, int property_id);
int raspiCamCvSetCaptureProperty(RaspiCamCvCapture * capture, int property_id, double value);
IplImage * raspiCamCvQueryFrame(RaspiCamCvCapture * capture);

#ifdef __cplusplus
}
#endif

#endif
