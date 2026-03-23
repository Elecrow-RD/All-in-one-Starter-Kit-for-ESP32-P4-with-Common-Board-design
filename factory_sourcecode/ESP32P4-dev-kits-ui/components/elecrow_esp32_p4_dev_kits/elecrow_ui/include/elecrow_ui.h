#ifndef _ELECROW_UI_H
#define _ELECROW_UI_H
#include "lvgl.h"

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/

/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef __cplusplus
extern "C"
{
#endif
    LV_IMG_DECLARE(Bp_background);
    LV_IMG_DECLARE(Elecrow_logo);
    LV_IMG_DECLARE(Loading_arrow);
    LV_IMG_DECLARE(Loading_background);
    LV_IMG_DECLARE(Loading_frame);
    LV_IMG_DECLARE(Progress_bar);
    LV_IMG_DECLARE(Progress_bar_all);
    void elecrow_screen(void);
    extern bool elecrow_success;
#ifdef __cplusplus
} /*extern "C"*/
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/

#endif