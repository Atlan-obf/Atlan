cmake_minimum_required(VERSION 3.16)
project(AIAssistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required GTK4 and gtkmm4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GTKMM4 REQUIRED gtkmm-4.0)

# Find curl for HTTP requests
find_package(CURL REQUIRED)

# Find threads
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${GTK4_INCLUDE_DIRS})
include_directories(${GTKMM4_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/AIEngine.cpp
    src/NetworkManager.cpp
    src/CodeGenerator.cpp
    src/LearningModule.cpp
)

# Header files
set(HEADERS
    include/MainWindow.h
    include/AIEngine.h
    include/NetworkManager.h
    include/CodeGenerator.h
    include/LearningModule.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Compiler flags for GTK4
target_compile_options(${PROJECT_NAME} PRIVATE ${GTK4_CFLAGS_OTHER})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTKMM4_CFLAGS_OTHER})

# Link directories
target_link_directories(${PROJECT_NAME} PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM4_LIBRARY_DIRS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${GTK4_LIBRARIES}
    ${GTKMM4_LIBRARIES}
    ${CURL_LIBRARIES}
    Threads::Threads
    jsoncpp
)