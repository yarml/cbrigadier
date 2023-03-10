cmake_minimum_required(VERSION 3.12)

project(CBrigadier)

file(GLOB_RECURSE SRC_FILES "src/*.c")
file(GLOB_RECURSE INC_FILES "inc/*.h" "src/*.h")

set(BRIGADIER "brigadier")

add_library(
  ${BRIGADIER}
  STATIC
  ${SRC_FILES}
  ${INC_FILES}
)

target_include_directories(
  ${BRIGADIER}
  PUBLIC "inc/"
)
