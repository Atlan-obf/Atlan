cmake_minimum_required(VERSION 3.16)
project(AIAssistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Find curl for HTTP requests
find_package(CURL REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Network
    ${CURL_LIBRARIES}
)

# Enable Qt MOC for Qt classes
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)