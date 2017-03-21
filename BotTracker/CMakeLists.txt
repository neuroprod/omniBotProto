cmake_minimum_required(VERSION 3.6)
project(BotTracker)

set(CMAKE_CXX_STANDARD 11)



# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
    # Add OpenCV headers location to your include paths
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()



include_directories(/opt/vc/include)
include_directories(/opt/vc/userland-master/interface/vcos)
include_directories(/opt/vc/userland-master)
include_directories(/opt/vc/userland-master/interface/vcos/pthreads)
include_directories(/opt/vc/userland-master/interface/vmcs_host/linux)
include_directories(/opt/vc/userland-master/host_applications/linux/libs/bcm_host/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glm)
link_directories(/opt/vc/lib)

set(SOURCE_FILES main.cpp graphics.cpp camera/camera.cpp camera/cameracontrol.cpp mmalincludes.h BotTracker.cpp BotTracker.h CameraHandler.cpp CameraHandler.h GLWindow.cpp GLWindow.h GLProgram.cpp GLProgram.h GLTexture.cpp GLTexture.h)

add_executable(untitled ${SOURCE_FILES})

target_link_libraries(untitled ${OpenCV_LIBS} ${SDL2_Libs} librf24-bcm.so libmmal_core.so libmmal_util.so libmmal_vc_client.so libvcos.so libbcm_host.so GLESv2 EGL  ${OpenCV_LIBS})
