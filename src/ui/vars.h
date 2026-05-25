#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_CONSTANT_VOLTAGE_SETPOINT = 0,
    FLOW_GLOBAL_VARIABLE_CONSTANT_CURRENT_SETPOINT = 1
};

// Native global variables

extern float get_var_constant_voltage_setpoint();
extern void set_var_constant_voltage_setpoint(float value);
extern float get_var_constant_current_setpoint();
extern void set_var_constant_current_setpoint(float value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/