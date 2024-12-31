// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"

void ui_Home_screen_init(void)
{
    ui_Home = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Button2 = lv_btn_create(ui_Home);
    lv_obj_set_width(ui_Button2, 53);
    lv_obj_set_height(ui_Button2, 50);
    lv_obj_set_x(ui_Button2, -105);
    lv_obj_set_y(ui_Button2, 62);
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Button3 = lv_btn_create(ui_Home);
    lv_obj_set_width(ui_Button3, 81);
    lv_obj_set_height(ui_Button3, 50);
    lv_obj_set_x(ui_Button3, -8);
    lv_obj_set_y(ui_Button3, 62);
    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Button5 = lv_btn_create(ui_Home);
    lv_obj_set_width(ui_Button5, 100);
    lv_obj_set_height(ui_Button5, 50);
    lv_obj_set_x(ui_Button5, 103);
    lv_obj_set_y(ui_Button5, 63);
    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button5, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Temperture = lv_obj_create(ui_Home);
    lv_obj_remove_style_all(ui_Temperture);
    lv_obj_set_width(ui_Temperture, 122);
    lv_obj_set_height(ui_Temperture, 50);
    lv_obj_set_x(ui_Temperture, -61);
    lv_obj_set_y(ui_Temperture, -77);
    lv_obj_set_align(ui_Temperture, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Temperture, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label3 = lv_label_create(ui_Temperture);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, -15);
    lv_obj_set_y(ui_Label3, 0);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "25.0");
    lv_obj_set_style_text_font(ui_Label3, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label5 = lv_label_create(ui_Temperture);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label5, 42);
    lv_obj_set_y(ui_Label5, 3);
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "'C");
    lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Temperture1 = lv_obj_create(ui_Home);
    lv_obj_remove_style_all(ui_Temperture1);
    lv_obj_set_width(ui_Temperture1, 122);
    lv_obj_set_height(ui_Temperture1, 50);
    lv_obj_set_x(ui_Temperture1, 69);
    lv_obj_set_y(ui_Temperture1, -78);
    lv_obj_set_align(ui_Temperture1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Temperture1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label1 = lv_label_create(ui_Temperture1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label1, -15);
    lv_obj_set_y(ui_Label1, 0);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "45");
    lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(ui_Temperture1);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, 28);
    lv_obj_set_y(ui_Label2, 3);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "%");
    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Temperture2 = lv_obj_create(ui_Home);
    lv_obj_remove_style_all(ui_Temperture2);
    lv_obj_set_width(ui_Temperture2, 206);
    lv_obj_set_height(ui_Temperture2, 50);
    lv_obj_set_x(ui_Temperture2, 4);
    lv_obj_set_y(ui_Temperture2, -12);
    lv_obj_set_align(ui_Temperture2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Temperture2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label8 = lv_label_create(ui_Temperture2);
    lv_obj_set_width(ui_Label8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label8, -33);
    lv_obj_set_y(ui_Label8, 0);
    lv_obj_set_align(ui_Label8, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label8, "1024");
    lv_obj_set_style_text_font(ui_Label8, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label9 = lv_label_create(ui_Temperture2);
    lv_obj_set_width(ui_Label9, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label9, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label9, 121);
    lv_obj_set_y(ui_Label9, 3);
    lv_obj_set_align(ui_Label9, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_Label9, "hPa");
    lv_obj_set_style_text_font(ui_Label9, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button3, ui_event_Button3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button5, ui_event_Button5, LV_EVENT_ALL, NULL);
    uic_sample = ui_Button2;

}
