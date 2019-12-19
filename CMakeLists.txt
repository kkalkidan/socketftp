project(socketftp)
cmake_minimum_required(VERSION 3.5)

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc"
    @ONLY
)

file( GLOB SOURCES src/*.c)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

include(GNUInstallDirs)
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install (
    FILES ${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
