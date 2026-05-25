#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: no_shadow_button
//

void init_style_no_shadow_button_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_shadow_width(style, 0);
};

lv_style_t *get_style_no_shadow_button_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_no_shadow_button_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_no_shadow_button_MAIN_DISABLED(lv_style_t *style) {
    lv_style_set_shadow_width(style, 0);
};

lv_style_t *get_style_no_shadow_button_MAIN_DISABLED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_no_shadow_button_MAIN_DISABLED(style);
    }
    return style;
};

void init_style_no_shadow_button_MAIN_PRESSED(lv_style_t *style) {
    lv_style_set_shadow_width(style, 0);
};

lv_style_t *get_style_no_shadow_button_MAIN_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_no_shadow_button_MAIN_PRESSED(style);
    }
    return style;
};

void add_style_no_shadow_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_no_shadow_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_no_shadow_button_MAIN_DISABLED(), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_add_style(obj, get_style_no_shadow_button_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
};

void remove_style_no_shadow_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_no_shadow_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_no_shadow_button_MAIN_DISABLED(), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_remove_style(obj, get_style_no_shadow_button_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
};

//
// Style: no_shadow_keypad
//

void init_style_no_shadow_keypad_ITEMS_DEFAULT(lv_style_t *style) {
    lv_style_set_shadow_width(style, 0);
    lv_style_set_shadow_ofs_x(style, 0);
    lv_style_set_shadow_ofs_y(style, 0);
    lv_style_set_shadow_spread(style, 0);
    lv_style_set_shadow_color(style, lv_color_hex(0xffffffff));
    lv_style_set_shadow_opa(style, 255);
};

lv_style_t *get_style_no_shadow_keypad_ITEMS_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_no_shadow_keypad_ITEMS_DEFAULT(style);
    }
    return style;
};

void add_style_no_shadow_keypad(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_no_shadow_keypad_ITEMS_DEFAULT(), LV_PART_ITEMS | LV_STATE_DEFAULT);
};

void remove_style_no_shadow_keypad(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_no_shadow_keypad_ITEMS_DEFAULT(), LV_PART_ITEMS | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_no_shadow_button,
        add_style_no_shadow_keypad,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_no_shadow_button,
        remove_style_no_shadow_keypad,
    };
    remove_style_funcs[styleIndex](obj);
}