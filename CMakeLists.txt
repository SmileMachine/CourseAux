cmake_minimum_required(VERSION 3.23)

# Set the project name
project (CourseAux)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

# Create a sources variable with a link to all cpp files to compile
# set(SOURCES
# 	/src*.cpp
# )
file(GLOB SOURCES "src/*")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)

# Add an executable with the above sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
)
