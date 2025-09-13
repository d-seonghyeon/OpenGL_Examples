#include "context.h"
#include "image.h"

ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if(!context->Init())
        return nullptr;

    return std::move(context);

}

bool Context::Init(){

    float vertices[]={

	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //top right, red
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,//bottom right, green
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //bottom left, blue
	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //top left, yellow


	};

    uint32_t indices[]={
        0,1,3,//first triangle
        1,2,3//second triangle
    };

    //VAO 생성

    m_vertexLayout= VertexLayout:: Create();

    /*
    glGenVertexArrays(1,&m_vertexArrayObject); //VAO 생성
    glBindVertexArray(m_vertexArrayObject); //지금부터 사용할 VAO 설정
    */

    //context::Init()에서 VBO 생성 및 정점 데이터를 GPU로 복사
    

    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,GL_STATIC_DRAW,vertices,sizeof(float)*32);
   
    m_vertexLayout -> SetAttrib(0,3,GL_FLOAT,GL_FALSE,sizeof(float) *8,0); // 0번 attribute/ 값 개수/type/normalize 여부/ stride/ offset
	m_vertexLayout -> SetAttrib(1,3,GL_FLOAT,GL_FALSE,sizeof(float) *8, sizeof(float)*3); 
    m_vertexLayout -> SetAttrib(2,2,GL_FLOAT,GL_FALSE,sizeof(float) *8, sizeof(float)*6); 
 

    m_indexBuffer = Buffer:: CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,indices,sizeof(uint32_t)*6);







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
    glUniform1i(glGetUniformLocation(m_program->Get(),"tex"),0);
    glUniform1i(glGetUniformLocation(m_program->Get(),"tex2"),1);

    return true;

}

void Context::Render(){
    glClear(GL_COLOR_BUFFER_BIT);//glClearColor의 색상으로 버퍼 초기화

	m_program ->Use();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

    //m_program ->Use();
    //glDrawArrays(GL_POINTS,0,1);

    //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

}