#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_to_charging_menu_1(lv_event_t * e);
extern void action_to_charging_menu_2(lv_event_t * e);
extern void action_to_charging_menu_3(lv_event_t * e);
extern void action_to_set_parameters_1(lv_event_t * e);
extern void action_set_parameters_keypad_voltage(lv_event_t * e);
extern void action_to_view_debug(lv_event_t * e);
extern void action_to_main_menu(lv_event_t * e);
extern void action_to_test(lv_event_t * e);
extern void action_to_charging_menu_4(lv_event_t * e);
extern void action_to_charging_menu_5(lv_event_t * e);
extern void action_to_charging_menu_9(lv_event_t * e);
extern void action_to_charging_menu_6(lv_event_t * e);
extern void action_to_charging_menu_7(lv_event_t * e);
extern void action_to_charging_menu_8(lv_event_t * e);
extern void action_to_set_parameters_2(lv_event_t * e);
extern void action_set_parameters_keypad_current(lv_event_t * e);
extern void action_demo_change_state(lv_event_t * e);
extern void action_demo_start_charts(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/