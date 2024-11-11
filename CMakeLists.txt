cmake_minimum_required(VERSION 3.16)

# Get the workspace name from the directory
get_filename_component(WORKSPACE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE "-" "_" PROJECT_NAME ${WORKSPACE_NAME})

project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add position independent code flag
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system locale)
find_package(fmt REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)

# Collect source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Create main library
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
    ${Boost_LIBRARIES}
    fmt::fmt-header-only
    Boost::system
    Boost::locale
    ICU::uc 
    ICU::i18n
    -pthread
)
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${ICU_INCLUDE_DIRS}
)

# Create executable
add_executable(${PROJECT_NAME}_exe src/main.cpp)
target_link_libraries(${PROJECT_NAME}_exe 
    PRIVATE 
    ${PROJECT_NAME}
)
set_target_properties(${PROJECT_NAME}_exe PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")

# Add compile definitions
target_compile_definitions(${PROJECT_NAME} 
    PUBLIC 
    BOOST_ALL_NO_LIB
    FMT_HEADER_ONLY
)