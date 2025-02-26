cmake_minimum_required(VERSION 3.10)
project(GameC)

set(CMAKE_CXX_STANDARD 17)

# Set the SFML_DIR to the directory containing SFMLConfig.cmake
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

# Find the SFML package (SFML 2.5.1)
find_package(SFML 2.5 REQUIRED graphics audio network)

# Add your executable
add_executable(GameC main.cpp
        Roulette.cpp)

# Link the SFML libraries to your project
target_link_libraries(GameC PRIVATE sfml-graphics sfml-audio sfml-network)