OBJS = objs

CFLAGS_OPENCV = -I/usr/include/opencv
LDFLAGS2_OPENCV = -lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_imgproc -lpthread -lm

USERLAND_ROOT = $(HOME)/git/raspberrypi/userland
CFLAGS_PI = \
	-I$(USERLAND_ROOT)/host_applications/linux/libs/bcm_host/include \
	-I$(USERLAND_ROOT)/host_applications/linux/apps/raspicam \
	-I$(USERLAND_ROOT) \
	-I$(USERLAND_ROOT)/interface/vcos/pthreads \
	-I$(USERLAND_ROOT)/interface/vmcs_host/linux \
	-I$(USERLAND_ROOT)/interface/mmal \

LDFLAGS_PI = -L$(USERLAND_ROOT)/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host

BUILD_TYPE=debug
#BUILD_TYPE=release

CFLAGS_COMMON = -Wno-multichar -g $(CFLAGS_OPENCV) $(CFLAGS_PI) -MD

ifeq ($(BUILD_TYPE), debug)
	CFLAGS = $(CFLAGS_COMMON)
endif
ifeq ($(BUILD_TYPE), release)
	CFLAGS = $(CFLAGS_COMMON) -O3
endif

LDFLAGS = 
LDFLAGS2 = $(LDFLAGS2_OPENCV) $(LDFLAGS_PI) -lX11 -lXext -lrt -lstdc++

RASPICAMCV_OBJS = \
	$(OBJS)/RaspiCamControl.o \
	$(OBJS)/RaspiCLI.o \
	$(OBJS)/RaspiCamCV.o \

RASPICAMTEST_OBJS = \
	$(OBJS)/RaspiCamTest.o \

TARGETS = libraspicamcv.a raspicamtest libraspicamcv.so

all: $(TARGETS)

$(OBJS)/%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

$(OBJS)/%.o: $(USERLAND_ROOT)/host_applications/linux/apps/raspicam/%.c
	gcc -c $(CFLAGS) $< -o $@

libraspicamcv.a: $(RASPICAMCV_OBJS)
	ar rcs libraspicamcv.a -o $+

libraspicamcv.so: $(RASPICAMCV_OBJS)
	gcc -shared -o libraspicamcv.so $+ -Wl,-whole-archive $(LDFLAGS_PI) -lopencv_core -lopencv_highgui -Wl,-no-whole-archive

raspicamtest: $(RASPICAMTEST_OBJS) libraspicamcv.a
	gcc $(LDFLAGS) $+ $(LDFLAGS2) -L. libraspicamcv.a -o $@

clean:
	rm -f $(OBJS)/* $(TARGETS)

-include $(OBJS)/*.d
