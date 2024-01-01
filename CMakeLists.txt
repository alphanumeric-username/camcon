cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "C:/Users/filipe/Libraries")

project(CamCon)

include_directories(include)

# find_package(SFML COMPONENTS system graphics window CONFIG REQUIRED)

add_library(deps src/deps_dummy.cpp)
target_link_libraries(deps dxva2 evr mf mfplat mfplay mfreadwrite mfuuid strmiids comctl32 gdiplus)

file(GLOB system_SRC src/system/*.cpp)
add_library(system ${system_SRC})

file(GLOB mf-wrapper_SRC src/mf_wrapper/*.cpp)
add_library(mf-wrapper ${mf-wrapper_SRC})
target_link_libraries(mf-wrapper deps system)

file(GLOB win32-wrapper_SRC src/win32_wrapper/*.cpp src/win32_wrapper/gdi/*.cpp src/win32_wrapper/layout/*.cpp)
add_library(win32-wrapper ${win32-wrapper_SRC})
target_link_libraries(win32-wrapper deps)

file(GLOB camcon_SRC src/camcon/*.cpp)
add_library(camcon ${camcon_SRC})
target_link_libraries(camcon mf-wrapper)

# file(GLOB ui_SRC src/ui/*.cpp)
# add_library(ui ${ui_SRC})
# target_link_libraries(ui sfml-graphics)

# add_executable(camcon src/main.cpp)
# target_link_libraries(camcon deps)

add_executable(list-dev src/tools/list_dev.cpp)
target_link_libraries(list-dev deps mf-wrapper)

add_executable(get-current-cam-config src/tools/get_current_cam_config.cpp)
target_link_libraries(get-current-cam-config deps mf-wrapper system)

add_executable(set-current-cam-config src/tools/set_current_cam_config.cpp)
target_link_libraries(set-current-cam-config deps mf-wrapper system)

add_executable(rt-control WIN32 src/tools/rt_control.cpp)
target_link_libraries(rt-control deps win32-wrapper mf-wrapper camcon)

add_compile_definitions(UNICODE)

# add_executable(dev-probe src/tools/dev_probe.cpp)
# target_link_libraries(dev-probe deps mf-wrapper system sfml-graphics)

# add_executable(rt-control src/tools/rt_control.cpp)
# target_link_libraries(rt-control deps mf-wrapper system sfml-graphics)

# file(GLOB SFML_DLL ${CMAKE_PREFIX_PATH}/SFML/bin/*.dll)
# file(COPY ${SFML_DLL} DESTINATION Debug)
# file(COPY assets DESTINATION Debug)
# file(COPY ${SFML_DLL} DESTINATION Release)
# file(COPY assets DESTINATION Release)