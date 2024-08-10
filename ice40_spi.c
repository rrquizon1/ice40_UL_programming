#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <gpiod.h>
#include "fastconfig_data.h"
#include "ice40_spi.h"

static uint32_t speed;
static int spi_fd = -1;
char a='A';
int configure_output(struct gpiod_line *line, const char *consumer, int value)
{
    if (gpiod_line_request_output(line, consumer, value) < 0)
    {
        perror("Request line as output failed");
        return -1;
    }

    return 0;
}


int spi_init(int spi_speed,struct gpiod_line **crst,struct gpiod_line **cs){
	
	int ret = 0;
	uint8_t mode = SPI_MODE_0;
	uint8_t bits = 8;
	uint32_t reg;
	uint32_t shift;

	// setup spi via ioctl

	speed = spi_speed;

	spi_fd = open("/dev/spidev0.0", O_RDWR);
	if (spi_fd < 0) {
		fprintf(stderr, "Failed to open /dev/spidev0.0: %s\n", strerror(errno));
		return 0;
	}

	ret |= ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	ret |= ioctl(spi_fd, SPI_IOC_RD_MODE, &mode);
	ret |= ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	ret |= ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	ret |= ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	ret |= ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);

	if(ret) {
		fprintf(stderr, "Failed to setup SPI.\n");
	if (spi_fd > -1)
		close(spi_fd);
	spi_fd = -1;
		return 0;
	}	
	
	
	
	
	
	
    //GPIO CS and CRST initialize
    struct gpiod_chip *chip;

    
    
    int offset_crst = 20; // Replace with your GPIO pin number for CRST
    int offset_cs = 21; // Replace with your GPIO pin number for CS
    
    chip = gpiod_chip_open("/dev/gpiochip4"); // Replace 4 with the appropriate chip number

    if (!chip) {
        perror("Open chip failed");
        return 1;
    }

    *crst= gpiod_chip_get_line(chip, offset_crst);
    *cs= gpiod_chip_get_line(chip, offset_cs);

    if (!*crst) {
        perror("Get line failed");
        gpiod_chip_close(chip);
        return 1;
    }
    
    if (!*cs) {
        perror("Get line failed");
        gpiod_chip_close(chip);
        return 1;
    }

     configure_output(*crst, &a, 1);
     configure_output(*cs, &a, 1);
 
    usleep(100);

    usleep(100);

    usleep(100);
    return 0;
    
}

// Function for ioctl call
static int rbpi_ioctl(unsigned char *tx_buf, unsigned char *rx_buf, int len)
{
	struct spi_ioc_transfer req;

	memset(&req, 0, sizeof(req));

	req.tx_buf = (uintptr_t) tx_buf;
	req.rx_buf = (uintptr_t) rx_buf;
	req.len = len;

	return ioctl(spi_fd, SPI_IOC_MESSAGE(1), &req) == -1;
}


// Function for data transmit
int rbpi_tx(unsigned char *buf, int bytes)
{




	if(rbpi_ioctl(buf, NULL, bytes)) {
		fprintf(stderr, "SPI ioctl write failed: %s\n", strerror(errno));
		return 0;
	} else
		return 1;
}

//Closes the SPI bus

int rbpi_exit(){
	
	close(spi_fd);
}
