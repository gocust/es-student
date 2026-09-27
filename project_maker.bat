@echo off
set /p dirnm="insert project name "
echo %dirnm%
mkdir %dirnm%
cd %dirnm%
copy C:\Repositories\pico\es-student\base C:\Repositories\pico\es-student\%dirnm%
set "dirnm=%dirnm:-=_%"
(
echo cmake_minimum_required^(VERSION 3.15^)
echo.
echo set^(PICO_BOARD pico^)
echo set^(PICO_PLATFORM rp2040^)
echo.
echo include^(pico_sdk_import.cmake^)
echo.
echo project^(%dirnm%^)
echo.
echo pico_sdk_init^(^)
echo.
echo add_executable^(${PROJECT_NAME}
echo    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
echo ^)
echo.
echo target_link_libraries^(${PROJECT_NAME}
   echo pico_stdlib
echo ^)
echo.
echo pico_set_linker_script^(${PROJECT_NAME}
echo    ${CMAKE_CURRENT_SOURCE_DIR}/memmap_rp2040.ld
echo ^)
echo.
echo pico_add_extra_outputs^(${PROJECT_NAME}^)
) > CMakeLists.txt

echo goidovo
pause