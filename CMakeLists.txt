cmake_minimum_required(VERSION 3.23)
# 项目名称
project (CourseAux)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
# 如果平台为Windows，则将可执行文件的编码设置为GBK
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS -fexec-charset=GBK)
endif()
# set(CMAKE_CXX_FLAGS -Wall)

# 将可执行文件输出到项目的根目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
# 指定源代码的路径
file(
	GLOB SOURCES 
		"src/*"
		"src/UI/*"
)
add_executable(${PROJECT_NAME} ${SOURCES})
# 指定头文件的目录
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
)
