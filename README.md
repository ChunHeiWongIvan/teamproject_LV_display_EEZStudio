# Formula Student Charger LV/ Human-Machine Interface (HMI) GUI

This repository contains the EEZ Studio project used to design and edit the LVGL touchscreen interface for a Formula Student EV battery charger.

The interface was developed visually in EEZ Studio, with the generated LVGL source files integrated into the STM32 firmware running on an STM32G474RE.

## Relationship to the Main Repository

This repository contains the GUI design project and generated LVGL interface code.

The complete embedded implementation, including the STM32 firmware, display drivers, touchscreen control, charger state logic, communications, diagnostics, and integration of the generated GUI, is maintained in the [main LV display repository](https://github.com/ChunHeiWongIvan/teamproject_LV_display_G474RE).

## Tools

- EEZ Studio
- LVGL 9
- STM32G474RE
