cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(CamCon)

include_directories(include)

add_library(deps src/deps_dummy.cpp)
target_link_libraries(deps dxva2 evr mf mfplat mfplay mfreadwrite mfuuid strmiids)

file(GLOB mf-wrapper_SRC src/mf_wrapper/*.cpp)
add_library(mf-wrapper ${mf-wrapper_SRC})
target_link_libraries(mf-wrapper deps)

file(GLOB system_SRC src/system/*.cpp)
add_library(system ${system_SRC})

add_executable(camcon src/main.cpp)
target_link_libraries(camcon deps)

add_executable(list-dev src/tools/list_dev.cpp)
target_link_libraries(list-dev deps mf-wrapper)

add_executable(get-current-cam-config src/tools/get_current_cam_config.cpp)
target_link_libraries(get-current-cam-config deps mf-wrapper)

add_executable(set-current-cam-config src/tools/set_current_cam_config.cpp)
target_link_libraries(set-current-cam-config deps mf-wrapper system)