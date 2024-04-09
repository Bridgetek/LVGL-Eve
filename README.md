

# LVGL-Eve

This repository is an extention for EveApps to support LVGL for reference purposes.

For LVGL details, please refer to https://lvgl.io/. The original LVGL source code can be downloaded from https://github.com/lvgl/lvgl. Current LVGL version is V9.0.0. (https://github.com/lvgl/lvgl/releases/tag/v9.0.0)

This repository is already modified to support EVE chips, and tested with most of the examples and "widget demo", "benchmark demo", "music demo" on Raspberry Pico RP2040 with BT816 chip.

Due to code support needed from EVE's common files located under the "EveApps/common" folder. This repository can't work alone, please download EveApps from https://github.com/Bridgetek/EveApps

Users are expected to be familiar with LVGL.
  

## Build instructions

1. Download EveApps.
2. Download LVGL-Eve (this repo) and copy the Demo_lvgl folder to EveApps/
3. Change lvgl configuration in lv_conf depends on your project, such as enable log, enable demos.
4. Change HSIZE in Demo_lvgl/eve_lvgl to your LCD's height for display buffer. (optional, need further optimize) 
5. Add the example or demo which needed to run in Demo_lvgl/eve_lvgl. Demo need to be enabled in step 3.

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
cmake -G "NMake Makefiles" -DEVE_APPS_PLATFORM=EVE_PLATFORM_RP2040 -DEVE_APPS_GRAPHICS=[EVE graphics] ..
nmake Demo_lvgl

# [EVE graphics] can be EVE or module name, such as BT817, BT815, VM800B35A_BK ...
```

Example: 
```
$ cmake.exe -G "NMake Makefiles" -DEVE_APPS_PLATFORM=EVE_PLATFORM_RP2040 -DEVE_APPS_GRAPHICS=EVE_GRAPHICS_BT816 -DEVE_APPS_DISPLAY=EVE_DISPLAY_WXGA ..
$ nmake Demo_lvgl
```

Display resolution is set via `EVE_APPS_DISPLAY`, example: cmake -G "NMake Makefiles"  -DEVE_APPS_DISPLAY=EVE_DISPLAY_WXGA
By default, WVGA is set

EVE graphics is set via `EVE_APPS_GRAPHICS`, example: cmake -G "NMake Makefiles"  -DEVE_APPS_GRAPHICS=EVE_GRAPHICS_BT815
By default, MULTI is set

#### Connections

| RP2040 | EVE | UART | SD |
| --- | --- | --- | --- |
| GP0 (TX) | | RX (debug) | |
| GP1 (RX) | | TX (optional) | |
| GP2 (SPI0 SCK) | SCK | | |
| GP3 (SPI0 MOSI) | MOSI | | |
| GP4 (SPI0 MISO) | MISO | | |
| GP5 (GPIO) | CS | | |
| GP6 (GPIO) | INT | | |
| GP7 (GPIO) | PWD | | |
| 5V | 5V | | |
| GND | GND | | |

### Emulator, FT4222 and MPSSE
TBD

### FT9XX (FT93X and FT90X)
TBD

## Demo reference
### benchmark
![benchmark](https://github.com/BridgetekChip/LVGL-Eve/assets/142303696/2efc6797-ee26-4ca2-963a-022d0572b175)

### widgets
![widgets](https://github.com/BridgetekChip/LVGL-Eve/assets/142303696/1fad0a32-8004-4c25-a206-33f0de354de1)


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
This version is v1.0.0-alpha

## Release log
v1.0.0-alpha
 - Initial version