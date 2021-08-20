#include <iostream>
#include <unistd.h>
#include <array>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <chrono>
#include <cmath>
#include <string>
#include <poll.h>
#include <csignal>
#include "MPU6050.h"

// BCM2835 pin number
#define INT_PIN 5

volatile bool dataReady = false;
const int ARRAY_SIZE = 500;
const int presize =  50;
const char FILENAME[] = "/home/pi/datalog/logall/fifo/MPU6050_data.fifo";
int fifo_fd = 0;


void Handle(int signo){
	if (fifo_fd) close(fifo_fd);
	unlink(FILENAME);
	exit(0);
}
// Write to FIFO, return 0 if no reader
int writeToFifo(int fd, void *data, int dlen)
{
    struct pollfd pollfd = {fd, POLLOUT, 0};
    poll(&pollfd, 1, 0);
    if (pollfd.revents&POLLOUT && !(pollfd.revents&POLLERR))
        return(fd ? write(fd, data, dlen) : 0);
    return(0);
}



int main(int argc, char * argv[]) {
	signal(SIGINT, Handle);
	signal(SIGTERM, Handle);
	if(!bcm2835_init()) {
		std::cout << "Failed to initialise bcm2835. sudo?\r\n";
		return 1;
	}

  	MPU6050 mpu6050;

	if (mpu6050.init()){
			std::cout << "Failed to initialise MPU6050\r\n";
			return 1;
  }

	mpu6050.setInterrupt(true);

	mkfifo(FILENAME, 0666);
	char buffer[256];
	int slen;
	bool drdy = false;

	while (1) {

    if (!fifo_fd){

                if ((fifo_fd = open(FILENAME, O_WRONLY | O_NONBLOCK)) >0 ){
                      std::cout << "FIFO PIPE opened, started streaming\r\n";
                }
								else{
									fifo_fd = 0;
								}

          }

    if (fifo_fd){
      		//mpu6050.resetFIFO();
		//mpu6050.waitDataReady();		
		
		while (!drdy){ drdy = !mpu6050.isDataReady();}
      		drdy =false;

		//mpu6050.getAccelDataFromFIFO();
		mpu6050.getAccelData();
		
  		slen = sprintf(buffer, "%.8f,%.8f,%.8f\n", mpu6050.data.ax, mpu6050.data.ay, mpu6050.data.az);

        if (!writeToFifo(fifo_fd, buffer, slen)){
                close(fifo_fd);
                std::cout << "Streaming stopped\r\n";
                fifo_fd=0;
        }
    }
}

	return 0;
}
