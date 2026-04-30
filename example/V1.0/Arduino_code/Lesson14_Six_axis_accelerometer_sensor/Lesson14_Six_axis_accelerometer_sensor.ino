#include "Arduino.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <lvgl_v8_port.h>
#include "bsp_i2c.h"
#include "bsp_lsm6ds3tr.h"

using namespace esp_panel::drivers;
using namespace esp_panel::board;

TaskHandle_t read_lsm6ds3;  // Task handle for the LSM6DS3 read task
static lv_obj_t *lsm6ds3_acc_data = NULL; // LVGL label object for accelerometer data
static lv_obj_t *lsm6ds3_gry_data = NULL; // LVGL label object for gyroscope data

// Function to update the LVGL labels with LSM6DS3 sensor data
void update_lsm6ds3_value(lsm6ds3tr data) {
  if ((lsm6ds3_acc_data) && (lsm6ds3_gry_data)) {
    char buffer_acc[80];
    char buffer_gry[80];

    // Format accelerometer data into a string
    snprintf(buffer_acc, sizeof(buffer_acc), "acc_x: %.2f m/s2 acc_y = %.2f m/s2 acc_z = %.2f m/s2", data.acc_x, data.acc_y, data.acc_z);

    // Format gyroscope data into a string
    snprintf(buffer_gry, sizeof(buffer_gry), "gry_x: %.2f rad/s gry_y = %.2f rad/s gry_z = %.2f rad/s", data.gry_x, data.gry_y, data.gry_z);

    // Update the LVGL labels with new sensor data
    lv_label_set_text(lsm6ds3_acc_data, buffer_acc);
    lv_label_set_text(lsm6ds3_gry_data, buffer_gry);
  }
}

// FreeRTOS task to continuously read LSM6DS3 sensor data
void lsm6ds3_read_task(void *param) {
  while (1) {
    // Scan the LSM6DS3 sensor to determine data availability
    if (lsm6ds3_scan() != ESP_OK) {
      if (lvgl_port_lock(0)) {
        // Display read error messages on the LVGL labels
        lv_label_set_text(lsm6ds3_acc_data, "lsm6ds3 read acc data error");
        lv_label_set_text(lsm6ds3_gry_data, "lsm6ds3 read gry data error");
        lvgl_port_unlock();
      }
      //MAIN_ERROR("lsm6ds3 scan false"); // Optional debug log
    } else {
      if (lvgl_port_lock(0)) {
        // Update the labels with the latest sensor values
        update_lsm6ds3_value(my_lsm6ds3);
        lvgl_port_unlock();
      }
    }

#if 0
    // Optional debug logs for sensor values
    MAIN_INFO("acc_x: %.2f m/s2", my_lsm6ds3.acc_x);
    MAIN_INFO("acc_y: %.2f m/s2", my_lsm6ds3.acc_y);
    MAIN_INFO("acc_z: %.2f m/s2", my_lsm6ds3.acc_z);
    MAIN_INFO("gry_x: %.2f rad/s", my_lsm6ds3.gry_x);
    MAIN_INFO("gry_y: %.2f rad/s", my_lsm6ds3.gry_y);
    MAIN_INFO("gry_z: %.2f rad/s", my_lsm6ds3.gry_z);
#endif

    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay 1 second
  }
}

void setup() {
  // Initialize GPIOs for board control
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(32, 0);
  digitalWrite(33, 1);

  // Initialize I2C and LSM6DS3 sensor
  i2c_init();
  lsm6ds3_begin();

  Serial.begin(115200);
  Serial.println("Initializing board");

  // Initialize the display board
  Board *board = new Board();
  board->init();
  board->begin();

  // Initialize LVGL port with the board's LCD
  lvgl_port_init(board->getLCD(), nullptr);

  // Set initial backlight to 0
  board->getBacklight()->setBrightness(0); 

  lv_obj_t *scr = lv_scr_act(); // Get the active LVGL screen

  // Set screen background to black
  lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

  lvgl_port_lock(-1); // Lock LVGL for object creation

  // Create and configure accelerometer label
  lsm6ds3_acc_data = lv_label_create(scr);
  lv_obj_set_style_text_color(lsm6ds3_acc_data, lv_color_white(), LV_STATE_DEFAULT);
  lv_obj_align(lsm6ds3_acc_data, LV_ALIGN_CENTER, 0, -30);
  lv_label_set_text(lsm6ds3_acc_data, "acc_x: 0.00 m/s2 acc_y = 0.00 m/s2 acc_z = 0.00 m/s2");

  // Create and configure gyroscope label
  lsm6ds3_gry_data = lv_label_create(scr);
  lv_obj_set_style_text_color(lsm6ds3_gry_data, lv_color_white(), LV_STATE_DEFAULT);
  lv_obj_align(lsm6ds3_gry_data, LV_ALIGN_CENTER, 0, 30);
  lv_label_set_text(lsm6ds3_gry_data, "gry_x: 0.00 rad/s gry_y = 0.00 rad/s gry_z = 0.00 rad/s");

  lvgl_port_unlock(); // Unlock LVGL

  // Create the FreeRTOS task to read LSM6DS3 sensor values
  xTaskCreate(lsm6ds3_read_task, "read_lsm6ds3", 4096, NULL, configMAX_PRIORITIES - 5, &read_lsm6ds3);

  delay(200);
  board->getBacklight()->setBrightness(100); // Set full backlight
}

void loop() {
    delay(500); // Main loop does nothing; sensor reading handled in task
}