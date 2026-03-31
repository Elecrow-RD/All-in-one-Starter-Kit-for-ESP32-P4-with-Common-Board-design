#include "Arduino.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <lvgl_v8_port.h>
#include "bsp_i2c.h"
#include "bsp_lsm6ds3tr.h"

#define DPS_TO_RADS (3.14159265f / 180.0f)
#define GRAVITY_CONSTANT 9.80665f

using namespace esp_panel::drivers;
using namespace esp_panel::board;
TaskHandle_t read_lsm6ds3;
static lv_obj_t *lsm6ds3_acc_data = NULL;
static lv_obj_t *lsm6ds3_gry_data = NULL;

void update_lsm6ds3_value(lsm6ds3tr data) {
  if ((lsm6ds3_acc_data) && (lsm6ds3_gry_data)) {
    char buffer_acc[80];
    char buffer_gry[80];
    snprintf(buffer_acc, sizeof(buffer_acc), "acc_x: %.2f m/s2 acc_y = %.2f m/s2 acc_z = %.2f m/s2", data.acc_x * GRAVITY_CONSTANT, data.acc_y * GRAVITY_CONSTANT, data.acc_z * GRAVITY_CONSTANT);    /*Format the data into a string*/
    snprintf(buffer_gry, sizeof(buffer_gry), "gry_x: %.2f rad/s gry_y = %.2f rad/s gry_z = %.2f rad/s", data.gry_x * DPS_TO_RADS, data.gry_y * DPS_TO_RADS, data.gry_z * DPS_TO_RADS); /*Format the data into a string*/
    lv_label_set_text(lsm6ds3_acc_data, buffer_acc);                                                                                         /*Set a new text for a label*/
    lv_label_set_text(lsm6ds3_gry_data, buffer_gry);                                                                                         /*Set a new text for a label*/
  }
}

void lsm6ds3_read_task(void *param) {
  while (1) {
    if (lsm6ds3_scan() != ESP_OK) /*The lsm6ds3 scans to determine whether the currently read data is from the gyroscope or the accelerometer*/
    {
      if (lvgl_port_lock(0)) {
        lv_label_set_text(lsm6ds3_acc_data, "lsm6ds3 read acc data error"); /*Read failure message displayed*/
        lv_label_set_text(lsm6ds3_gry_data, "lsm6ds3 read gry data error"); /*Read failure message displayed*/
        lvgl_port_unlock();
      }
      //MAIN_ERROR("lsm6ds3 scan false");
    } else {
      if (lvgl_port_lock(0)) {
        update_lsm6ds3_value(my_lsm6ds3); /*Update the lsm6ds3 data displayed on the screen*/
        lvgl_port_unlock();
      }
    }
#if 0
        MAIN_INFO("acc_x: %.2f m/s2", my_lsm6ds3.acc_x);
        MAIN_INFO("acc_y: %.2f m/s2", my_lsm6ds3.acc_y);
        MAIN_INFO("acc_z: %.2f m/s2", my_lsm6ds3.acc_z);
        MAIN_INFO("gry_x: %.2f rad/s", my_lsm6ds3.gry_x);
        MAIN_INFO("gry_y: %.2f rad/s", my_lsm6ds3.gry_y);
        MAIN_INFO("gry_z: %.2f rad/s", my_lsm6ds3.gry_z);
#endif
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void setup() {

  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(32, 0);
  digitalWrite(33, 1);

  i2c_init();
  lsm6ds3_begin();
  Serial.begin(115200);

  Serial.println("Initializing board");


  Board *board = new Board();
  board->init();

  board->begin();


  lvgl_port_init(board->getLCD(), nullptr);
  board->getBacklight()->setBrightness(0); 
  lv_obj_t *scr = lv_scr_act();


  lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN); 
  lvgl_port_lock(-1);
  lsm6ds3_acc_data = lv_label_create(scr); /*Create a label object*/
  lv_obj_set_style_text_color(lsm6ds3_acc_data, lv_color_white(), LV_STATE_DEFAULT);
  lv_obj_align(lsm6ds3_acc_data, LV_ALIGN_CENTER, 0, -30);                                     /*Change the alignment of an object*/
  lv_label_set_text(lsm6ds3_acc_data, "acc_x: 0.00 m/s2 acc_y = 0.00 m/s2 acc_z = 0.00 m/s2"); /*Set a new text for a label*/

  lsm6ds3_gry_data = lv_label_create(scr); /*Create a label object*/
  lv_obj_set_style_text_color(lsm6ds3_gry_data, lv_color_white(), LV_STATE_DEFAULT);
  lv_obj_align(lsm6ds3_gry_data, LV_ALIGN_CENTER, 0, 30);                                         /*Change the alignment of an object*/
  lv_label_set_text(lsm6ds3_gry_data, "gry_x: 0.00 rad/s gry_y = 0.00 rad/s gry_z = 0.00 rad/s"); /*Set a new text for a label*/

  lvgl_port_unlock();
  xTaskCreate(lsm6ds3_read_task, "read_lsm6ds3", 4096, NULL, configMAX_PRIORITIES - 5, &read_lsm6ds3); /*Create a lsm6ds3tr data display refresh thread*/
  delay(200);
  board->getBacklight()->setBrightness(100); 
}

void loop() {
    delay(500);
}