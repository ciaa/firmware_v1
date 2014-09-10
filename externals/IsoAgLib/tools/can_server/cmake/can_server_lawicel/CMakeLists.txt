cmake_minimum_required(VERSION 2.8.1)

#message(STATUS "(. ${.})")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(CAN_SERVER_LAWICEL)

add_definitions(
  -DCAN_DRIVER_SOCKET)

set(ISOAGLIB_ADDITIONAL_RELEASE_DEFINITIONS "-DDEBUG_CANSERVER=0")
set(ISOAGLIB_ADDITIONAL_DEBUG_DEFINITIONS "-DDEBUG_CANSERVER=1")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${ISOAGLIB_ADDITIONAL_DEBUG_DEFINITIONS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${ISOAGLIB_ADDITIONAL_RELEASE_DEFINITIONS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${ISOAGLIB_ADDITIONAL_RELEASE_DEFINITIONS}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${ISOAGLIB_ADDITIONAL_RELEASE_DEFINITIONS}")

include_directories(
  ../../src
  ../../../../library/xgpl_src/IsoAgLib/hal/pc/can
  ../../../../library/xgpl_src)

if(WIN32)
  include_directories(
    ../../../../tools/libs/pthread/include
    "C:/Programme/LAWICEL/CANUSB/include")
  find_package(Threads REQUIRED)
  link_directories(
    ../../../../tools/libs/pthread/lib
    "C:/Programme/LAWICEL/CANUSB/libs/Microsoft")
  set(ISOAGLIB_ADDITIONAL_LIBRARIES odbc32 odbccp32 winmm pthreadVC2 ws2_32 canusbdrv)
else()
  message( FATAL_ERROR "This can server variant can't be build on non Windows systems.")
endif(WIN32)

add_executable(
  can_server_lawicel
  ../../src/can_server_interface_server.cpp
  ../../src/can_server_common.cpp
  ../../src/can_server_sock.cpp
  ../../src/can_filtering.cpp
  ../../src/devices/can_device_lawicel.cpp)

target_link_libraries(can_server_lawicel ${ISOAGLIB_ADDITIONAL_LIBRARIES})
