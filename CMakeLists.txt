cmake_minimum_required (VERSION 2.6)
project (xennigan-shell)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
find_package(Boost REQUIRED COMPONENTS regex filesystem system program_options)
find_library(READLINE_LIBRARY NAMES readline)
include_directories( ${Boost_INCLUDE_DIR} )
add_executable(xennigan-shell xennigan-shell.cxx)
target_link_libraries(xennigan-shell
                        ${Boost_LIBRARIES}
                        ${READLINE_LIBRARY})
# TODO set group?
install(TARGETS xennigan-shell
        RUNTIME DESTINATION bin
        PERMISSIONS SETUID OWNER_EXECUTE GROUP_EXECUTE)
install(FILES update-xennigan
        DESTINATION sbin
        PERMISSIONS OWNER_EXECUTE)
#target_compile_features(xennigan-shell PRIVATE cxx_uniform_initialization)
