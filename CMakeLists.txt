cmake_minimum_required (VERSION 3.8)

project ("rcon" VERSION 1.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(PROJECT_SOURCES "src/main.c" "src/message.c" "src/rcon.c" "src/argparse.c")

add_executable (rcon ${PROJECT_SOURCES})

target_compile_options(rcon PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

if (MSVC)
  install(TARGETS rcon DESTINATION "Rcon/bin")
else()
  install(TARGETS rcon DESTINATION "/usr/local/bin" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()