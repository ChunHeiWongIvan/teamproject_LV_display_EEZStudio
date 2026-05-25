#include <stdio.h>
#include <math.h>

#include "lvgl/lvgl.h"
#include "actions.h"
#include "screens.h"
#include "vars.h"
#include "ui.h"

extern const char * get_var_constant_voltage_setpoint_text(void);
extern const char * get_var_constant_current_setpoint_text(void);

typedef enum
{
	CHARGER_IDLE_NC_BAT,
	CHARGER_IDLE_C_BAT,
    CHARGER_PRECHARGE,
    CHARGER_CHARGING,
    CHARGER_FAULT
} charger_state_t; // Charger states: Idle (battery not connected), Idle (battery connected), Pre-Charge, Charging, Fault

static volatile charger_state_t charger_state = CHARGER_IDLE_NC_BAT;

// Charts and variables for storing voltage/ current measurements received via UART
static lv_chart_series_t * s_o_v = NULL; // output voltage chart
static lv_chart_series_t * s_o_i = NULL; // output current chart
static lv_chart_series_t * s_o_p = NULL; // output power chart

static lv_chart_series_t * s_b_v = NULL; // battery voltage chart

static lv_chart_series_t * s_p_v = NULL; // PFC voltage chart
static lv_chart_series_t * s_p_i = NULL; // PFC current chart

static lv_chart_series_t * s_t_1 = NULL; // temperature 1 chart
static lv_chart_series_t * s_t_2 = NULL; // temperature 2 chart
static lv_chart_series_t * s_t_3 = NULL; // temperature 3 chart

static volatile float output_v = 0.0f; // output voltage
static volatile float output_i = 0.0f; // output current
static volatile float output_p = 0.0f; // output power
static volatile float battery_v = 0.0f; // battery voltage
static volatile float pfc_v = 0.0f; // PFC voltage
static volatile float pfc_i = 0.0f; // PFC current
static volatile float temp[3] = {0.0f}; // temperature 1, 2, 3

static lv_timer_t * charts_timer = NULL; // chart exclusive timer

// Simulated chart time
static float sim_time_s = 0.0f;

const char * get_var_voltage_text(float v) // Helper for displaying live voltage values
{
    static char buf[16];
    snprintf(buf,sizeof(buf),"%.1f V", v);
    return buf;
}

const char * get_var_current_text(float i) // Helper for displaying live current values
{
    static char buf[16];
    snprintf(buf,sizeof(buf),"%.3f A", i);
    return buf;
}

const char * get_var_power_text(float p) // Helper for displaying live power values
{
    static char buf[16];
    snprintf(buf,sizeof(buf),"%.3f kW", p);
    return buf;
}

const char * get_var_temperature_text(float t) // Helper for displaying live temperature values
{
    static char buf[16];
    snprintf(buf,sizeof(buf),"%.1f °C", t);
    return buf;
}

static void charts_init(void) // LVGL charts can only plot integers. Scaling used for decimals.
{
    // Output voltage chart
    lv_obj_t * c_o_v = objects.output_voltage_chart;
    lv_chart_set_point_count(c_o_v, 120); // last 120 samples on screen
    lv_chart_set_type(c_o_v, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(c_o_v, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
    lv_chart_set_axis_range(c_o_v, LV_CHART_AXIS_PRIMARY_Y, 0, 600*10); // volts (scaled by 10 for 1 d.p.)
    lv_obj_set_style_size(c_o_v, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
    lv_chart_set_div_line_count(c_o_v, 5+2, 3+2); // Chart grid setting
    s_o_v = lv_chart_add_series(c_o_v, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    // Output current chart
    lv_obj_t * c_o_i = objects.output_current_chart;
    lv_chart_set_point_count(c_o_i, 120); // last 120 samples on screen
    lv_chart_set_type(c_o_i, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(c_o_i, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
    lv_chart_set_axis_range(c_o_i, LV_CHART_AXIS_PRIMARY_Y, 0, 10*1000); // amps (scaled by 1000 for 3 d.p.)
    lv_obj_set_style_size(c_o_i, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
    lv_chart_set_div_line_count(c_o_i, 4+2, 3+2); // Chart grid setting
    s_o_i = lv_chart_add_series(c_o_i, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    // Output power chart
	lv_obj_t * c_o_p = objects.output_power_chart;
	lv_chart_set_point_count(c_o_p, 120); // last 120 samples on screen
	lv_chart_set_type(c_o_p, LV_CHART_TYPE_LINE);
	lv_chart_set_update_mode(c_o_p, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
	lv_chart_set_axis_range(c_o_p,LV_CHART_AXIS_PRIMARY_Y, 0, 4 * 1000); // kilowatts (scaled by 1000 for 3 d.p.)
	lv_obj_set_style_size(c_o_p, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
	lv_chart_set_div_line_count(c_o_p, 3+2, 3+2); // Chart grid setting
	s_o_p = lv_chart_add_series(c_o_p, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);

	// Battery voltage chart
	lv_obj_t * c_b_v = objects.battery_voltage_chart;
	lv_chart_set_point_count(c_b_v, 120); // last 120 samples on screen
	lv_chart_set_type(c_b_v, LV_CHART_TYPE_LINE);
	lv_chart_set_update_mode(c_b_v, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
	lv_chart_set_axis_range(c_b_v, LV_CHART_AXIS_PRIMARY_Y, 0, 600*10); // volts (scaled by 10 for 1 d.p.)
	lv_obj_set_style_size(c_b_v, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
	lv_chart_set_div_line_count(c_b_v, 5+2, 3+2); // Chart grid setting
	s_b_v = lv_chart_add_series(c_b_v, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

	// PFC voltage chart
	lv_obj_t * c_p_v = objects.pfc_voltage_chart;
	lv_chart_set_point_count(c_p_v, 120); // last 120 samples on screen
	lv_chart_set_type(c_p_v, LV_CHART_TYPE_LINE);
	lv_chart_set_update_mode(c_p_v, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
	lv_chart_set_axis_range(c_p_v, LV_CHART_AXIS_PRIMARY_Y, 0, 600*10); // volts (scaled by 10 for 1 d.p.)
	lv_obj_set_style_size(c_p_v, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
	lv_chart_set_div_line_count(c_p_v, 5+2, 3+2); // Chart grid setting
	s_p_v = lv_chart_add_series(c_p_v, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

	// PFC current chart
	lv_obj_t * c_p_i = objects.pfc_current_chart;
	lv_chart_set_point_count(c_p_i, 120); // last 120 samples on screen
	lv_chart_set_type(c_p_i, LV_CHART_TYPE_LINE);
	lv_chart_set_update_mode(c_p_i, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
	lv_chart_set_axis_range(c_p_i, LV_CHART_AXIS_PRIMARY_Y, 0, 10*1000); // amps (scaled by 1000 for 3 d.p.)
	lv_obj_set_style_size(c_p_i, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
	lv_chart_set_div_line_count(c_p_i, 4+2, 3+2); // Chart grid setting
	s_p_i = lv_chart_add_series(c_p_i, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

	// Temperature 1 chart
	lv_obj_t * c_t_1 = objects.temperature_chart_1;
	lv_chart_set_point_count(c_t_1, 120); // last 120 samples on screen
	lv_chart_set_type(c_t_1, LV_CHART_TYPE_LINE);
	lv_chart_set_update_mode(c_t_1, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
	lv_chart_set_axis_range(c_t_1, LV_CHART_AXIS_PRIMARY_Y, 0, 100*10); // degrees celsius (scaled by 10 for 1 d.p.)
	lv_obj_set_style_size(c_t_1, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
	lv_chart_set_div_line_count(c_t_1, 4+2, 3+2); // Chart grid setting
	s_t_1 = lv_chart_add_series(c_t_1, lv_palette_main(LV_PALETTE_BROWN), LV_CHART_AXIS_PRIMARY_Y);

	// Temperature 2 chart
	lv_obj_t * c_t_2 = objects.temperature_chart_2;
	lv_chart_set_point_count(c_t_2, 120); // last 120 samples on screen
	lv_chart_set_type(c_t_2, LV_CHART_TYPE_LINE);
	lv_chart_set_update_mode(c_t_2, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
	lv_chart_set_axis_range(c_t_2, LV_CHART_AXIS_PRIMARY_Y, 0, 100*10); // degrees celsius (scaled by 10 for 1 d.p.)
	lv_obj_set_style_size(c_t_2, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
	lv_chart_set_div_line_count(c_t_2, 4+2, 3+2); // Chart grid setting
	s_t_2 = lv_chart_add_series(c_t_2, lv_palette_main(LV_PALETTE_BROWN), LV_CHART_AXIS_PRIMARY_Y);

	// Temperature 3 chart
	lv_obj_t * c_t_3 = objects.temperature_chart_3;
	lv_chart_set_point_count(c_t_3, 120); // last 120 samples on screen
	lv_chart_set_type(c_t_3, LV_CHART_TYPE_LINE);
	lv_chart_set_update_mode(c_t_3, LV_CHART_UPDATE_MODE_SHIFT); // scrolling chart
	lv_chart_set_axis_range(c_t_3, LV_CHART_AXIS_PRIMARY_Y, 0, 100*10); // degrees celsius (scaled by 10 for 1 d.p.)
	lv_obj_set_style_size(c_t_3, 0, 0, LV_PART_INDICATOR); // remove chart dot for pure line chart
	lv_chart_set_div_line_count(c_t_3, 4+2, 3+2); // Chart grid setting
	s_t_3 = lv_chart_add_series(c_t_3, lv_palette_main(LV_PALETTE_BROWN), LV_CHART_AXIS_PRIMARY_Y);
}

static void charts_feed_cb(lv_timer_t * t)
{
    (void)t;

    sim_time_s += 0.5f; // timer period = 500 ms

    // --- Simulated charger data ---

    // Output voltage ramp: +20 V/min from 280 V
    output_v = 280.0f + (20.0f / 60.0f) * sim_time_s;

    if(output_v > 400.0f)
        output_v = 400.0f;

    // Constant-current phase
    output_i = 7.5f;

    // Add ±1% ripple, except temperature
    float r1 = 1.0f + 0.01f * sinf(2.0f * 3.14159f * 0.8f * sim_time_s);
    float r2 = 1.0f + 0.01f * sinf(2.0f * 3.14159f * 0.6f * sim_time_s + 1.2f);
    float r3 = 1.0f + 0.01f * sinf(2.0f * 3.14159f * 0.9f * sim_time_s + 2.0f);

    output_v *= r1;
    output_i *= r2;

    output_p = (output_v * output_i) / 1000.0f; // kW

    pfc_v = 400.0f * r3;

    battery_v = output_v;

    // 100% efficiency assumption
    pfc_i = (output_p * 1000.0f) / pfc_v;

    // Simulated temperatures
    temp[0] = 25.0f + 0.020f * sim_time_s;
    temp[1] = 24.0f + 0.015f * sim_time_s;
    temp[2] = 23.0f + 0.010f * sim_time_s;

    // Copy values locally
    float o_v = output_v;
    float o_i = output_i;
    float o_p = output_p;
    float b_v = battery_v;
    float p_v = pfc_v;
    float p_i = pfc_i;

    float temperature[3];
    temperature[0] = temp[0];
    temperature[1] = temp[1];
    temperature[2] = temp[2];

    // Clamp to chart ranges
    if(o_v < 0) o_v = 0;
    if(o_v > 600) o_v = 600;

    if(o_i < 0) o_i = 0;
    if(o_i > 10) o_i = 10;

    if(o_p < 0) o_p = 0;
    if(o_p > 4) o_p = 4;

    if(b_v < 0) b_v = 0;
    if(b_v > 600) b_v = 600;

    if(p_v < 0) p_v = 0;
    if(p_v > 600) p_v = 600;

    if(p_i < 0) p_i = 0;
    if(p_i > 10) p_i = 10;

    for(int k = 0; k < 3; k++) {
        if(temperature[k] < 0) temperature[k] = 0;
        if(temperature[k] > 100) temperature[k] = 100;
    }

    // Set chart series
    lv_chart_set_next_value(objects.output_voltage_chart, s_o_v, (int32_t)(o_v * 10.0f + 0.5f));
    lv_chart_set_next_value(objects.output_current_chart, s_o_i, (int32_t)(o_i * 1000.0f + 0.5f));
    lv_chart_set_next_value(objects.output_power_chart,   s_o_p, (int32_t)(o_p * 1000.0f + 0.5f));

    lv_chart_set_next_value(objects.battery_voltage_chart, s_b_v, (int32_t)(b_v * 10.0f + 0.5f));
    lv_chart_set_next_value(objects.pfc_voltage_chart,     s_p_v, (int32_t)(p_v * 10.0f + 0.5f));
    lv_chart_set_next_value(objects.pfc_current_chart,     s_p_i, (int32_t)(p_i * 1000.0f + 0.5f));

    lv_chart_set_next_value(objects.temperature_chart_1, s_t_1, (int32_t)(temperature[0] * 10.0f + 0.5f));
    lv_chart_set_next_value(objects.temperature_chart_2, s_t_2, (int32_t)(temperature[1] * 10.0f + 0.5f));
    lv_chart_set_next_value(objects.temperature_chart_3, s_t_3, (int32_t)(temperature[2] * 10.0f + 0.5f));

    // Set chart labels
    lv_label_set_text(objects.output_voltage_label, get_var_voltage_text(o_v));
    lv_label_set_text(objects.output_current_label, get_var_current_text(o_i));
    lv_label_set_text(objects.output_power_label,   get_var_power_text(o_p));

    lv_label_set_text(objects.battery_voltage_label, get_var_voltage_text(b_v));
    lv_label_set_text(objects.pfc_voltage_label,     get_var_voltage_text(p_v));
    lv_label_set_text(objects.pfc_current_label,     get_var_current_text(p_i));

    lv_label_set_text(objects.temperature_label_1, get_var_temperature_text(temperature[0]));
    lv_label_set_text(objects.temperature_label_2, get_var_temperature_text(temperature[1]));
    lv_label_set_text(objects.temperature_label_3, get_var_temperature_text(temperature[2]));

    // Also update main menu voltage/ current labels
    if (charger_state == CHARGER_CHARGING) {
        lv_label_set_text(objects.main_menu_voltage_label,
                        get_var_voltage_text(output_v));
        lv_label_set_text(objects.main_menu_current_label,
                        get_var_current_text(output_i));    
    } else if (charger_state == CHARGER_FAULT) {
        lv_label_set_text(objects.main_menu_voltage_label,
                        "0.0 V");
        lv_label_set_text(objects.main_menu_current_label,
                        "0.000 A");
    }
}

void start_charts(void)
{
    charts_init();
    if(charts_timer == NULL) {
        charts_timer = lv_timer_create(charts_feed_cb, 500, NULL); // 500 ms timer for updating charts
    }
}

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

void action_demo_change_state(lv_event_t * e)
{
    charger_state++;

    if(charger_state > CHARGER_FAULT)
    {
        charger_state = CHARGER_IDLE_NC_BAT;
    }

    switch(charger_state) // Charger state machine
	  {
	      case CHARGER_IDLE_NC_BAT:

	    	  // Status widget
	          lv_label_set_text(objects.status_label,
	                            "IDLE");
	          lv_label_set_text(objects.detailed_status_label,
								"Battery not connected");
	          lv_obj_set_style_bg_color(objects.status_container,
	                                    lv_color_hex(0x464646),
	                                    0);

	          // Parameters widget (unlocked)
	          lv_label_set_text(objects.parameters_label,
								"Parameters");

	          // Bottom left stats widget
	          lv_label_set_text(objects.bottom_left_stat_desc,
								"Battery Voltage");
	          lv_label_set_text(objects.main_menu_voltage_label,
								"0.0 V");
	          lv_label_set_text(objects.main_menu_current_label,
								"");

	          // Set parameters button (unlocked)
	          lv_obj_remove_state(objects.set_parameters_button, LV_STATE_DISABLED);

	          break;

	      case CHARGER_IDLE_C_BAT:

	    	  // Status widget
	      	  lv_label_set_text(objects.status_label,
								"IDLE");
	      	  lv_label_set_text(objects.detailed_status_label,
								"Battery connected");
	      	  lv_obj_set_style_bg_color(objects.status_container,
										lv_color_hex(0x464646),
										0);

	      	  // Parameters widget (unlocked)
	      	  lv_label_set_text(objects.parameters_label,
								"Parameters");

	      	  // Bottom left stats widget
	      	  lv_label_set_text(objects.bottom_left_stat_desc,
								"Battery Voltage");
	      	  lv_label_set_text(objects.main_menu_voltage_label,
								"280.0 V");
			  lv_label_set_text(objects.main_menu_current_label,
								"");

	      	  // Set parameters button (unlocked)
	      	  lv_obj_remove_state(objects.set_parameters_button, LV_STATE_DISABLED);
	      	  lv_label_set_text(objects.set_parameters_label,
								"Set\nParameters");

			  break;

	      case CHARGER_PRECHARGE:

	    	  // Status widget
	          lv_label_set_text(objects.status_label,
	                            "PRE-CHARGE");
	          lv_label_set_text(objects.detailed_status_label,
								"Battery connected");
	          lv_obj_set_style_bg_color(objects.status_container,
	                                    lv_color_hex(0xFF8C00),
	                                    0);

	          // Parameters widget (locked)
	          lv_label_set_text(objects.parameters_label,
								"Parameters\n(locked)");
	          if(lv_screen_active() == objects.set_parameters_1 || lv_screen_active() == objects.set_parameters_2)
	          {
	              lv_screen_load(objects.main_menu); // Kicks user out of settings if state changes while in settings
	          }

	          // Bottom left stats widget
	          lv_label_set_text(objects.bottom_left_stat_desc,
								"Battery Voltage");
	          lv_label_set_text(objects.main_menu_voltage_label,
								"280.0 V");
			  lv_label_set_text(objects.main_menu_current_label,
								"");

	          // Set parameters button (locked)
			  lv_obj_add_state(objects.set_parameters_button, LV_STATE_DISABLED);
			  lv_label_set_text(objects.set_parameters_label,
								"Parameters\nlocked");

	          break;

	      case CHARGER_CHARGING:

	    	  // Status widget
	          lv_label_set_text(objects.status_label,
	                            "CHARGING");
	          lv_label_set_text(objects.detailed_status_label,
								"Battery connected");
	          lv_obj_set_style_bg_color(objects.status_container,
	                                    lv_color_hex(0x008000),
	                                    0);

	          // Parameters widget (locked)
	          lv_label_set_text(objects.parameters_label,
								"Parameters\n(locked)");
	          if(lv_screen_active() == objects.set_parameters_1 || lv_screen_active() == objects.set_parameters_2)
	          {
	              lv_screen_load(objects.main_menu); // Kicks user out of settings if state changes while in settings
	          }

	          // Bottom left stats widget
	          lv_label_set_text(objects.bottom_left_stat_desc,
								"Output");
	          lv_label_set_text(objects.main_menu_voltage_label,
								get_var_voltage_text(output_v));
			  lv_label_set_text(objects.main_menu_current_label,
					  	  	  	get_var_current_text(output_i));

	          // Set parameters button (locked)
			  lv_obj_add_state(objects.set_parameters_button, LV_STATE_DISABLED);
			  lv_label_set_text(objects.set_parameters_label,
								"Parameters\nlocked");

	          break;

	      case CHARGER_FAULT:

	    	  // Status widget
	          lv_label_set_text(objects.status_label,
	                            "FAULT");
	          lv_label_set_text(objects.detailed_status_label,
								"Shutdown circuit opened");
	          lv_obj_set_style_bg_color(objects.status_container,
	                                    lv_color_hex(0xB40000),
	                                    0);

	          // Parameters widget (locked)
	          lv_label_set_text(objects.parameters_label,
								"Parameters\n(locked)");
	          if(lv_screen_active() == objects.set_parameters_1 || lv_screen_active() == objects.set_parameters_2)
	          {
	              lv_screen_load(objects.main_menu); // Kicks user out of settings if state changes while in settings
	          }

	          // Bottom left stats widget
	          lv_label_set_text(objects.bottom_left_stat_desc,
								"Output");
	          lv_label_set_text(objects.main_menu_voltage_label,
								get_var_voltage_text(output_v));
			  lv_label_set_text(objects.main_menu_current_label,
								get_var_current_text(output_i));

	          // Set parameters button (locked)
			  lv_obj_add_state(objects.set_parameters_button, LV_STATE_DISABLED);
			  lv_label_set_text(objects.set_parameters_label,
								"Parameters\nlocked");

	          break;
	  }
}

void action_demo_start_charts(lv_event_t * e)
{
    start_charts();
}

void action_to_test(lv_event_t * e)
{
    
}
