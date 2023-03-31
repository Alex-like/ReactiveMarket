cmake_minimum_required(VERSION 3.22)
project(ReactiveMarket)

set(CMAKE_CXX_STANDARD 17)
# ---------------------- Reactive C++ --------------------- #
find_package(rxcpp REQUIRED)
# ------------------------- boost ------------------------- #
find_package( Boost 1.79 COMPONENTS program_options REQUIRED )
# ------------------------ MongoDB ------------------------ #
find_package( libmongocxx REQUIRED )
find_package( libbsoncxx REQUIRED )
include_directories( ${LIBMONGOCXX_INCLUDE_DIR} )
include_directories( ${LIBBSONCXX_INCLUDE_DIR} )
include_directories( "/usr/local/include/mongocxx/v_noabi" )
include_directories( "/usr/local/include/bsoncxx/v_noabi" )
include_directories( "/usr/local/include/libmongoc-1.0" )
include_directories( "/usr/local/include/libbson-1.0" )
include_directories( "/usr/local/lib" )
# ------------------------- Main -------------------------- #
file( GLOB MainSource ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp )
add_executable( ${PROJECT_NAME}_main
        ${MainSource} )
target_link_libraries( ${PROJECT_NAME}_main PRIVATE
        ${LIBMONGOCXX_LIBRARIES}
        ${LIBBSONCXX_LIBRARIES}
        Boost::program_options
        rxcpp )
# ------------------------ Tests -------------------------- #
include( FetchContent )
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/cead3d57c93ff8c4e5c1bbae57a5c0b0b0f6e168.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set( gtest_force_shared_crt ON CACHE BOOL "" FORCE )
FetchContent_MakeAvailable( googletest )

file( GLOB TestSources ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
add_executable( ${PROJECT_NAME}_test
        ${TestSources} )
target_link_libraries( ${PROJECT_NAME}_test PRIVATE
        ${LIBMONGOCXX_LIBRARIES}
        ${LIBBSONCXX_LIBRARIES}
        Boost::program_options
        gtest )