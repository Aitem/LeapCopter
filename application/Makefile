OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := lib/leapmotion/lib/x64/libLeap.so -Wl,-rpath,../lib/x64
  else
    LEAP_LIBRARY := lib/leapmotion/lib/x86/libLeap.so -Wl,-rpath,../lib/x86
  endif
else
  # OS X
  LEAP_LIBRARY := lib/leapmotion/lib/libLeap.dylib
endif


ARCHS   = -arch x86_64
CFLAGS += $(ARCHS)
CFLAGS += -mmacosx-version-min=10.6
CFLAGS_MONGOOSE=  -I./lib/serial/mongoose -pthread -g
LIBS+=	 $(ARCHS)
INCLUDES += -ISerialConnector -Ilib/serial

CFLAGS += $(INCLUDES) -O -Wall

all: clean copterController


copterController :
	$(CXX) $(CFLAGS) -g -Ilib/leapmotion/include -Iclasses -ISerialConnector main.c -o CopterController $(LIBS) ./lib/serial/*.o  $(LEAP_LIBRARY)

listner:
	$(CXX) $(CFLAGS) -Ilib/leapmotion/include -c ControllerListner/ControllerListner.c -o ControllerListner/ControllerListner.o

serial:
	$(CXX) $(CFLAGS) -c lib/serial/arduino-serial-lib.c -o lib/serial/arduino-serial-lib.o
    
connector:
	$(CXX) $(CFLAGS) -c  SerialConnector/SerialConnector.c -o SerialConnector/SerialConnector.o
    


# автоматическая компиляция всего и вся
#.c.o:
#	$(CXX) $(CFLAGS) -c $*.c -o $*.o


clean:
	rm -f CopterController
	rm -rf *.o *.a
    
cleanAfter:
	rm -rf ControllerListner/ControllerListner.o
	rm -rf SerialConnector/SerialConnector.o

