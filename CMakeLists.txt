cmake_minimum_required(VERSION 3.5)
PROJECT(mdio-tool)

include(GNUInstallDirs)

set(SOURCES mdio-tool.c mii.h)
add_executable(mdio-tool ${SOURCES})

install(TARGETS mdio-tool RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
