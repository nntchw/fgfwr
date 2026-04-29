cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

project(
  fgfwr
  VERSION 0.1
  LANGUAGES CXX
)

file(GLOB_RECURSE SRC_FILES src/*)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cc
        ${CMAKE_SOURCE_DIR}/include/*.hh
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formatting all source files with clang-format."
    )
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-fconstexpr-steps=100000000)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fconstexpr-ops-limit=100000000)
endif()

add_compile_options(
  -O3
  -flto -funroll-loops -fno-exceptions
  -march=native
  # -fsanitize=undefined -fsanitize=address
)

add_link_options(
  -flto
  # -fsanitize=undefined -fsanitize=address
)

add_executable(fgfwr ${SRC_FILES})

target_link_libraries(fgfwr)
