

# LVGL-Eve
This repository is an extension of EveApps, designed to provide LVGL support for reference purposes.  

For more information about LVGL, visit [LVGL.io](https://lvgl.io/). The original LVGL source code can be downloaded from [GitHub](https://github.com/lvgl/lvgl).   
The current LVGL version used is [V9.0.0](https://github.com/lvgl/lvgl/releases/tag/v9.0.0).  

This repository has been modified to support EVE chips and has been tested with various examples, including the "widget demo," "benchmark demo," and "music demo" on a Raspberry Pico RP2040 with a FT800 chip (IDM2040-21R board can be got from https://brtchip.com/product/idm2040-21R/).  

Users should have prior experience with LVGL.

## Dependency
This repository can't work alone, please download EveApps from https://github.com/Bridgetek/EveApps.   
The folder "common" of EveApps repo is required. 

## Build instructions

1. Download EveApps.
2. Download LVGL-Eve (this repo) and copy the Demo_lvgl folder to EveApps/
3. Change lvgl configuration in lv_conf depends on your project, such as enable log, enable demos.
4. Add the example or demo which needed to run in Demo_lvgl/eve_lvgl. Demo needs to be enabled in step 3.

```c
EVE_CoCmd_dlStart(s_pHalContext);
//lv_example_get_started_1();
lv_demo_widgets(); //enable the widget demo
//lv_demo_benchmark();
```

### Raspberry Pi Pico

Requires the Pico toolchain https://github.com/ndabas/pico-setup-windows to be installed.

Pico-SDK version 1.3.0 is required


The following steps will build for Raspberry Pi Pico.

 1. Install cmake 3.19.x, python, Visual Studio 2019 community (must select C++), GNU Arm Embedded Toolchain for window.
 2. Launch the *Developer Command Prompt for VS*
```sh
set PICO_SDK_PATH=[path to pico-sdk]
set PICO_TOOLCHAIN_PATH=[path to GNU Arm Embedded Toolchain\\10 2020-q4-major\\bin]
cd EveApps
mkdir build
cd build
cmake.exe -G "NMake Makefiles"  -DEVE_APPS_GRAPHICS=EVE_GRAPHICS_IDM2040_21R -DEVE_APPS_PLATFORM=MM2040EV -DEVE_APPS_DISPLAY=EVE_DISPLAY_WQVGA_480x480 -DCMAKE_BUILD_TYPE=Debug ..
nmake Demo_lvgl
```
## Demo reference
### Widgets
<img width="2889" height="2863" alt="image" src="https://github.com/user-attachments/assets/a154e89f-0169-41e9-b103-f04dfeb15604" />
<img width="2764" height="2673" alt="image" src="https://github.com/user-attachments/assets/d0e85630-bc2a-4585-a481-0dddb100f72a" />

## Limitation

At present, LVGL-Eve supports only fundamental functions, implying that not all necessary LVGL functions are available. For specific information regarding the LVGL examples supported in LVGL-Eve, please consult the documentation located at docs/LVGL_examples_supported_test_v1.0.pdf.

Unsupported cases:

1.	Rotation is solely applicable to images and does not extend to other objects like buttons or images with an outline.
2.	The functionality of the canvas is not available.

For cases with partial support, the following issues may be addressed in a future release:

1.	In the current version, shadows appear to function only for a single object. If multiple objects are present, the shadows do not display correctly.
2.	Scroll bars are observed more frequently in EVE's settings, prompting the need for further investigation.
3.	There exists a layout problem wherein certain objects extend beyond the outer box boundaries.
4.	The lines separating two lines of text are not visible.

To avoid underrun issues, it is necessary to reduce the complexity of demos due to the limitations of the display list.

## Others

This repository has taken following as a reference

https://github.com/juanjqh/lvgl_eve_gpu_test-main

## Version
This version is changed for IDM2040-21R demo based on v1.0.0-alpha

## Release log
v1.0.0-alpha
 - Initial version
 - Changed to v1.1.0 for IDM2040-43A demo
 - Changed to v1.2.0 for IDM2040-21R demo

