#ifndef __RaspiCamCV__
#define __RaspiCamCV__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _RASPIVID_STATE RASPIVID_STATE;

typedef struct {
	RASPIVID_STATE * pState;
} RaspiCamCvCapture;

typedef struct _IplImage IplImage;

enum
{
	RPI_CAP_PROP_FRAME_WIDTH  = 0,
	RPI_CAP_PROP_FRAME_HEIGHT = 1,
	RPI_CAP_PROP_FPS          = 2
};

RaspiCamCvCapture * raspiCamCvCreateCameraCapture(int index);
void raspiCamCvReleaseCapture(RaspiCamCvCapture ** capture);
/**
 * Set a property in the RaspiCamCvCapture.
 *
 * capture - Pointer to the capture to modify
 * property_id - Property to change. Possible property ids:
 * 	RPI_CAP_PROP_FRAME_WIDTH - Width of the frames in the stream.
 * 	RPI_CAP_PROP_FRAME_HEIGHT - Height of the frames in the stream.
 * value - Value to set the property to
 */
void raspiCamCvSetCaptureProperty(RaspiCamCvCapture * capture, int property_id,
		double value);
IplImage * raspiCamCvQueryFrame(RaspiCamCvCapture * capture);

#ifdef __cplusplus
}
#endif

#endif
