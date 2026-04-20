# Using the Elecrow esp32-p4 kit on Fedora

The Elecrow esp32-p4 kit is a fun way to play with the esp32-p4 with a rich variety of I/O devices. As a Linux developer, however, I find the official Elecrow instruction manual pdf, with its emphasis on Visual Studio and fuzzy screenshot images to be very hard to use. It is also surprisingly hard to tell what the expected output should be for each example application. 

The description of example 6 is particularly bad, as it has a screenshot in Chinese, and never explains how to run a serial monitor over USB, which is essential to connect the UART input and output. (It provides only a google drive link to an exe file, which is essentially useless for both Windows and Linux). On linux, esp-idf comes with a built in serial monitor as described below.

I find it much easier and much more useful to use the esp-idf development directly. It is easier to install, easier to use, and provides the full power of esp-idf. This markdown document shows how to install esp-idf on Fedora, and shows how to use it directly on the Elecrow provided example applications, inluding how to use esp-idf's built in serial monitor. It has clear explanations (in English) of what to expect for each example.

This repository is a fork of Elecrow's original repo, and includes some patches to fix minor issues in the original example codes.

## 1. Install esp-idf in a user home directory

It is easiest to install esp-idf in ~/esp/esp-idf, as shown in these instructions. Multiple versions of esp-idf can be installed in ~/esp, and the desired one chosen by pointing the symbolic link ~/esp/esp-idf to it. 

**IMPORTANT NOTE: The Elecrow example applications only work with esp-idf version 5.4.2.** (For some reason version 5.5.2 breaks example 17 (record mic to sd card), and version 6.0 also breaks several bsp include files.
```
$ sudo dnf install git wget flex bison gperf python3 cmake ninja-build ccache dfu-util libusbx
$ cd
$ mkdir esp
$ cd esp
$ git clone -b v5.4.2 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.4.2
$ ln -s esp-idf-v5.4.2/ esp-idf
$ cd esp-idf
$ ./install.sh all
$ cd    
$ vi .bashrc
   add the following line
       alias get_idf='. $HOME/esp/esp-idf/export.sh'
   
Put user in 'dialout' group
$ sudo usermod -a -G dialout <user>
```
Logout and log back in for the group membership to take effect.


## 2. Install updated Elecrow examples

The example repository can be cloned in any user subdirectory.
```
$ git clone https://github.com/safforddr/All-in-one-Starter-Kit-for-ESP32-P4-with-Common-Board-design.git
```

## 3. Running the Elecrow example applications

First connect a usb cable from your computer to the "Serial/JTAG" labeled USB-C port on the Elecrow kit. (This is the one closer to the box's corner). Then run an example program with:
```
   $ get_idf
   $ cd <example dir>
   $ idf.py build
   $ idf.py flash
   $ idf.py monitor (ctrl ']' to exit)
 ```  
Here the "example dir" is the example directory with the top level "sdkconfig" file. For example, application 1 ("gpio") has the build directory:
```
All-in-one-Starter-Kit-for-ESP32-P4-with-Common-Board-design/example/idf-code/
    1ESP32P4-dev-kits_gpio/ESP32P4-dev-kits_gpio
```
The USB port provides power, is able to flash the program, and is also able to connect a terminal to the kit's console, so you can view console output and provide console input. The "idf.py monitor" command is essential for example 6, but it is also useful to observe the boot and operation of all of the examples and for debugging.

The example applications are already configured, but the configurations can be changed (for example to turn on secure boot) with the command:
```
$ idf.py menuconfig
```
## 4. Notes on the Example Applications

### 1. "gpio" (Flash an led on gpio 5)
Expected output: the green LED next to gpio 5 should blink continuously.

### 2. "relay"
Expected output: The red led by the relay should blink, and the relay should switch the screw-down output lines, connecting the common line to the normally open, rather than the default normally closed pin.

### 3. "touch" (touch pad on circuit board)
Expected output: touching the touch pad should invert the led on gpio 5.

### 4. "pir" (Passive Infrared sensor)
Expected output: The PIR sensor detects changes in reflected light. Moving your hand in front of the sensor should turn on led io5. If you hold your hand steady, the led should go out.

### 5. "hall" (hall effect sensor)
Expected output: placing a small magned near the sensor should turn on both led 7 (which is on the sensor gpio line), and output led io5.

### 6. "uart" (console over usb)
Expected output: the input and output are only available on the usb console, which is connected with the normal "idf.py monitor" command. Typing "LED_ON" or "LED_OFF" on the serial console (with or without a following 'enter' key) will turn LED 5 on and off.

### 7. "timer" (in the esp32-p4 chip)
Expected output: led io5 shoudld blink every three seconds.

### 8. "servo" (move the pwm controlled servo)
Expected output: the servo should repeatedly rotate in both directions.

### 9. "display" (output on the LCD)
Expected output: "Hello P4" should be displayed in the center of the LCD, with the background changing between red, yellow and blue.

### 10. "ultrasonic" (measure distance with the ultrasonic sensor)
Expected output: A distance in centimeters will be displayed on the LCD, and on the serial console.

### 11. "dht20" (temperature and humidity sensor)
Expected output: the temperature (C) and humidity (%) will be displayed on the LCD, and on the serial console.

### 12. "bh1750" (light sensor)
Expected output: The light intensity on the sensor (in lux) will be displayed on the LCD, and on the serial console.

### 13. "LSM6DS3" (gyro and accelerometer)
Expected output: X, Y and Z linear and rotational accelerations will be displayed on the LCD, and on the serial console.

### 14. "led" (RGYB LEDs)
Expected output: the four color LEDs should illuminate sequentially.

### 15. "key" (analog button input)
Expected output: Pressing one of the analog ("ADC") buttons will turn on one of the four color LEDs, and the button up and down events are output on the serial console.

### 16. "gas" (smoke detector sensor)
Expected output: By default, the air quality is periodicaly reported only on the serial console. If a flame is burning near the sensor (with a match or lighter), the output should go above 1000ppm, which will turn on the 4 color LEDs.

### 17. "mic" (microphone input)
Setup: insert a fat32 formatted micro sd card. I have successfully tried 2GB to 32 GB cards.
Expected output: five seconds of microphone input will be recorded and saved on the SD card. The serial console displays the start and end of the five second period.

### 18. "audio" (audio output to stereo speakers)
Expected output: The wav file recorded in (17) will be replayed through the speakers. The serial console displays information on the wav file.

### 19. "lvgl_touch" (LCD touch screen input)
Expected output: four colored buttons will display on the LCD, and pressing one of them will turn on the corresponding color LED.

##  5. Important Links and References

The esp-idf manual, version 6 for the esp32p4:

https://docs.espressif.com/projects/esp-idf/en/release-v6.0/esp32p4/index.html

The esp32p4 technical reference manual:

https://documentation.espressif.com/esp32-p4-chip-revision-v1.3_technical_reference_manual_en.pdf

The Elecrow github repository:

https://github.com/Elecrow-RD/All-in-one-Starter-Kit-for-ESP32-P4-with-Common-Board-design

The schematic to the Elecrow esp32-p4 kit:

https://github.com/Elecrow-RD/All-in-one-Starter-Kit-for-ESP32-P4-with-Common-Board-design/blob/master/Eagle_SCH%26PCB/1.0/All-in-one%20Starter%20Kit%20for%20ESP32%20P4_V1.0.pdf


## 6. GPIO Pins:
Elecrow kit specific gpio pin use is documented in the schematic, and also is reflected in the various example peripheral Kconfig files, which define the pins used by each peripheral, and which are picked up as part of the configuration process during build.
```
   
   boot switch - io35
   
   DHT20 (0x38) and gyro (0x6b) and dsi and light (0x5c) all use i2c with
      i2c-sda io18
      i2c-scl io19
      
   dsi also uses
          io5 lcd reset
          io20 lcd backlight
          io32 updn
          io33 shlr
          io40 reset tp
          io41 int tp
      
   servo io25
   
   ultrasonic echo io12
              trigger io13
   
   mic clk io3
       data io4
       
   hall io7
   
   touch pad io2
   
   2814 leds io8
       
   adc key io16
   
   pir io24
   
   relay io42
   
   sd do io39
   sd sdsck io43
   sd sdcmd io44
   
   speaker audio 
       enable io6
       i2s lrclk io21
       i2s bclk io22
       i2s dsdin io23
       
   camera csi
       i2c scl io46
       i2c sda io45
       
   uart tx io47
        rx io48
        
   spi clk io9
       miso io10
       mosi io11
       
   demo led io5
```