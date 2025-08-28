#include "common.h"
#include "shader.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>//순서 중요, 반드시 glfw 이전에 작성
#include <GLFW/glfw3.h>


//frambuffer 변경시 callback 함수
void OnFramebufferSizeChange(GLFWwindow *window, int width ,int height){
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0,0,width,height);
}

//keyboard 입력시 callback 함수
void OnKeyEvent(GLFWwindow *window, int key, int scancode,int action, int mods){

    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
    key, scancode,
    action ==GLFW_PRESS ? "Pressed" :
    action ==GLFW_REPEAT ? "Repeat" : "Unknown",
    mods & GLFW_MOD_CONTROL ? "C" : "-",
    mods & GLFW_MOD_SHIFT ? "S" : "-",
    mods & GLFW_MOD_ALT ? "A" : "-");

    if(key ==GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


int main() {
    
    //glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Start program");
    if(!glfwInit()){
        const char* description =nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}",description);
        return -1;
    }



    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

 

    //glfw 윈도우 생성, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,nullptr,nullptr);
    
    if(!window){
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //glad를 활용한 OpenGL 함수 로딩

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    auto glVersion =glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}",reinterpret_cast < const char*>(glVersion));

    auto vertexShader = Shader::CreateFromFile("./shader/simple.vs",GL_VERTEX_SHADER);
    auto fragmentShader = Shader::CreateFromFile("./shader/simple.fs",GL_FRAGMENT_SHADER);
    SPDLOG_INFO("vertex shader id: {}", vertexShader ->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader ->Get());


    //윈도우 생성 직후에는 프레임 버퍼 변경 이벤트가 발생하지 않으므로 첫 호출을 수동으로 한다.
    OnFramebufferSizeChange(window,WINDOW_WIDTH,WINDOW_HEIGHT);

    //창크기 변경시 콜백 함수 호출
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);

    //키보드 입력시 콜백 함수 호출
    glfwSetKeyCallback(window,OnKeyEvent);

    //glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();//이벤트 수집
        glClearColor(0.0f,0.1f,0.2f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
