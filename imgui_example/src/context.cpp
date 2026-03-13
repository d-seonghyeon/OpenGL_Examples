#include "context.h"
#include "image.h"
#include <imgui.h>

ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if(!context->Init())
        return nullptr;

    return std::move(context);

}

void Context::ProcessInput(GLFWwindow *window){

    const float cameraSpeed = 0.05f;
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * m_cameraFront;
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * m_cameraFront;
    
    auto cameraRight = glm::normalize(glm::cross(m_cameraUp,-m_cameraFront));
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraRight;
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraRight;

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraUp;
    if(glfwGetKey(window,GLFW_KEY_Q)==GLFW_PRESS)
        m_cameraPos -= cameraSpeed *cameraUp;

    if(!m_cameraControl)
        return;
}

void Context::Reshape(int width, int height){
    m_width = width;
    m_height = height;
    glViewport(0,0,m_width,m_height);

}

void Context::MouseMove(double x, double y){

    if(!m_cameraControl)
        return;
    auto pos = glm::vec2((float)x,(float)y);
    auto deltaPos = pos - m_prevMousePos;

    const float cameraRotSpeed = 0.8f;
    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -=deltaPos.y * cameraRotSpeed;

    if(m_cameraYaw <0.0f) m_cameraYaw+=360.0f;
    if(m_cameraYaw>360.0f) m_cameraYaw-=360.0f;

    if(m_cameraPitch >89.0f) m_cameraPitch = 89.0f;
    if(m_cameraPitch <-89.0f) m_cameraPitch = -89.0f;

    m_prevMousePos = pos;

}

void Context::MouseButton(int button, int action, double x, double y){
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
        if(action == GLFW_PRESS){
            m_prevMousePos =glm::vec2((float)x,(float)y);
            m_cameraControl = true;
        }
        else if (action==GLFW_RELEASE){
            m_cameraControl = false;
        }
    }
}



bool Context::Init(){

    float vertices[]={

        //A
        -0.5f,-0.5f,-0.5f,0.0f,0.0f,
         0.5f,-0.5f,-0.5f,1.0f,0.0f,
         0.5f,0.5f,-0.5f,1.0f,1.0f,
        -0.5f,0.5f,-0.5f,0.0f,1.0f,


        //B
        0.5f, -0.5f, 0.5f, 0.0f,0.0f,
        0.5f, -0.5f,-0.5f, 1.0f,0.0f,
        0.5f,0.5f,-0.5f, 1.0f,1.0f,
        0.5f,0.5f, 0.5f, 0.0f,1.0f,

        //C
        -0.5f,-0.5f, 0.5f,0.0f,0.0f,
         0.5f,-0.5f, 0.5f,1.0f,0.0f,
         0.5f,-0.5f,-0.5f,1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,0.0f,1.0f,

        //D
        0.5f,0.5f,0.5f,0.0f,0.0f,
         0.5f,0.5f,-0.5f,1.0f,0.0f,
        -0.5f,0.5f,-0.5f,1.0f,1.0f,
        -0.5f,0.5f,0.5f,0.0f,1.0f,

        //E
        -0.5f,0.5f,0.5f,0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,0.0f,1.0f,

        //F
        -0.5f, -0.5f, 0.5f, 0.0f,0.0f,
         0.5f, -0.5f, 0.5f, 1.0f,0.0f,
         0.5f, 0.5f, 0.5f, 1.0f,1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,1.0f,
	
	};

    uint32_t indices[]={
        
        0,1,2,2,3,0,
        4,5,6,6,7,4,
        8,9,10,10,11,8,
        12,13,14,14,15,12,
        16,17,18,18,19,16,
        20,21,22,22,23,20,
    };

    //VAO 생성

    m_vertexLayout= VertexLayout:: Create();

    /*
    glGenVertexArrays(1,&m_vertexArrayObject); //VAO 생성
    glBindVertexArray(m_vertexArrayObject); //지금부터 사용할 VAO 설정
    */

    //context::Init()에서 VBO 생성 및 정점 데이터를 GPU로 복사
    

    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,GL_STATIC_DRAW,vertices,sizeof(float)*120);
   
    m_vertexLayout -> SetAttrib(0,3,GL_FLOAT,GL_FALSE,sizeof(float) *5,0); // 0번 attribute/ 값 개수/type/normalize 여부/ stride/ offset
    m_vertexLayout -> SetAttrib(2,2,GL_FLOAT,GL_FALSE,sizeof(float) *5, sizeof(float)*3); 
 

    m_indexBuffer = Buffer:: CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,indices,sizeof(uint32_t)*36);







    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs",GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs",GL_FRAGMENT_SHADER);
    
    if(!vertShader || !fragShader)
        return false;
    
    SPDLOG_INFO("vertex shader id: {}", vertShader ->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader ->Get());

    
    m_program = Program::Create({fragShader, vertShader});
    if(!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program ->Get());

    //Uniform
    /*
    auto loc = glGetUniformLocation(m_program -> Get(), "color");
    m_program->Use();
    glUniform4f(loc, 1.0f,1.0f,0.0f,1.0f);//glUniform4f <- floating 값이 4개
    */
    glClearColor(0.0f,0.1f,0.2f,0.0f);

    /* VAO
    uint32_t vao=0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    */


    auto image =Image::Load("./image/container.jpg");
    if(!image)
    {
        return false;
    }

    SPDLOG_INFO("image: {}x{}, {} channels",image->GetWidth(),image->GetHeight(),image->GetChannelCount());

    m_texture = Texture::CreateFromImage(image.get());

    m_texture = Texture::CreateFromImage(image.get());

    auto image2= Image::Load("./image/awesomeface.png");
    m_texture2 =Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);//0번 슬롯 활성화
    glBindTexture(GL_TEXTURE_2D,m_texture->Get()); //0번슬롯에 텍스처 바인딩, 호출한 순서대로 바인딩
    glActiveTexture(GL_TEXTURE1);//1번 슬롯 활성화
    glBindTexture(GL_TEXTURE_2D,m_texture2->Get()); //1번 슬롯에 텍스처 바인딩, 호출한 순서대로 바인딩


    m_program->Use();
    m_program ->SetUniform("tex",0);
    m_program ->SetUniform("tex2",1);
    glUniform1i(glGetUniformLocation(m_program->Get(),"tex"),0);
    glUniform1i(glGetUniformLocation(m_program->Get(),"tex2"),1);


    return true;

}

void Context::Render(){

    if(ImGui::Begin("ui window")){
        
        if(ImGui::ColorEdit4("clear color",glm::value_ptr(m_clearColor))){
            
            glClearColor(m_clearColor.x,m_clearColor.y,m_clearColor.z,m_clearColor.w);
        }
        ImGui::Separator();//분리선
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos),0.01f);
        ImGui::DragFloat("camera yaw", &m_cameraYaw,0.5f);
        ImGui::DragFloat("camera pitch",&m_cameraPitch,0.5f,-89.0f,89.0f);
        ImGui::Separator();
        if(ImGui::Button("reset camera")){
            m_cameraYaw =0.0f;
            m_cameraPitch=0.0f;
            m_cameraPos = glm::vec3(0.0f,0.0f,3.0f);
        }
    }
    ImGui::End();//begin, end는 쌍으로 꼭존재


    std::vector<glm::vec3> cubePositions = {

        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(2.0f,5.0f,-15.0f),
        glm::vec3(-1.5f,-2.2f,-2.5f),
        glm::vec3(-3.8f,-2.0f,-12.3f),
        glm::vec3(2.4f,-0.4f,-3.5f),
        glm::vec3(-1.7f,3.0f,-7.5f),
        glm::vec3(1.3f,-2.0f,-2.5f),
        glm::vec3(1.5f,2.0f,-2.5f),
        glm::vec3(1.5f,0.2f,-1.5f),
        glm::vec3(-1.3f,1.0f,-1.5f),

    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//glClearColor의 색상으로 버퍼 초기화

    glEnable(GL_DEPTH_TEST);


    m_cameraFront= 
    glm::rotate(glm::mat4(1.0f),glm::radians(m_cameraYaw),glm::vec3(0.0f,1.0f,0.0f)) * 
    glm::rotate(glm::mat4(1.0f),glm::radians(m_cameraPitch),glm::vec3(1.0f,0.0f,0.0f)) *
    glm::vec4(0.0f,0.0f,-1.0f,0.0f); //동차좌표계, 벡터가 아닌 점, 평행이동 x


	auto projection = glm::perspective(glm::radians(45.0f),(float)m_width/(float)m_height,0.01f,20.0f);//마지막 parameter 크게 -> 회전할때 사라지는 큐브 문제 해결 가능
/*
    //x축으로 진동하는 카메라
    float x = sinf((float)glfwGetTime()*glm::pi<float>()*2.0f) * 3.0f;
    auto cameraPos = glm::vec3(x, 0, 3.0f);
    auto cameraTarget = glm::vec3(x ,0.0f,0.0f);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
*/
/*
    //y축을 중심으로 빙글빙글 도는 카메라
    float angle = glfwGetTime() * glm::pi<float>() *0.5; //4초마다 1바퀴
    auto x = sinf(angle)* 10.0f; //반지름 = 10
    auto z = cosf(angle) * 10.0f; //반지름 = 10
    auto cameraPos = glm::vec3(x,0.0f,z);
    auto cameraTarget = glm::vec3(0.0f,0.0f,0.0f);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
*/
    /* 카메라 3축 결정 과정
    auto cameraZ = glm::normalize(cameraPos - cameraTarget);
    auto cameraX = glm::normalize(glm::cross(cameraUp, cameraZ));
    auto cameraY = glm::cross(cameraZ, cameraX);

    auto cameraMat = glm::mat4(
        glm::vec4(cameraX,0.0f),
        glm::vec4(cameraY, 0.0f),
        glm::vec4(cameraZ, 0.0f),
        glm::vec4(cameraPos, 1.0f)
    );

    auto view= glm::inverse(cameraMat);
    */

    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);


    for(size_t i=0; i < cubePositions.size();i++){

        auto &pos =cubePositions[i];

        auto model = glm:: translate(glm::mat4(1.0f),pos);

        model= glm::rotate(model, glm::radians((float)glfwGetTime()*240.0f + 20.0f*(float)i),glm::vec3(1.0f,0.5f,0.0f));

        auto transform = projection * view * model;

        m_program ->SetUniform("transform",transform);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);


    }

}