All-in-one_Starter_Kit_for_ESP32-P4_AI Demo User Guide
Preparation tools:
All-in-one Starter Kit for ESP32-P4 with Common Board

<img width="366" height="366" alt="image" src="https://github.com/user-attachments/assets/611640cc-9db6-447c-b972-8ccfb80c1863" />

Wireless Module ESP32-C6:

<img width="297" height="282" alt="image" src="https://github.com/user-attachments/assets/b171b389-6ec2-49bf-8bc1-94114488c5e3" />

USB cable:

<img width="491" height="268" alt="image" src="https://github.com/user-attachments/assets/e82e069c-077f-41aa-b05c-d7b5c605206d" />

VS Code (ESP-IDF v5.5.3 installed)

<img width="554" height="299" alt="image" src="https://github.com/user-attachments/assets/dcb6e6a3-7b16-4f5a-a775-3e635afd0c2e" />

Download the code for the P4 kit:
Download the code for the corresponding screen size:

https://github.com/Elecrow-RD/All-in-one-Starter-Kit-for-ESP32-P4-with-Common-Board-design/tree/master/example/AI%20Projects/ESP-IDF_Code

Use VS Code to open the project:
1. Open VS Code and choose to open the project folder.

<img width="180" height="301" alt="image" src="https://github.com/user-attachments/assets/25cdc2fe-d645-4de7-8b48-300c70c30a92" />

2. Select the folder you want to open, click “Select Folder” to open the project. Be careful not to double-click to open it.

<img width="554" height="334" alt="image" src="https://github.com/user-attachments/assets/817a0a06-1243-4c28-b4d8-a182f869a441" />

3. Connect the device to the computer. Connect the device side to the UART0 port and plug it into the computer’s COM port.

<img width="554" height="356" alt="image" src="https://github.com/user-attachments/assets/916cc636-29c0-4b0c-a1d1-2c65f8c4620f" />

4. Select ESP-IDF v5.4.2.

<img width="554" height="403" alt="image" src="https://github.com/user-attachments/assets/e8527bf0-301e-43fd-8942-9637c33b1b3f" />

5. Select the flashing method.

<img width="554" height="468" alt="image" src="https://github.com/user-attachments/assets/5a4b1499-af4a-4b51-9966-17846654b235" />

6. Select the COM port.

<img width="453" height="457" alt="image" src="https://github.com/user-attachments/assets/b4e5277f-fdb5-4ade-94c5-014f3b7df73a" />

7. Select the flashing development board – esp32p4.

<img width="475" height="456" alt="image" src="https://github.com/user-attachments/assets/bd82e767-3ddd-45ef-96f3-b72663147c4e" />

8.Before flashing, you can choose to clear the previous “build” cache. This step is not necessary, but you can choose to clear it. If you have modified the code, clearing the previous cache will improve the compilation success rate.

<img width="318" height="251" alt="image" src="https://github.com/user-attachments/assets/f9a8de82-80e1-4f73-8d49-6c872d523659" />

9. Compile the project. Please make sure the network connection is good. The compilation process may take some time, so please wait patiently.

<img width="384" height="348" alt="image" src="https://github.com/user-attachments/assets/5c0483b9-6403-41bb-bfbd-379e612718f9" />

This means the compilation is successful:

<img width="554" height="251" alt="image" src="https://github.com/user-attachments/assets/eb7d59a4-e1be-4a29-bccf-4e7d776b68ef" />

10. Click to flash to the P4 HMI development board.

<img width="554" height="342" alt="image" src="https://github.com/user-attachments/assets/e08ef5f2-46f2-4fa3-b5fe-476ec2b92377" />

Once the download is complete, start configuring the **Wi-Fi**:

<img width="554" height="380" alt="image" src="https://github.com/user-attachments/assets/bc6a8386-cb5d-4c16-8269-f9c706490185" />

① Connect your phone to the Wi-Fi network named “Xiaozhi-A0B5.” You will be automatically redirected to the Wi-Fi settings page. Enter the password for the Wi-Fi network you want to connect to, tap “Connect,” and the Wi-Fi setup is complete.

<img width="442" height="837" alt="image" src="https://github.com/user-attachments/assets/56eeed58-afab-4f5e-96c5-e2d76c744453" />

② A message indicating that the configuration was successful will appear.

<img width="388" height="729" alt="image" src="https://github.com/user-attachments/assets/e5104868-a8e8-47a1-9ca7-ceb857a70f02" />

11. After flashing is completed, when the device connects to Wi-Fi, the screen will display the verification code for connecting the device:

<img width="515" height="300" alt="image" src="https://github.com/user-attachments/assets/64aabb4b-7c24-4d5b-bc2a-2b6011ade3ff" />

12. On the computer, log in to the “xiaozhi.me” website to configure the device, and first set it to the English version.

<img width="553" height="142" alt="image" src="https://github.com/user-attachments/assets/203adcb9-8860-45de-bff0-95151169484b" />

13. Click “Console” to enter the console.

<img width="554" height="215" alt="image" src="https://github.com/user-attachments/assets/a7e8f730-67d4-40f6-b2f7-75826573258e" />

14. Click “Add Device” to add a device.

<img width="554" height="258" alt="image" src="https://github.com/user-attachments/assets/8fc96bfe-fb2d-45f2-b8c6-ddeeee3510c4" />

15. Enter the device code, then click “Confirm”. Note that the device code here must be based on the output displayed on your own device.

<img width="554" height="162" alt="image" src="https://github.com/user-attachments/assets/ef8c9529-91d2-49ff-ae8d-c0df9ab97f29" />

16. Select “Open Source” and click “Start Using”.

<img width="540" height="558" alt="image" src="https://github.com/user-attachments/assets/bc239043-d1e4-44c7-ae43-34d310b62a40" />

17. Now your AI device can already chat. Since the default language is Chinese, we also need to configure the role. Click “Configure Role”.

<img width="554" height="336" alt="image" src="https://github.com/user-attachments/assets/577dcde5-aec7-4331-9bc2-8eecb278794f" />

18. Set “Dialogue Language” to “English”.

<img width="554" height="291" alt="image" src="https://github.com/user-attachments/assets/700cc7ab-c5ac-4b7d-956a-546eb2bf4888" />

19. Save the configuration.

<img width="554" height="485" alt="image" src="https://github.com/user-attachments/assets/3df8f837-1514-451b-ac94-d5ce306341f1" />

20. Next, we set the wake word. First, go back to “Agents”.

<img width="554" height="268" alt="image" src="https://github.com/user-attachments/assets/7f5f9262-0dd1-4770-a21a-5cc1725cdc91" />

21. Select “Manage Devices”.

<img width="554" height="222" alt="image" src="https://github.com/user-attachments/assets/0da424ea-dead-4c2f-a093-4a806a02364e" />

22. Select “Customize”, then choose “English”, and click “Next” to enter the next configuration step.

<img width="554" height="288" alt="image" src="https://github.com/user-attachments/assets/32f7fe1d-1ba9-477d-8adb-0e67868112e5" />

23. In “Select Wake Word”, choose your wake word, or you can select “No Wake Word” if you do not want a wake word.
Note: Here you can also choose to customize the emojis you need by selecting “Emoji Collection”. Otherwise, there will be no emojis. You also need to select a preset font; otherwise, the dialogue text will not be fully displayed.

<img width="501" height="252" alt="image" src="https://github.com/user-attachments/assets/52638b84-6940-45a9-9376-27c81ae28391" />

24. After selecting, click “Next”.

<img width="554" height="276" alt="image" src="https://github.com/user-attachments/assets/a07c471d-aea7-4eb4-890f-5b32d71a8fe2" />

25. Click “Generate assets.bin”.

<img width="554" height="363" alt="image" src="https://github.com/user-attachments/assets/cc79c4de-6bb6-4883-8040-0d183f58fb0d" />

26. Start Generate.

<img width="554" height="461" alt="image" src="https://github.com/user-attachments/assets/c934bbad-0100-4931-8938-4cd4fa1aa3c2" />

27. Click “Flash to Device Online” to perform the remote update. After the update is completed, you can use the new wake word to wake up the device.

<img width="539" height="479" alt="image" src="https://github.com/user-attachments/assets/be329a1a-c145-42e3-bcba-4fe861499051" />

28. During the update process, please do not disconnect the device from power or the network.

<img width="542" height="346" alt="image" src="https://github.com/user-attachments/assets/f54990a2-4de3-4d03-8a72-ca5bb61f817d" />

