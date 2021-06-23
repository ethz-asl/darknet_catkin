cmake_minimum_required(VERSION 3.8)
project(darknet_catkin)

find_package(catkin_simple REQUIRED)
catkin_simple(ALL_DEPS_REQUIRED)

option(CMAKE_VERBOSE_MAKEFILE "Create verbose makefile" OFF)
option(CUDA_VERBOSE_BUILD "Create verbose CUDA build" OFF)
option(BUILD_AS_CPP "Build Darknet using C++ compiler also for C files" ON)
option(ENABLE_OPENCV "Enable OpenCV integration" ON)
option(ENABLE_CUDA "Enable CUDA support" OFF)
option(ENABLE_CUDNN "Enable CUDNN" OFF)
option(ENABLE_CUDNN_HALF "Enable CUDNN Half precision" OFF)

set(Darknet_MAJOR_VERSION 0)
set(Darknet_MINOR_VERSION 2)
set(Darknet_PATCH_VERSION 5)
set(Darknet_TWEAK_VERSION 1)
set(Darknet_VERSION ${Darknet_MAJOR_VERSION}.${Darknet_MINOR_VERSION}.${Darknet_PATCH_VERSION}.${Darknet_TWEAK_VERSION})

enable_language(C)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Install prefix" FORCE)
endif()

if(${CMAKE_VERSION} VERSION_LESS "3.9.0")
  message(WARNING "To build with CUDA support you need CMake 3.9.0+")
  set(ENABLE_CUDA "FALSE" CACHE BOOL "Enable CUDA support" FORCE)
else()
  include(CheckLanguage)
  check_language(CUDA)
  message(${CMAKE_CUDA_COMPILER})
  if(CMAKE_CUDA_COMPILER AND ENABLE_CUDA)
    set(CUDA_ARCHITECTURES "Auto" CACHE STRING "\"Auto\" detects local machine GPU compute arch at runtime, \"Common\" and \"All\" cover common and entire subsets of architectures, \"Names\" is a list of architectures to enable by name, \"Numbers\" is a list of compute capabilities (version number) to enable")
    set_property(CACHE CUDA_ARCHITECTURES PROPERTY STRINGS "Auto" "Common" "All" "Kepler Maxwell Kepler+Tegra Maxwell+Tegra Pascal" "3.0 7.5")
    enable_language(CUDA)
    find_package(CUDA REQUIRED)
    if(CUDA_VERSION VERSION_LESS "9.0")
      message(STATUS "Unsupported CUDA version, please upgrade to CUDA 9+. Disabling CUDA support")
      set(ENABLE_CUDA "FALSE" CACHE BOOL "Enable CUDA support" FORCE)
    else()
      cuda_select_nvcc_arch_flags(CUDA_ARCH_FLAGS ${CUDA_ARCHITECTURES})
      message(STATUS "Building with CUDA flags: " "${CUDA_ARCH_FLAGS}")
      if (NOT "arch=compute_75,code=sm_75" IN_LIST CUDA_ARCH_FLAGS)
        set(ENABLE_CUDNN_HALF "FALSE" CACHE BOOL "Enable CUDNN Half precision" FORCE)
        message(STATUS "Your setup does not supports half precision (it requires CC >= 7.5)")
      endif()
    endif()
  else()
    set(ENABLE_CUDA "FALSE" CACHE BOOL "Enable CUDA support" FORCE)
  endif()
endif()

set(Stb_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdparty/stb CACHE PATH "Path where Stb image library can be located")
find_package(Stb REQUIRED)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)
if(ENABLE_OPENCV)
  find_package(OpenCV)
  if(OpenCV_FOUND)
    if(SELECT_OPENCV_MODULES)
      if(TARGET opencv_world)
        list(APPEND OpenCV_LINKED_COMPONENTS "opencv_world")
      else()
        if(TARGET opencv_core)
          list(APPEND OpenCV_LINKED_COMPONENTS "opencv_core")
        endif()
        if(TARGET opencv_highgui)
          list(APPEND OpenCV_LINKED_COMPONENTS "opencv_highgui")
        endif()
        if(TARGET opencv_imgproc)
          list(APPEND OpenCV_LINKED_COMPONENTS "opencv_imgproc")
        endif()
        if(TARGET opencv_video)
          list(APPEND OpenCV_LINKED_COMPONENTS "opencv_video")
        endif()
        if(TARGET opencv_videoio)
          list(APPEND OpenCV_LINKED_COMPONENTS "opencv_videoio")
        endif()
        if(TARGET opencv_imgcodecs)
          list(APPEND OpenCV_LINKED_COMPONENTS "opencv_imgcodecs")
        endif()
        if(TARGET opencv_text)
          list(APPEND OpenCV_LINKED_COMPONENTS "opencv_text")
        endif()
      endif()
    else()
      list(APPEND OpenCV_LINKED_COMPONENTS ${OpenCV_LIBS})
    endif()
  endif()
endif()

#if(${CMAKE_VERSION} VERSION_LESS "3.11.0")
#  message(WARNING "To build with OpenMP support you need CMake 3.11.0+")
#else()
find_package(OpenMP)
#endif()

set(ADDITIONAL_CXX_FLAGS "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings")
set(ADDITIONAL_C_FLAGS "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings")

if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    if (UNIX AND NOT APPLE)
      set(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}")  #force pthread to avoid bugs in some cmake setups
      set(CMAKE_C_FLAGS "-pthread ${CMAKE_C_FLAGS}")
    endif()
  endif()
  set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${ADDITIONAL_C_FLAGS} ${CMAKE_C_FLAGS}")
  string(REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  string(REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  string(REGEX REPLACE "-O0" "-Og" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  string(REGEX REPLACE "-O3" "-Ofast" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffp-contract=fast -mavx -mavx2 -msse3 -msse4.1 -msse4.2 -msse4a")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffp-contract=fast -mavx -mavx2 -msse3 -msse4.1 -msse4.2 -msse4a")
endif()

if(ENABLE_CUDA)
  find_package(CUDNN)
  if(NOT CUDNN_FOUND)
    set(ENABLE_CUDNN "FALSE" CACHE BOOL "Enable CUDNN" FORCE)
  endif()
endif()

if(ENABLE_CUDA)
  set(CUDA_HOST_COMPILER_FLAGS "--compiler-options \" ${ADDITIONAL_CXX_FLAGS} -fPIC -fopenmp -Ofast \"")

  string (REPLACE ";" " " CUDA_ARCH_FLAGS_SPACE_SEPARATED "${CUDA_ARCH_FLAGS}")
  set(CMAKE_CUDA_FLAGS "${CUDA_ARCH_FLAGS_SPACE_SEPARATED} ${CUDA_HOST_COMPILER_FLAGS} ${CMAKE_CUDA_FLAGS}")
  message(STATUS "CMAKE_CUDA_FLAGS: ${CMAKE_CUDA_FLAGS}")
endif()

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/include/version.h.in"
  "${CMAKE_CURRENT_LIST_DIR}/include/version.h"
)

#look for all *.h files in src folder
file(GLOB headers "${CMAKE_CURRENT_LIST_DIR}/src/*.h")
#add also files in the include folder
list(APPEND headers
  ${CMAKE_CURRENT_LIST_DIR}/include/darknet.h
)

#look for all *.c files in src folder
file(GLOB sources "${CMAKE_CURRENT_LIST_DIR}/src/*.c" "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

#add also .cpp files
#list(APPEND sources
#  ${CMAKE_CURRENT_LIST_DIR}/src/http_stream.cpp
#  ${CMAKE_CURRENT_LIST_DIR}/src/image_opencv.cpp
#)

if(ENABLE_CUDA)
  file(GLOB cuda_sources "${CMAKE_CURRENT_LIST_DIR}/src/*.cu")
endif()

if(BUILD_AS_CPP)
  set_source_files_properties(${sources} PROPERTIES LANGUAGE CXX)
endif()

cs_add_library(dark ${sources} ${headers} ${cuda_sources})
set_target_properties(dark PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(ENABLE_CUDA)
  set_target_properties(dark PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
endif()
if(BUILD_AS_CPP)
  set_target_properties(dark PROPERTIES LINKER_LANGUAGE CXX)
endif()

target_include_directories(dark PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${Stb_INCLUDE_DIR}>)

if(CUDNN_FOUND)
  target_link_libraries(dark CuDNN::CuDNN)
  set(CUDNN TRUE)
  if(ENABLE_CUDNN_HALF)
    set(CUDNN_HALF TRUE)
  endif()
endif()

if(OpenCV_FOUND)
  target_link_libraries(dark ${OpenCV_LINKED_COMPONENTS})
  target_include_directories(dark PUBLIC ${OpenCV_INCLUDE_DIRS})
  set(OPENCV TRUE)
endif()

if(OPENMP_FOUND)
  target_link_libraries(dark OpenMP::OpenMP_CXX)
  target_link_libraries(dark OpenMP::OpenMP_C)
endif()

if(CMAKE_COMPILER_IS_GNUCC)
  target_link_libraries(dark m)
endif()

target_link_libraries(dark Threads::Threads)

if(ENABLE_CUDA)
  target_include_directories(dark PUBLIC ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
  target_link_libraries(dark curand cublas cuda)
  set_target_properties(dark PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
  set(GPU TRUE)
endif()

set_target_properties(dark PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_LIST_DIR}/include/darknet.h")

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/include/options.h.in"
  "${CMAKE_CURRENT_LIST_DIR}/include/options.h"
)

cs_install()
cs_export(INCLUDE_DIRS include ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
