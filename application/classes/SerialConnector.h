#include <stdio.h>    
#include <stdlib.h>
#include <string.h>   
#include <getopt.h>

#include "arduino-serial-lib.h"

class SerialConnector {
private:
    int fd;
public:
    SerialConnector();
    SerialConnector(char* serialport, int baudrate);
    void usage();
    void error(char* msg);
    void ConnectionOpen(char* serialport, int baudrate);
    void send(char* msg);
};

SerialConnector::SerialConnector(){
    fd = -1;
}

SerialConnector::SerialConnector(char* serialport, int baudrate)
{
    fd = serialport_init(serialport, baudrate);
    if( fd==-1 ) error("couldn't open port");
    printf("opened port %s\n",serialport);
    serialport_flush(fd);
}


void SerialConnector::error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

void SerialConnector::ConnectionOpen(char* serialport, int baudrate)
{
    if( fd != -1 ) {
        serialport_close(fd);
        printf("closed port %s\n",serialport);
    }
    
    fd = serialport_init(serialport, baudrate);
    if( fd==-1 ) error("couldn't open port");
    printf("opened port %s\n",serialport);
    serialport_flush(fd);
}

void SerialConnector::send(char* msg)
{
    if( fd == -1 ) error("serial port not opened");
    char buf[250];
    sprintf(buf, "%s", msg);
    
    printf("send string:%s\n", buf);
    int rc = serialport_write(fd, buf);
    if(rc==-1) error("error writing");
}
