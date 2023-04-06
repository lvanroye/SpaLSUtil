cmake_minimum_required(VERSION 3.0.0)
project(symspals VERSION 0.1.0 LANGUAGES CXX)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_library(symspals SHARED SymSpaLS.cpp node.hpp sparse_matrix.hpp)
add_executable(util SpaLSUtil.cpp)

include(CPack)

# make the symspals library installable as a cmake package
install(TARGETS symspals
    EXPORT symspalsConfig 
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
# include the configuration file to the installation
INSTALL(EXPORT symspalsConfig DESTINATION cmake)
