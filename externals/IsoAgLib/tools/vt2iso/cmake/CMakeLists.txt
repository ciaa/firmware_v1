cmake_minimum_required(VERSION 2.8.1)

#message(STATUS "(. ${.})")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(VT2ISO)

add_definitions(
  -DAPP_NO_THREADS
  -DXML_USE_NO_THREADS
  -D_CRT_SECURE_NO_WARNINGS
  -D_SCL_SECURE_NO_WARNINGS)

include_directories(
  ../src
  ../../libs/freeimage)

if(WIN32)
  set(BOOST_ROOT "../../libs/boost/boost_1_38_0")
  include_directories(
    ${BOOST_ROOT}
    ../../libs/xerces-c/xerces-c_2_5_0-windows_nt-msvc_60/include)
  link_directories(
    ../../libs/xerces-c/xerces-c_2_5_0-windows_nt-msvc_60/lib
    ../../libs/freeimage)
  set(ISOAGLIB_ADDITIONAL_LIBRARIES User32 Gdi32 Ws2_32 xerces-c_2 Advapi32)
else()
  link_directories(
    ../../libs/freeimage)
  set(ISOAGLIB_ADDITIONAL_LIBRARIES xerces-c)
endif(WIN32)


find_package(Boost REQUIRED)

add_executable(
  vt2iso
  ../src/vt2iso.cpp
  ../src/vt2iso-defines.cpp
  ../src/vt2iso-globals.cpp
  ../src/vt2iso.cpp
  ../src/vt2isoimagebase_c.cpp
  ../src/vt2isoimagefreeimage_c.cpp)

target_link_libraries(vt2iso freeimage ${ISOAGLIB_ADDITIONAL_LIBRARIES})
