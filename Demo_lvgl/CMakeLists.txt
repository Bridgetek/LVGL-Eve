LINK_LIBRARIES(eve_hal)

set_directory_properties(PROPERTIES FOLDER Demo_lvgl)

IF((IS_PLATFORM_FT9XX) OR (IS_PLATFORM_RP2040))
  LINK_LIBRARIES(fatfs)
  INCLUDE_DIRECTORIES("../common/application/fatfs/source")
ENDIF()

INCLUDE_DIRECTORIES("../common/application")  

FILE(GLOB SRCS ../common/application/*.c *.c)
FILE(GLOB HDRS ../common/application/*.h *.h)

get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})

MESSAGE(STATUS "Configuring target: ${ProjectId}")

project(${ProjectId} LANGUAGES C CXX)

ADD_EXECUTABLE(${ProjectId} ${SRCS} ${HDRS})

# Specify path to own LVGL config header
set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h
    CACHE STRING "" FORCE)

add_definitions("-DLV_USE_DRAW_EVE")
add_definitions("-DEVE_ADAPTIVE_FRAMERATE")

ADD_SUBDIRECTORY(lvgl)
target_link_libraries(${ProjectId} PRIVATE lvgl::lvgl lvgl::demos lvgl::examples)

IF(IS_PLATFORM_FT9XX)
  add_custom_command(
      TARGET "${ProjectId}"
      POST_BUILD
      COMMAND "${CMAKE_OBJCOPY}" --output-target binary ${ProjectId} "${ProjectId}.bin"
  )
ENDIF()

IF(IS_PLATFORM_RP2040)
  pico_enable_stdio_uart(${ProjectId} 1)
  pico_enable_stdio_usb(${ProjectId} 1) # Enable Virtual COM port
  pico_add_extra_outputs(${ProjectId})
ENDIF()

# Deployment ###################################################################
SET(path_deploy "${CMAKE_BINARY_DIR}/deploy/${ProjectId}")
SET(path_exe "${path_deploy}/executable" )
SET(path_test "${CMAKE_CURRENT_SOURCE_DIR}/Test" )

IF (WIN32)
    SET(exe_filename "${ProjectId}.exe")
    SET(path_exe "${path_deploy}/executable/1/2" ) # it need to access ../../../Test/image...
ELSEIF(IS_PLATFORM_FT9XX)
    SET(exe_filename "${ProjectId}.bin")
ELSEIF(IS_PLATFORM_RP2040)
    SET(exe_filename "${ProjectId}.uf2")
ENDIF()

add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${path_deploy}" )
add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${path_exe}" )

if(EXISTS "${path_test}")
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${path_test}" "${path_deploy}/Test" )
    if(EXISTS "${path_test}/Flash/BT81X_Flash.bin" AND NOT EXISTS "${path_exe}/__Flash.bin")
        add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E rename  "${path_deploy}/Test/Flash/BT81X_Flash.bin" "${path_exe}/__Flash.bin")
    endif()
endif()

add_custom_command(TARGET "${ProjectId}"  POST_BUILD  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${ProjectId}> "${path_exe}/${exe_filename}")    
  
IF (WIN32)
  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x64/libMPSSE.dll "${path_exe}")
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x64/LibFT4222-64.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x64/ftd2xx.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x64/bt8xxemu.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x64/mx25lemu.dll "${path_exe}")   
  ELSE()
      
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x86/libMPSSE.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x86/LibFT4222.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/MSVC/x86/ftd2xx.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x86/bt8xxemu.dll "${path_exe}")   
    add_custom_command(TARGET "${ProjectId}"  PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/common/eve_hal/Bin/Simulation/x86/mx25lemu.dll "${path_exe}")   
  ENDIF()
ENDIF()
