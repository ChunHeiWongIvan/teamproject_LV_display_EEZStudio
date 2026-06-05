#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN_MENU = 1,
    SCREEN_ID_CHARGING_MENU_1 = 2,
    SCREEN_ID_CHARGING_MENU_2 = 3,
    SCREEN_ID_CHARGING_MENU_3 = 4,
    SCREEN_ID_CHARGING_MENU_4 = 5,
    SCREEN_ID_CHARGING_MENU_5 = 6,
    SCREEN_ID_CHARGING_MENU_7 = 7,
    SCREEN_ID_CHARGING_MENU_8 = 8,
    SCREEN_ID_CHARGING_MENU_9 = 9,
    SCREEN_ID_SET_PARAMETERS_1 = 10,
    SCREEN_ID_SET_PARAMETERS_2 = 11,
    SCREEN_ID_VIEW_DEBUG = 12,
    _SCREEN_ID_LAST = 12
};

typedef struct _objects_t {
    lv_obj_t *main_menu;
    lv_obj_t *charging_menu_1;
    lv_obj_t *charging_menu_2;
    lv_obj_t *charging_menu_3;
    lv_obj_t *charging_menu_4;
    lv_obj_t *charging_menu_5;
    lv_obj_t *charging_menu_7;
    lv_obj_t *charging_menu_8;
    lv_obj_t *charging_menu_9;
    lv_obj_t *set_parameters_1;
    lv_obj_t *set_parameters_2;
    lv_obj_t *view_debug;
    lv_obj_t *title;
    lv_obj_t *start_charging_button;
    lv_obj_t *set_parameters_button;
    lv_obj_t *set_parameters_label;
    lv_obj_t *view_debug_button;
    lv_obj_t *obj0;
    lv_obj_t *status_container;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *status_label;
    lv_obj_t *bottom_left_stat_desc;
    lv_obj_t *parameters_label;
    lv_obj_t *obj4;
    lv_obj_t *main_menu_cv_label;
    lv_obj_t *obj5;
    lv_obj_t *main_menu_cc_label;
    lv_obj_t *detailed_status_label;
    lv_obj_t *main_menu_voltage_label;
    lv_obj_t *main_menu_current_label;
    lv_obj_t *back_button_1;
    lv_obj_t *output_voltage_chart;
    lv_obj_t *output_voltage_scale_1;
    lv_obj_t *output_voltage_label;
    lv_obj_t *output_voltage_scale_2;
    lv_obj_t *left_button_1;
    lv_obj_t *right_button_1;
    lv_obj_t *back_button_4;
    lv_obj_t *output_current_chart;
    lv_obj_t *output_current_scale_1;
    lv_obj_t *output_current_label;
    lv_obj_t *output_current_scale_2;
    lv_obj_t *left_button_2;
    lv_obj_t *right_button_2;
    lv_obj_t *back_button_5;
    lv_obj_t *output_power_chart;
    lv_obj_t *output_power_scale_1;
    lv_obj_t *output_power_label;
    lv_obj_t *output_power_scale_2;
    lv_obj_t *left_button_3;
    lv_obj_t *right_button_3;
    lv_obj_t *back_button_6;
    lv_obj_t *battery_voltage_chart;
    lv_obj_t *battery_voltage_scale_1;
    lv_obj_t *battery_voltage_label;
    lv_obj_t *battery_voltage_scale_2;
    lv_obj_t *left_button_4;
    lv_obj_t *right_button_4;
    lv_obj_t *back_button_7;
    lv_obj_t *pfc_voltage_chart;
    lv_obj_t *pfc_voltage_scale_1;
    lv_obj_t *pfc_voltage_label;
    lv_obj_t *pfc_voltage_scale_2;
    lv_obj_t *left_button_5;
    lv_obj_t *right_button_5;
    lv_obj_t *back_button_9;
    lv_obj_t *temperature_chart_1;
    lv_obj_t *temperature_scale_1;
    lv_obj_t *temperature_label_1;
    lv_obj_t *temperature_scale_2;
    lv_obj_t *left_button_7;
    lv_obj_t *right_button_7;
    lv_obj_t *back_button_10;
    lv_obj_t *temperature_chart_2;
    lv_obj_t *temperature_scale_3;
    lv_obj_t *temperature_label_2;
    lv_obj_t *temperature_scale_4;
    lv_obj_t *left_button_8;
    lv_obj_t *right_button_8;
    lv_obj_t *back_button_11;
    lv_obj_t *temperature_chart_3;
    lv_obj_t *temperature_scale_5;
    lv_obj_t *temperature_label_3;
    lv_obj_t *temperature_scale_6;
    lv_obj_t *left_button_9;
    lv_obj_t *right_button_9;
    lv_obj_t *constant_voltage_setpoint_bar;
    lv_obj_t *constant_voltage_setpoint_label;
    lv_obj_t *back_button_2;
    lv_obj_t *left_button_10;
    lv_obj_t *right_button_10;
    lv_obj_t *constant_current_setpoint_bar_1;
    lv_obj_t *constant_current_setpoint_label_1;
    lv_obj_t *obj6;
    lv_obj_t *constant_current_setpoint_bar;
    lv_obj_t *constant_current_setpoint_label;
    lv_obj_t *back_button_12;
    lv_obj_t *left_button_11;
    lv_obj_t *right_button_11;
    lv_obj_t *obj7;
    lv_obj_t *constant_voltage_setpoint_bar_1;
    lv_obj_t *constant_voltage_setpoint_label_1;
    lv_obj_t *title_5;
    lv_obj_t *back_button_3;
    lv_obj_t *obj8;
    lv_obj_t *obj9;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
    lv_obj_t *obj15;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *obj18;
    lv_obj_t *obj19;
    lv_obj_t *obj20;
    lv_obj_t *obj21;
    lv_obj_t *obj22;
    lv_obj_t *hv_states;
    lv_obj_t *overall_states;
    lv_obj_t *input_states;
    lv_obj_t *lv_states;
} objects_t;

extern objects_t objects;

void create_screen_main_menu();
void tick_screen_main_menu();

void create_screen_charging_menu_1();
void tick_screen_charging_menu_1();

void create_screen_charging_menu_2();
void tick_screen_charging_menu_2();

void create_screen_charging_menu_3();
void tick_screen_charging_menu_3();

void create_screen_charging_menu_4();
void tick_screen_charging_menu_4();

void create_screen_charging_menu_5();
void tick_screen_charging_menu_5();

void create_screen_charging_menu_7();
void tick_screen_charging_menu_7();

void create_screen_charging_menu_8();
void tick_screen_charging_menu_8();

void create_screen_charging_menu_9();
void tick_screen_charging_menu_9();

void create_screen_set_parameters_1();
void tick_screen_set_parameters_1();

void create_screen_set_parameters_2();
void tick_screen_set_parameters_2();

void create_screen_view_debug();
void tick_screen_view_debug();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/