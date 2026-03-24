

# LVGL-Eve
This repository is an extension of EveApps, designed to provide LVGL support for reference purposes.  

For more information about LVGL, visit [LVGL.io](https://lvgl.io/). The original LVGL source code can be downloaded from [GitHub](https://github.com/lvgl/lvgl).   
The current LVGL version used is [V9.0.0](https://github.com/lvgl/lvgl/releases/tag/v9.0.0).  

This repository has been modified to support EVE chips and has been tested with various examples, including the "widget demo," "benchmark demo," and "music demo" on a Raspberry Pico RP2040 with a BT883 chip (IDM2040-43A board can be got from https://brtchip.com/product/idm2040-43a/).  

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
cmake.exe -G "NMake Makefiles"  -DEVE_APPS_GRAPHICS=EVE_GRAPHICS_BT883 -DEVE_APPS_PLATFORM=MM2040EV -DEVE_APPS_DISPLAY=EVE_DISPLAY_WQVGA  -DCMAKE_BUILD_TYPE=Debug ..
nmake Demo_lvgl
```
## Demo reference
### Widgets
<img width="3729" height="2246" alt="image" src="https://github.com/user-attachments/assets/302cbfce-4745-4962-84ad-a5d9e5d85684" />
<img width="3631" height="2196" alt="image" src="https://github.com/user-attachments/assets/d25c242e-60de-4dc4-a6dd-4bb012606906" />
<img width="3709" height="2192" alt="image" src="https://github.com/user-attachments/assets/46e3aa52-b3d2-4ea4-92a1-fc326a2b7ff8" />

### benchmark
<img width="3918" height="2323" alt="image" src="https://github.com/user-attachments/assets/62e13507-8bab-4d1f-a07e-44d8f5d00c08" />

### music
<img width="3551" height="2146" alt="image" src="https://github.com/user-attachments/assets/5b3a01b0-1aa8-4154-8104-5a7f184e581f" />
<img width="3496" height="2076" alt="image" src="https://github.com/user-attachments/assets/945e6be2-b26d-44e3-ab77-b1e235944bab" />

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
This version is changed for IDM2040-43A demo based on v1.0.0-alpha

## Release log
v1.0.0-alpha
 - Initial version
 - Changed to v1.1.0 for IDM2040-43A demo

