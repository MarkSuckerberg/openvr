set(TARGET_NAME grift)

# This is the name of the driver according to SteamVR
set(DRIVER_NAME "driver_${TARGET_NAME}")

project(${TARGET_NAME})

add_library(${DRIVER_NAME} SHARED
  src/dataHandler.cpp
  src/dataHandler_IMU.cpp
  src/driver_main.cpp
  src/filters/MadgwickOrientation.cpp
  src/filters/SimpleKalman.cpp
  src/filters/V3Kalman.cpp
  src/HandTracking.cpp
  src/HIDTransport.cpp
)

# This is so we can build directly to "<binary_dir>/<target_name>/<platform>/<arch>/<driver_name>.<dll/so>"
set_target_properties(${DRIVER_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}/bin/${ARCH_TARGET}>)

target_link_libraries(${DRIVER_NAME} PRIVATE ${OPENVR_LIBRARIES} util_driverlog util_vrmath)
target_include_directories(${DRIVER_NAME} PRIVATE ${OPENVR_INCLUDE_DIR})

# Copy driver assets to output folder
add_custom_command(
  TARGET ${DRIVER_NAME}
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}
)
