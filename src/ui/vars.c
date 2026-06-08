#include "vars.h"
#include <stdio.h>

float constant_voltage_setpoint = 200.0f;
float constant_current_setpoint = 1.000f;

// For constant voltage (CV) setpoint
float get_var_constant_voltage_setpoint() {
    return constant_voltage_setpoint;
}

const char * get_var_constant_voltage_setpoint_text(void)
{
    static char buf[16];
    snprintf(buf, sizeof(buf), "%.1f V",constant_voltage_setpoint);
    return buf;
}

void set_var_constant_voltage_setpoint(float value) {
    constant_voltage_setpoint = value;
}

// For constant current (CC) setpoint
float get_var_constant_current_setpoint() {
    return constant_current_setpoint;
}

const char * get_var_constant_current_setpoint_text(void)
{
    static char buf[16];

    snprintf(buf,sizeof(buf),"%.3f A",constant_current_setpoint);

    return buf;
}

void set_var_constant_current_setpoint(float value) {
    constant_current_setpoint = value;
}