This is an SSPI CRAM programming implementation of ice40 Ultralite using Raspberry pi 5. 

This example uses lgpiod library so makesure to install it: sudo apt-get install gpiod libgpiod-dev

For this example, the dedicated resources of raspberry pi 5 is used for SPI communication. Also, the buffer limit was modified to 65536 to be able to send the whole bitstream in one call.

To modify the buffer limit of the raspberry pi see here: https://raspberrypi.stackexchange.com/questions/65595/spi-transfer-fails-with-buffer-size-greater-than-4096

The instructions used for programming for this example is taken from FPGA-TN-02001-3.4( ice40 Programming and Configuration) technical note. See below for the waveform instructions from the document.
![image](https://github.com/user-attachments/assets/d2f8afd1-0711-4ea5-af9f-bec0dd8c61b2)

Source files: 
main.c-contains the programming procedure
fastconfig_data.c- contains the bitstream data to be sent. This can be modified to send bitsream on different devices
ice40_spi.c- contains the functions used for SPI transaction and gpio control
Makefile-compiles the project 

You can modify g_iDataSize and g_pucDataArray with your bitstream information. With this example, ice40 UL breakout board is used which has 30945 bytes of data. This is within the buffer size limit of 65536.
![image](https://github.com/user-attachments/assets/6a628d53-7f24-4785-b03e-134fbfeb3cab)

Main.c follows the programming procedure indicated from FPGA-TN-02001-3.4. 

![image](https://github.com/user-attachments/assets/b38a9cd4-a934-4bb0-b11b-ec684b92ef04)


Sample waveform transactions are below:
![image](https://github.com/user-attachments/assets/0a807872-a117-482f-bf04-91dee6643076)

Reset Sequence:
![image](https://github.com/user-attachments/assets/b0401fc8-0eef-4997-9d2c-b18e3625c61b)

End of sending bitstream and CDONE goes high after sending 01 06 (wake up command) located at the end of the bitstream:
![image](https://github.com/user-attachments/assets/b1dbc925-c353-4fdb-9c96-95d0fce48403)





