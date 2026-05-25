#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: no_shadow_button
lv_style_t *get_style_no_shadow_button_MAIN_DEFAULT();
lv_style_t *get_style_no_shadow_button_MAIN_DISABLED();
lv_style_t *get_style_no_shadow_button_MAIN_PRESSED();
void add_style_no_shadow_button(lv_obj_t *obj);
void remove_style_no_shadow_button(lv_obj_t *obj);

// Style: no_shadow_keypad
lv_style_t *get_style_no_shadow_keypad_ITEMS_DEFAULT();
void add_style_no_shadow_keypad(lv_obj_t *obj);
void remove_style_no_shadow_keypad(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/