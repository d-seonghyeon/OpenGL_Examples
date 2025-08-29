#include "context.h"

ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if(!context->Init())
        return nullptr;

    return std::move(context);

}

bool Context::Init(){

    float vertices [] ={
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f,
    };

    //VAO 생성
    glGenVertexArrays(1,&m_vertexArrayObject); //VAO 생성
    glBindVertexArray(m_vertexArrayObject); //지금부터 사용할 VAO 설정


    //context::Init()에서 VBO 생성 및 정점 데이터를 GPU로 복사
    glGenBuffers(1,&m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);//m_vertexBuffer가 어떤 데이터가 들어있는 버퍼인지 GL_ARRAY_BUFFER에 바인딩해준다.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*9,vertices,GL_STATIC_DRAW);
    //

    //

    glEnableVertexAttribArray(0); //정점 attribute 중 n번째를 사용하도록 설정
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(float)*3,0);


    //




    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs",GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs",GL_FRAGMENT_SHADER);
    
    if(!vertShader || !fragShader)
        return false;
    
    SPDLOG_INFO("vertex shader id: {}", vertShader ->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader ->Get());

    m_program = Program::Create({fragShader, vertShader});
    if(!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program ->Get());
    
    glClearColor(0.0f,0.1f,0.2f,0.0f);

    /* VAO
    uint32_t vao=0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    */
    return true;

}

void Context::Render(){
    glClear(GL_COLOR_BUFFER_BIT);//glClearColor의 색상으로 버퍼 초기화

    glUseProgram(m_program->Get());
    //glDrawArrays(GL_POINTS,0,1);
    glDrawArrays(GL_TRIANGLES,0,3);

}