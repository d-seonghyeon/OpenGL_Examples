# CMake 최소 버전 지정
cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.13...4.1)


# 프로젝트 이름 및 C++ 표준 설정
set(PROJECT_NAME glsl) #프로젝트 이름
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "glsl")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

project(${PROJECT_NAME})

# 메인 프로젝트 실행 파일 정의

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/common.h src/common.cpp
    src/shader.h src/shader.cpp
    src/program.h src/program.cpp
    src/context.h src/context.cpp
    src/buffer.h src/buffer.cpp
    src/vertex_layout.h src/vertex_layout.cpp
    src/glad.c
   

        ${DEP_SRCS})

# 외부 의존성 불러오기
include(Dependency.cmake)

# include / lib 경로 연결
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC external/glad/include)



#
target_compile_definitions(${PROJECT_NAME} PUBLIC

    WINDOW_NAME="glsl"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}

)


# 의존성 빌드 후 실행파일 빌드
add_dependencies(${PROJECT_NAME} ${DEP_LIST})


