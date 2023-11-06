cmake_minimum_required(VERSION 3.5)

project(loconvertor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LOROOT /opt/libreoffice7.6)

add_executable(loconvertor main.cpp)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/com/sun/star
    ${LOROOT}/sdk/include
    )
target_link_directories(loconvertor PRIVATE
    ${LOROOT}/program
    ${LOROOT}/sdk/lib
    )
target_link_libraries(loconvertor
    uno_sal
    uno_cppu
    uno_cppuhelpergcc3
    uno_salhelpergcc3
    unoidllo
    xmlreaderlo
    reglo
    mergedlo
    )

add_definitions(-DLINUX)
execute_process(COMMAND ${LOROOT}/sdk/bin/cppumaker -Gc -O. ${LOROOT}/program/types.rdb ${LOROOT}/program/types/offapi.rdb)

