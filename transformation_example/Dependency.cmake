# ExternalProject 모듈 포함
include(ExternalProject)

# 의존성 설치 경로
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# ---------- spdlog ----------
ExternalProject_Add(
    dep_spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.x"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
)

set(DEP_LIST ${DEP_LIST} dep_spdlog)
set(DEP_LIBS ${DEP_LIBS} spdlog)

# ---------- glfw ----------
ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.9"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
)

set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)

# ---------- glad ----------
ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad.git"
    GIT_TAG "v2.0.8"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""   # CMake configure 안 함
    BUILD_COMMAND ""       # 빌드 없음
    INSTALL_COMMAND ""     # 설치 없음
)

# glad 소스와 include 경로 수동 추가
set(GLAD_DIR ${PROJECT_BINARY_DIR}/dep_glad-prefix/src/dep_glad)

set(GLAD_INCLUDE_DIR ${GLAD_DIR}/include)
set(GLAD_SRC ${GLAD_DIR}/src/glad.c)

# include 디렉토리에 추가
set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

# 실행 파일에 glad.c 포함 (라이브러리처럼 링크할 필요 없음)
set(DEP_SRCS ${DEP_SRCS} ${GLAD_SRC})

set(DEP_LIST ${DEP_LIST} dep_glad)


# ---------stb------------

ExternalProject_Add(

    dep_stb
    GIT_REPOSITORY "https://github.com/nothings/stb"
    GIT_TAG "master"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image.h
        ${DEP_INSTALL_DIR}/include/stb/stb_image.h
)

set(DEP_LIST ${DEP_LIST} dep_stb)


# ---------GLM------------

ExternalProject_Add(

    dep_glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "1.0.1"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm
        ${DEP_INSTALL_DIR}/include/glm

)

set(DEP_LIST ${DEP_LIST} dep_glm)


# ---------- macOS 프레임워크 추가 ----------
if(APPLE)
    set(DEP_LIBS ${DEP_LIBS}
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
    )
endif()
