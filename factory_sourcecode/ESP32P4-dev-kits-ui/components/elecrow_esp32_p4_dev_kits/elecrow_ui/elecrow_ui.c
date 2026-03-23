/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "include/elecrow_ui.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
static lv_obj_t *Elecrow_logo_screen;
static lv_obj_t *Elecrow_P_bar_screen;
static lv_obj_t *Bp_bg_img;
static lv_obj_t *Bp_logo_img;
static lv_obj_t *Load_bg_img;
static lv_obj_t *Load_frame_img;
static lv_obj_t *Load_arrow_img;
static lv_obj_t *Load_label;
static lv_obj_t *Progress_bar_frame;
static lv_obj_t *Progress_bar_img;
lv_timer_t *progress_timer;
bool elecrow_success = false;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
static void update_progress(lv_timer_t *timer)
{
    static int progress = 0;
    progress += 2;

    if (progress > 100)
        progress = 100;
    lv_coord_t new_width = 34 + ((652 * progress) / 100);
    lv_obj_set_size(Progress_bar_img, new_width, 44);
    lv_obj_invalidate(Progress_bar_img);
    if (progress >= 100)
    {
        lv_timer_del(timer);
        elecrow_success = true;
    }
}

static void switch_page_timer_cb(lv_timer_t *timer)
{
    lv_scr_load_anim(Elecrow_P_bar_screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    progress_timer = lv_timer_create(update_progress, 100, NULL);
}

void elecrow_screen(void)
{
    Elecrow_logo_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(Elecrow_logo_screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Elecrow_logo_screen, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    Elecrow_P_bar_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(Elecrow_logo_screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Elecrow_logo_screen, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    Bp_bg_img = lv_img_create(Elecrow_logo_screen);
    lv_img_set_src(Bp_bg_img, &Bp_background);
    lv_obj_align(Bp_bg_img, LV_ALIGN_CENTER, 0, 0);
    Bp_logo_img = lv_img_create(Bp_bg_img);
    lv_img_set_src(Bp_logo_img, &Elecrow_logo);
    lv_obj_align(Bp_logo_img, LV_ALIGN_CENTER, 0, 0);

    lv_scr_load(Elecrow_logo_screen);

    Load_bg_img = lv_img_create(Elecrow_P_bar_screen);
    lv_img_set_src(Load_bg_img, &Loading_background);
    lv_obj_align(Load_bg_img, LV_ALIGN_CENTER, 0, 0);

    Load_frame_img = lv_img_create(Load_bg_img);
    lv_img_set_src(Load_frame_img, &Loading_frame);
    lv_obj_align(Load_frame_img, LV_ALIGN_CENTER, 0, 220);

    Load_label = lv_label_create(Load_bg_img);
    lv_label_set_text(Load_label, "Loading");
    lv_obj_set_style_text_color(Load_label, lv_color_make(0xFF, 0xFF, 0xFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(Load_label, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(Load_label, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_align_to(Load_label, Load_frame_img, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

    Load_arrow_img = lv_img_create(Load_bg_img);
    lv_img_set_src(Load_arrow_img, &Loading_arrow);
    lv_obj_align_to(Load_arrow_img, Load_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

    Progress_bar_img = lv_img_create(Load_frame_img);
    lv_img_set_src(Progress_bar_img, &Progress_bar_all);
    lv_obj_align(Progress_bar_img, LV_ALIGN_LEFT_MID, 7, 0);
    lv_obj_set_size(Progress_bar_img, 34, 44);

    lv_timer_t *switch_timer = lv_timer_create(switch_page_timer_cb, 2000, NULL);
    lv_timer_set_repeat_count(switch_timer, 1);
}

/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/