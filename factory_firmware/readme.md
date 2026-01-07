# How to install the factory demo firmware with flash download tool?

#### 1.Preparation:

- USB C cable
- Laptop/PC
- Crowpanel Advance HMI ESP32 AI Display
- Flash download tool

Check whether the materials on the PCBA board are defective. Such as less material, false welding, tin and other bad phenomena.

#### 2.Flash the firmware 

##### Step1 Connect the ESP32 display to the laptop/PC with USB-C cable. The power indicator will light up.

![flash01](./flash01.jpg)

##### Step2 Open the flash download tool. 

![flash02](./flash02.jpg)



For the  Crowpanel Advance HMI ESP32 AI Display, the chip select ESP32-P4:

<img width="219" height="212" alt="image" src="https://github.com/user-attachments/assets/aba4d454-b576-4804-8669-8b9a84136e08" />


##### Step3

①Choose the firmware files;


②Enter the address: 

bootloader-0x2000.bin-------0x2000

esp32p4_dev_kits_ui-0x10000.bin----0x10000

partition-table-0x8000.bin-----0x8000

storage-0x910000.bin-------0x910000



③Check the firmware files;

④Select the correct port number;

⑤Click “Start” to download


##### Step5 After the download is complete, blue FINISH will be displayed, and the progress bar at the bottom shows that it has come to an end.


##### Step6 Press the reset button to show the demo.

