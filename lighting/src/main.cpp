#include "context.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>//순서 중요, 반드시 glfw 이전에 작성
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//frambuffer 변경시 callback 함수



void OnFramebufferSizeChange(GLFWwindow *window, int width ,int height){
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0,0,width,height);
    auto context = (Context*)glfwGetWindowUserPointer(window);// Cstyle 형변환
    //reinterpret_cast<Context*>(glfwGetWindowUserPointer(window)); //cpp style 형변환
    context->Reshape(width,height);
}


//keyboard 입력시 callback 함수
void OnKeyEvent(GLFWwindow *window, int key, int scancode,int action, int mods){

    ImGui_ImplGlfw_KeyCallback(window,key,scancode,action,mods);
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

void OnCursorPos(GLFWwindow *window, double x, double y){
    auto context = (Context*)glfwGetWindowUserPointer(window);
    context->MouseMove(x,y);
}

void OnMouseButton(GLFWwindow *window, int button, int action, int modifier){

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
    auto context = (Context*)glfwGetWindowUserPointer(window);
    double x,y;
    glfwGetCursorPos(window,&x,&y);
    context->MouseButton(button,action,x,y);
}

void OnCharEvent(GLFWwindow *window, unsigned int ch){
    ImGui_ImplGlfw_CharCallback(window,ch);
}

void OnScroll(GLFWwindow *window, double xoffset, double yoffset){
    ImGui_ImplGlfw_ScrollCallback(window,xoffset,yoffset);
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

    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window,false); //callback함수 자동호출 비활성화
    ImGui_ImplOpenGL3_Init();
    //ImGui_ImplOpenGL3_CreateFontsTexture(); 최신 ImGUI 자동생성 삭제
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    //context 초기화
    auto context = Context::Create();
    if(!context){
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    //context 생성 후 userpointer 이용
    glfwSetWindowUserPointer(window,context.get()); //임의의 pointer를 만든 후 window를 가리킨다.
    auto pointer = (Context*)glfwGetWindowUserPointer(window);//user pointer를 받은후 context로 형변환

    //mac OS 프레임버퍼(창 크기) 초기값과, 윈도우 논리값과 다르기 때문에 viewport 전에 fdwidth값과 fdheight를 버퍼값으로 바꿔줘야 삼각형이 정중앙에옴
    int fbWidth; 
    int fbHeight;
    glfwGetFramebufferSize(window,&fbWidth,&fbHeight);



    //윈도우 생성 직후에는 프레임 버퍼 변경 이벤트가 발생하지 않으므로 첫 호출을 수동으로 한다.
    OnFramebufferSizeChange(window,fbWidth,fbHeight);

    //창크기 변경시 콜백 함수 호출
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);

    //키보드 입력시 콜백 함수 호출
    glfwSetKeyCallback(window,OnKeyEvent);

    //마우스 입력시 콜백 함수 호출
    glfwSetCursorPosCallback(window,OnCursorPos);

    //마우스 버튼 클릭시 콜백 함수 호출
    glfwSetMouseButtonCallback(window,OnMouseButton);

    //문자 입력시 콜백 함수 호출
    glfwSetCharCallback(window, OnCharEvent);

    //마우스 휠 사용시 콜백함수 호출
    glfwSetScrollCallback(window,OnScroll);

    //glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();//이벤트 수집

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        context->ProcessInput(window);
        context->Render();
    
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    context.reset();

    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

    glfwTerminate();

    return 0;
}
