cmake_minimum_required(VERSION 3.8)

project(GPUSim)

#Make sure we can include everything in project directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty")

# 2 : Change this to your OpenCL include path
include_directories("C:/Program Files (x86)/AMD APP SDK/3.0/include")
#include_directories("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/include")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty")

# 3 : Change this to your OpenCL lib path
link_directories("C:/Program Files (x86)/AMD APP SDK/3.0/lib/x86")
#link_directories("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/lib/Win32")

#Find all source, header, and shader files to add to build 
file(GLOB_RECURSE allCodeFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.hpp" "*.cpp")

add_executable(GPUSim WIN32 ${allCodeFiles})
target_compile_definitions(GPUSim PRIVATE GPUSimWin)

target_link_libraries(GPUSim OpenGL32)
target_link_libraries(GPUSim glew32)
target_link_libraries(GPUSim glew32s)
target_link_libraries(GPUSim glfw3)
target_link_libraries(GPUSim glfw3dll)
target_link_libraries(GPUSim OpenCL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set_target_properties(GPUSim PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

#Fix Visual studio source groups to correspond to the folder they're in
foreach(src ${allCodeFiles})
  get_filename_component(dir ${src} DIRECTORY)
  if(dir)
	string(REPLACE "/" "\\" fixedDir ${dir})
	source_group(${fixedDir} FILES ${src})
  else()
	source_group("\\\\" FILES ${src})
  endif()
endforeach()
