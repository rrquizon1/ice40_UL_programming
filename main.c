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

struct gpiod_line *crst;
struct gpiod_line *cs;




int main() {
	unsigned char initialclock[1] = { 0xFF };
	unsigned char extraclocks[20] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
	
	
	spi_init(3000,&crst,&cs);
	gpiod_line_set_value(crst, 0);
	gpiod_line_set_value(cs, 0);
	
	usleep(1); //200 ns
	gpiod_line_set_value(crst, 1);
	usleep(1500); //1200 ns
	
	
	
	gpiod_line_set_value(cs, 1);
	rbpi_tx(initialclock,1); // 8 clock cycles while CS is high
	gpiod_line_set_value(cs, 0);
	
	
	usleep(1); // 1 us delay before bitstream sending
	
	
    
	rbpi_tx(g_pucDataArray,g_iDataSize); //sending of bitstream
    
    
    	gpiod_line_set_value(cs, 1);
	
	rbpi_tx(extraclocks,20); //extra clocks necessary after CDONE goes high.

	

	
	
	
}
