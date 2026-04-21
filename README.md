

# LVGL-Eve
This repository is an extension of EveApps, designed to provide LVGL support for reference purposes.  

For more information about LVGL, visit [LVGL.io](https://lvgl.io/). The original LVGL source code can be downloaded from [GitHub](https://github.com/lvgl/lvgl).   
The current LVGL version used is [V9.0.0](https://github.com/lvgl/lvgl/releases/tag/v9.0.0).  

This repository has been modified to support EVE chips and has been tested with various examples, including the "widget demo," "benchmark demo," and "music demo" on [IDM2040-7A](https://brtchip.com/product/idm2040-7a/) and [IDM2040-43A](https://brtchip.com/product/idm2040-43a/) which using Raspberry Pico RP2040 and EVE chip.  

Users should have prior experience with LVGL.
  
## Dependency
This repository cannot function independently; Bridgetek’s [EveApps](https://github.com/Bridgetek/EveApps) is included as a submodule.

## Build instructions

1. Download LVGL-Eve (this repo) 
2. Download and update submodule

```sh
git submodule update --init --recursive
git submodule update --remote --merge
```

3. add Demo_lvgl folder to External's top level CMakeLists.txt

<img width="343" height="143" alt="image" src="https://github.com/user-attachments/assets/d8cc6d97-fd1e-4a86-8528-155080bc86dd" />

4. Modify the lvgl configuration in Demo_lvgl/lv_conf.h according to your project's requirements, such as enabling logs, demos, etc.

```c
/*Enable the log module*/
#define LV_USE_LOG 1

/*Show some widget. It might be required to increase `LV_MEM_SIZE` */
#define LV_USE_DEMO_WIDGETS 1
```

5. (Optional) Adjust the display buffer in Demo_lvgl/eve_lvgl.c to match your project. It's recommended to use a screen-sized buffer if there is sufficient RAM available.

```c
void lv_setup(void)
{
    lv_display_t *display;
    static uint8_t buf1[HSIZE * 160];
```

6. Include the necessary example or demo that needs to run in Demo_lvgl/eve_lvgl.c. Ensure that the demo is enabled in step 4.

```c
EVE_CoCmd_dlStart(s_pHalContext);
//lv_example_get_started_1();
lv_demo_widgets(); //enable the widget demo
//lv_demo_benchmark();
```

### Raspberry Pi Pico

Requires the Pico toolchain https://github.com/ndabas/pico-setup-windows to be installed.

Pico-SDK version 2.2.0 is required


The following steps will build for Raspberry Pi Pico.

 1. Install cmake 3.19.x, python, Visual Studio 2019 community (must select C++), GNU Arm Embedded Toolchain for window.
 2. Launch the *Developer Command Prompt for VS*
```sh
set PICO_SDK_PATH=[path to pico-sdk]
set PICO_TOOLCHAIN_PATH=[path to GNU Arm Embedded Toolchain\\10 2020-q4-major\\bin]
cd external
mkdir build
cd build
cmake -G "NMake Makefiles" -DEVE_APPS_PLATFORM=EVE_PLATFORM_RP2040 -DEVE_APPS_GRAPHICS=[EVE graphics] -DEVE_APPS_DISPLAY=[EVE display] ..
nmake Demo_lvgl

```

Example: 
```
$ cmake.exe -G "NMake Makefiles" -DEVE_APPS_PLATFORM=EVE_PLATFORM_RP2040 -DEVE_APPS_GRAPHICS=EVE_GRAPHICS_BT817 -DEVE_APPS_DISPLAY=EVE_DISPLAY_WXGA ..
$ nmake Demo_lvgl
```

>[!NOTE]
>[EVE graphics] can be EVE or module name, such as BT817 for IDM2040-7A, BT883 for IDM2040-43A...
>
>[EVE display] EVE_DISPLAY_WVGA for IDM2040-7A, EVE_DISPLAY_WQVGA for IDM2040-43A...

The uf2 file can be found in 'LVGL-Eve\external\build\Demo_lvgl_build\'

<details>
<summary>Connections reference</summary>

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
</details>

### Emulator
1, Open Demo_lvgl/Msvc_Emulator/DemoLvgl_Emulator.sln

2, Change configurations as per your project

>[!NOTE]
>[EVE graphics] can be EVE or module name, such as BT817 for IDM2040-7A, BT883 for IDM2040-43A...
>
>[EVE display] EVE_DISPLAY_WVGA for IDM2040-7A, EVE_DISPLAY_WQVGA for IDM2040-43A...

3, Build (Ctrl + B) and Run (F5)

### FT4222 and MPSSE
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
