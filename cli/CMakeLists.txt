cmake_minimum_required(VERSION 3.16)

project(rcon-cli LANGUAGES C VERSION 2.0.0)

add_executable(${PROJECT_NAME} main.c argparse.c cio.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cli)
target_link_libraries(${PROJECT_NAME} PRIVATE rcon)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
