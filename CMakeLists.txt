cmake_minimum_required (VERSION 3.8)
project (GPUSim)

add_subdirectory("source")

# 1 : Change to your OpenCL.dll path
file(COPY "C:/Program Files (x86)/AMD APP SDK/3.0/bin/x86/OpenCL.dll" DESTINATION "${CMAKE_BINARY_DIR}")
#file(COPY "C:/Program Files/NVIDIA Corporation/OpenCL" DESTINATION "${CMAKE_BINARY_DIR}")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glew32.dll" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glfw3.dll" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/opengl32.dll" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}")