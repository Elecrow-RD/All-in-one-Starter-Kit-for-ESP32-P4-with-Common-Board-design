### 1, Product picture

<img width="1000" height="1000" alt="image" src="https://github.com/user-attachments/assets/ed67410f-5c83-4278-adfa-fd5618af45dd" />

### 2, Product version number

|      | Hardware | Software | Remark |
| ---- | -------- | -------- | ------ |
| 1    | V1.0     | V1.0     | old |
| 2    | V1.1     | V1.1     | latest |

### 3, product information

| Main Processor（ESP32-P4）       |                                                              |
| -------------------------------- | ------------------------------------------------------------ |
| CPU/SoC                          | 32-bit RISC-V dual-core processor up to 360 MHz for HP system;32-bit RISC-V single-core processor up to 40 MHz for LP system |
| System Memory                    | 16 MB (OPI/HPI)³  in-Package PSRAM                           |
| Storage                          | 128 KB HP ROM、16 KB LP ROM                                  |
| Development language             | C/C++ Language                                               |
| **Display**                      |                                                              |
| Size                             | 7inch                                                        |
| Resolution                       | 1024*600                                                     |
| Touch Type                       | Capacitive                                                   |
| Panel Type                       | IPS                                                          |
| **Wireless Communication**       |                                                              |
| Bluetooth                        | Not supported                                                |
| WiFi                             | Not supported                                                |
| Wireless communication expansion | Support can be used with [wireless module](https://www.elecrow.com/wireless-module-for-crowpanel-advanced-series.html) |
| Camera                           | 2 MegaPixels,100° wide-angle                                 |
| **Hardware**                     |                                                              |
| Interface                        | 1x TYPE-C Interface,1x I/C(ZX-HY2.0-4PZZ), 1x UART(ZX-HY2.0-4PZZ), 1xI/O(20P*2,P=2.54mm), 1xCSI, 1xUSB, 1x Ethernet,1x TF Card Slot |
| Button                           | Rest Button                                                  |
| LED Indicator                    | I/O port working status indicator light                      |
| **Other**                        |                                                              |
| Size                             | 195\*170*46（mm）                                            |
| Material                         | PP Plastic                                                   |
| Input                            | 5v                                                           |
| Weight                           | 600g                                                         |

### 4, Use the driver module

| Name | dependency library |
| ---- | ------------------ |
| LVGL | lvgl/lvgl@8.3.3    |

### 5,Quick Start
##### Arduino IDE starts

1.Download the library files used by this product to the 'libraries' folder.

C:\Users\Documents\Arduino\libraries\

![2](https://github.com/user-attachments/assets/86c568bb-3921-4a07-ae91-62d7ce752e50)



2.Open the Arduino IDE

<img width="377" height="579" alt="7739691367693(1)" src="https://github.com/user-attachments/assets/22315d81-9c54-4e66-8a24-9d8f98a2da0e" />


3.Open the code configuration environment and burn it

<img width="696" height="901" alt="17742563652932" src="https://github.com/user-attachments/assets/afd9f4b7-db8e-4cc0-a6c4-a514e849b9dd" />




##### ESP-IDF starts

1.Right-click on an empty space in the project folder and select "Open with VS Code" to open the project.
![4](https://github.com/user-attachments/assets/a842ad62-ed8b-49c0-bfda-ee39102da467)



2.In the IDF plug-in, select the port, then compile and flash

<img width="1363" height="721" alt="image" src="https://github.com/user-attachments/assets/bad461b0-11f0-441b-b4e3-eeb4521025d4" />


### 6,Folder structure.
|--3D file： Contains 3D model files (.stp) for the hardware. These files can be used for visualization, enclosure design, or integration into CAD software.

|--Datasheet: Includes datasheets for components used in the project, providing detailed specifications, electrical characteristics, and pin configurations.

|--Eagle_SCH&PCB: Contains **Eagle CAD** schematic (`.sch`) and PCB layout (`.brd`) files. These are used for circuit design and PCB manufacturing.

|--example: Provides example code and projects to demonstrate how to use the hardware and libraries. These examples help users get started quickly.

|--factory_firmware: Stores pre-compiled factory firmware that can be directly flashed onto the device. This ensures the device runs the default functionality.

|--factory_sourcecode:  Contains the source code for the factory firmware, allowing users to modify and rebuild the firmware as needed.

|--libraries: Includes necessary libraries required for compiling and running the project. These libraries provide drivers and additional functionalities for the hardware.


### 7,Pin definition
1，Ultrasonic Sensor：

<img width="370" height="138" alt="image" src="https://github.com/user-attachments/assets/e6a8b354-1124-4620-af22-ba4685f9069f" />

2，LIGHT SENSOR

<img width="676" height="374" alt="image" src="https://github.com/user-attachments/assets/f97fe070-47ac-44f4-bd7f-6a99e830d5a9" />

3，LED

<img width="941" height="483" alt="image" src="https://github.com/user-attachments/assets/0e140e54-709c-4a86-ac78-3b79fa6280a2" />

4,Temperature & Humidity(DHT20)

<img width="485" height="345" alt="image" src="https://github.com/user-attachments/assets/82d8b6d0-1c36-4596-a401-8bae135a51f6" />

5,DSI

<img width="847" height="658" alt="image" src="https://github.com/user-attachments/assets/51203bb7-09ba-4757-b013-a8ca0977f0c8" />

6.PIR

IO24；

7.Servo Motor

IO25;

8,Accelerometer & Gyro

<img width="911" height="508" alt="image" src="https://github.com/user-attachments/assets/54b352c6-cf55-4192-b52b-ddec2e89cb40" />

9,HALL SENSOR

IO7;

10,Custom key

IO16;

<img width="934" height="378" alt="image" src="https://github.com/user-attachments/assets/11aa1b4b-1d3d-45c4-8af9-8a24ed948e92" />

11,Touch Sensor

IO2;

12,MIC

<img width="636" height="277" alt="image" src="https://github.com/user-attachments/assets/8917cfe5-cd1a-40b3-833e-83697b859c03" />

13,Audio Interface


<img width="338" height="254" alt="image" src="https://github.com/user-attachments/assets/e23b1695-66dd-431e-9cc7-46f580655f5d" />

14,GAS-MQ2

IO17;

15,Relay

IO42;

16,CSI Interface

<img width="771" height="631" alt="image" src="https://github.com/user-attachments/assets/d2f39423-e91d-4f06-a4ae-2515899ac755" />

17,Wireless module

<img width="744" height="381" alt="image" src="https://github.com/user-attachments/assets/4c0879a7-d457-4eae-8af7-aab551854165" />


