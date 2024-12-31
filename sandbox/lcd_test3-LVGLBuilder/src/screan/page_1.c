#include "page_1.h"
#include "app.h"

/**********************
 *       WIDGETS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/


lv_obj_t* button_1_ev_0;
lv_obj_t* slider_1_ev_0;

lv_obj_t* page_1_create(){
	lv_obj_t *parent = lv_obj_create(NULL, NULL);
	_lv_obj_set_style_local_color(parent,0,LV_STYLE_BG_COLOR,lv_color_hex(0xeaeff3));

	static lv_style_t button_1_s0;
	lv_style_init(&button_1_s0);
	lv_style_set_value_font(&button_1_s0,LV_STATE_DEFAULT,&lv_font_123abc_16);
	lv_style_set_value_font(&button_1_s0,LV_STATE_CHECKED,&lv_font_123abc_16);
	lv_style_set_value_font(&button_1_s0,LV_STATE_FOCUSED,&lv_font_123abc_16);
	lv_style_set_value_font(&button_1_s0,LV_STATE_EDITED,&lv_font_123abc_16);
	lv_style_set_value_font(&button_1_s0,LV_STATE_HOVERED,&lv_font_123abc_16);
	lv_style_set_value_font(&button_1_s0,LV_STATE_PRESSED,&lv_font_123abc_16);
	lv_style_set_value_font(&button_1_s0,LV_STATE_DISABLED,&lv_font_123abc_16);
	lv_obj_t *button_1 = lv_btn_create(parent, NULL);
	lv_obj_add_style(button_1, 0, &button_1_s0);
	lv_obj_set_hidden(button_1, false);
	lv_obj_set_click(button_1, true);
	lv_obj_set_drag(button_1, false);
	lv_obj_set_pos(button_1, 21, 35);
	lv_obj_set_size(button_1, 62, 23);
	lv_btn_set_checkable(button_1, false);
	lv_btn_set_fit(button_1, LV_FIT_NONE);

	button_1_ev_0 = button_1;


	static lv_style_t slider_1_s0;
	lv_style_init(&slider_1_s0);
	lv_style_set_outline_opa(&slider_1_s0,LV_STATE_DEFAULT,127);
	lv_style_set_outline_opa(&slider_1_s0,LV_STATE_CHECKED,127);
	lv_style_set_outline_color(&slider_1_s0,LV_STATE_FOCUSED,lv_color_hex(0x44d1b6));
	lv_style_set_outline_opa(&slider_1_s0,LV_STATE_FOCUSED,0);
	lv_style_set_outline_opa(&slider_1_s0,LV_STATE_EDITED,127);
	lv_style_set_outline_opa(&slider_1_s0,LV_STATE_HOVERED,127);
	lv_style_set_outline_opa(&slider_1_s0,LV_STATE_PRESSED,127);
	lv_style_set_outline_opa(&slider_1_s0,LV_STATE_DISABLED,127);
	static lv_style_t slider_1_s1;
	lv_style_init(&slider_1_s1);
	static lv_style_t slider_1_s2;
	lv_style_init(&slider_1_s2);
	lv_obj_t *slider_1 = lv_slider_create(parent, NULL);
	lv_obj_add_style(slider_1, 0, &slider_1_s0);
	lv_obj_add_style(slider_1, 1, &slider_1_s1);
	lv_obj_add_style(slider_1, 2, &slider_1_s2);
	lv_obj_set_hidden(slider_1, false);
	lv_obj_set_click(slider_1, true);
	lv_obj_set_drag(slider_1, false);
	lv_obj_set_pos(slider_1, 22, 91);
	lv_obj_set_size(slider_1, 103, 10);
	lv_slider_set_range(slider_1, 0, 100);
	lv_slider_set_anim_time(slider_1,200);
	lv_slider_set_value(slider_1, 45, LV_ANIM_ON);

	slider_1_ev_0 = slider_1;


	return parent;
}
