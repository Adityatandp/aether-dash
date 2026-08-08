# Aether Dash firmware (ESP-IDF)
cmake_minimum_required(VERSION 3.16)

# Board selection: idf.py -D AETHER_BOARD=dash-s35 build
set(AETHER_BOARD "dash-s35" CACHE STRING "Aether board profile id")
message(STATUS "Aether board profile: ${AETHER_BOARD}")

set(AETHER_BOARD_DIR "${CMAKE_SOURCE_DIR}/boards/${AETHER_BOARD}")
if(NOT EXISTS "${AETHER_BOARD_DIR}/board.json")
  message(FATAL_ERROR "Unknown AETHER_BOARD='${AETHER_BOARD}'. Missing ${AETHER_BOARD_DIR}/board.json")
endif()

# Make board pins/config visible to all components.
include_directories("${AETHER_BOARD_DIR}")
add_compile_definitions(AETHER_BOARD_ID="${AETHER_BOARD}")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(aether_firmware)
