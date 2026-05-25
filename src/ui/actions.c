#include "actions.h"
#include "screens.h"
#include "vars.h"
#include "ui.h"

extern const char * get_var_constant_voltage_setpoint_text(void);
extern const char * get_var_constant_current_setpoint_text(void);

static int32_t clamp_i32(int32_t x, int32_t lo, int32_t hi) // Helper clamp function, int32
{
    if(x < lo) return lo;
    if(x > hi) return hi;
    return x;
}

static float clamp_float(float x, float lo, float hi) // Helper clamp function, float
{
    if(x < lo) return lo;
    if(x > hi) return hi;
    return x;
}

static int32_t voltage_to_bar(int32_t v, int32_t MIN, int32_t MAX) // Helper to convert voltage into bar percentage
{
    v = clamp_i32(v, MIN, MAX);

    // 0 to 100
    return (int32_t)((v - MIN) * 100 / (MAX - MIN));
}

static float current_to_bar(float i, float MIN, float MAX) // Helper to convert current into bar percentage
{
    i = clamp_float(i, MIN, MAX);

    // 0 to 100
    return (float)((i - MIN) * 100 / (MAX - MIN));
}

static float get_current_max_for_voltage(float voltage) // Helper for calculating max current depending on voltage
{
    if(voltage <= 400.0f)
    {
        return 7.50f;
    }

    // 3 kW power limit: I = P / V
    return 3000.0f / voltage;
}

void action_to_charging_menu_1(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_1);
}

void action_to_charging_menu_2(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_2);
}

void action_to_charging_menu_3(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_3);
}

void action_to_charging_menu_4(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_4);
}

void action_to_charging_menu_5(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_5);
}

void action_to_charging_menu_6(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_6);
}

void action_to_charging_menu_7(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_7);
}

void action_to_charging_menu_8(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_8);
}

void action_to_charging_menu_9(lv_event_t * e)
{
    lv_screen_load(objects.charging_menu_9);
}

void action_to_set_parameters_1(lv_event_t * e)
{
    lv_screen_load(objects.set_parameters_1);
}

void action_to_set_parameters_2(lv_event_t * e)
{
    lv_screen_load(objects.set_parameters_2);
}

void action_to_view_debug(lv_event_t * e)
{
    lv_screen_load(objects.view_debug);
}

void action_to_main_menu(lv_event_t * e)
{
    lv_screen_load(objects.main_menu);
}

void action_set_parameters_keypad_voltage(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t * btnm = lv_event_get_target_obj(e);

    // 0-based ID, not counting "\n" in the map
    uint32_t id = lv_buttonmatrix_get_selected_button(btnm);
    if(id == LV_BUTTONMATRIX_BUTTON_NONE) return;

    int32_t v = get_var_constant_voltage_setpoint();

    const int32_t V_MIN = 200; // Minimum full charge voltage of 200 V
    const int32_t V_MAX = 600; // Maximum full charge voltage of 600 V
    const float I_MIN = 0.0f; // Minimum current of 0.0 A

    switch(id) {
        case 0:  v += 1;    break;     // +1 V
        case 1:  v += 10;   break;     // +10 V
        case 2:  v += 100;  break;     // +100 V
        case 3:  v -= 1;    break;     // -1 V
        case 4:  v -= 10;   break;     // -10 V
        case 5:  v -= 100;  break;     // -100 V

        case 6:  v = V_MIN; break;     // "Set min: 200 V"
        case 7:  v = V_MAX; break;     // "Set max: 600 V"

        default: break;
    }

    // Clamp
    v = (float) clamp_i32(v, V_MIN, V_MAX);

    set_var_constant_voltage_setpoint(v);

    // Update all labels (params screen/ main menu)
    lv_label_set_text(objects.constant_voltage_setpoint_label,
                      get_var_constant_voltage_setpoint_text());
    lv_label_set_text(objects.main_menu_cv_label,
                      get_var_constant_voltage_setpoint_text());
    lv_label_set_text(objects.constant_voltage_setpoint_label_1,
                      get_var_constant_voltage_setpoint_text());

    // Update both bars (0 to 100)
    lv_bar_set_value(objects.constant_voltage_setpoint_bar,
                     voltage_to_bar(v, V_MIN, V_MAX),
                     LV_ANIM_ON);
    lv_bar_set_value(objects.constant_voltage_setpoint_bar_1,
                     voltage_to_bar(v, V_MIN, V_MAX),
                     LV_ANIM_ON);

    // Reclamp current as current max might decrease after voltage change
    float i = get_var_constant_current_setpoint();
    float I_MAX = get_current_max_for_voltage((float)v);

    i = clamp_float(i, I_MIN, I_MAX);
    set_var_constant_current_setpoint(i);

    // Update all labels (params screen/ main menu)
    lv_label_set_text(objects.constant_current_setpoint_label,
                      get_var_constant_current_setpoint_text());
    lv_label_set_text(objects.main_menu_cc_label,
                      get_var_constant_current_setpoint_text());
    lv_label_set_text(objects.constant_current_setpoint_label_1,
                      get_var_constant_current_setpoint_text());

    // Update both bars (0 to 100)                      
    lv_bar_set_value(objects.constant_current_setpoint_bar, 
                     current_to_bar(i, I_MIN, I_MAX), 
                     LV_ANIM_ON);
    lv_bar_set_value(objects.constant_current_setpoint_bar_1, 
                     current_to_bar(i, I_MIN, I_MAX), 
                     LV_ANIM_ON);                     

}

void action_set_parameters_keypad_current(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t * btnm = lv_event_get_target_obj(e);

    uint32_t id = lv_buttonmatrix_get_selected_button(btnm);
    if(id == LV_BUTTONMATRIX_BUTTON_NONE) return;

    float i = get_var_constant_current_setpoint();

    const float I_MIN = 0.0f;

    float voltage = (float)get_var_constant_voltage_setpoint();
    float I_MAX = get_current_max_for_voltage(voltage);

    switch(id) {
        case 0:  i += 0.01f;  break;
        case 1:  i += 0.10f;  break;
        case 2:  i += 1.00f;  break;
        case 3:  i -= 0.01f;  break;
        case 4:  i -= 0.10f;  break;
        case 5:  i -= 1.00f;  break;

        case 6:  i = I_MIN;   break;
        case 7:  i = I_MAX;   break;

        default: break;
    }

    // Clamp using voltage-dependent max
    i = clamp_float(i, I_MIN, I_MAX);

    set_var_constant_current_setpoint(i);

    // Update all labels (params screen/ main menu)
    lv_label_set_text(objects.constant_current_setpoint_label,
                      get_var_constant_current_setpoint_text());
    lv_label_set_text(objects.main_menu_cc_label,
                      get_var_constant_current_setpoint_text());
    lv_label_set_text(objects.constant_current_setpoint_label_1,
                      get_var_constant_current_setpoint_text());

    // Update both bars (0 to 100)                      
    lv_bar_set_value(objects.constant_current_setpoint_bar, 
                     current_to_bar(i, I_MIN, I_MAX), 
                     LV_ANIM_ON);
    lv_bar_set_value(objects.constant_current_setpoint_bar_1, 
                     current_to_bar(i, I_MIN, I_MAX), 
                     LV_ANIM_ON);
}

void action_to_test(lv_event_t * e)
{
    
}
