### 1, Product picture



### 2, Product version number

|      | Hardware | Software | Remark |
| ---- | -------- | -------- | ------ |
| 1    | V1.0     | V1.0     | latest |

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
| Size                             | 195*170*46（mm）                                             |
| Material                         | PP Plastic                                                   |
| Input                            | 5v                                                           |
| Weight                           | 600g                                                         |

### 4, Use the driver module

| Name | dependency library |
| ---- | ------------------ |
| LVGL | lvgl/lvgl@8.3.3    |

### 5,Quick Start
##### ESP-IDF starts

1.Right-click on an empty space in the project folder and select "Open with VS Code" to open the project.
![4](https://github.com/user-attachments/assets/a842ad62-ed8b-49c0-bfda-ee39102da467)



2.In the IDF plug-in, select the port, then compile and flash
![5](https://github.com/user-attachments/assets/76b6182f-0998-4496-920d-d262a5142df3)




### 6,Folder structure.
|--3D file： Contains 3D model files (.stp) for the hardware. These files can be used for visualization, enclosure design, or integration into CAD software.

|--Datasheet: Includes datasheets for components used in the project, providing detailed specifications, electrical characteristics, and pin configurations.

|--Eagle_SCH&PCB: Contains **Eagle CAD** schematic (`.sch`) and PCB layout (`.brd`) files. These are used for circuit design and PCB manufacturing.

|--example: Provides example code and projects to demonstrate how to use the hardware and libraries. These examples help users get started quickly.

|--factory_firmware: Stores pre-compiled factory firmware that can be directly flashed onto the device. This ensures the device runs the default functionality.

|--factory_sourcecode:  Contains the source code for the factory firmware, allowing users to modify and rebuild the firmware as needed.

|--libraries: Includes necessary libraries required for compiling and running the project. These libraries provide drivers and additional functionalities for the hardware.


### 7,Pin definition

**DSI**
<img width="427" height="695" alt="image" src="https://github.com/user-attachments/assets/8281ad80-b899-449b-96d8-22d0fff788d9" />


