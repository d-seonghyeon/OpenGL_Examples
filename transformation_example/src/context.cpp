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

    //0.5배 축소후 z축으로 90도 회전하는 행렬
    /*
    auto transform = glm::rotate(
        glm::scale(glm::mat4(1.0f),glm::vec3(2.0f)),
        glm::radians(60.0f),
        glm::vec3(0.0f,0.0f,1.0f)
        );
    */

    //x축으로 -55도 회전
    auto model = glm::rotate(glm::mat4(1.0f),
    glm::radians(-55.0f),
    glm::vec3(1.0f,0.0f,0.0f));
    //카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    auto view =glm::translate(glm::mat4(1.0f),
    glm::vec3(0.0f,0.0f,-3.0f));
    //종횡비 4:3, 세로화각 45도의 원근 투영
    auto projection = glm::perspective(glm::radians(45.0f),
    (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.01f,10.0f);
    auto transform = projection *view * model;

    m_program ->SetUniform("transform",transform);
  
    //SPDLOG_INFO("transformed vec: [{}, {}, {}]",vec.x,vec.y,vec.z);

    return true;

}

void Context::Render(){



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


	auto projection = glm::perspective(glm::radians(45.0f),(float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.01f,20.0f);

    auto view = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-3.0f));

    for(size_t i=0; i < cubePositions.size();i++){

        auto &pos =cubePositions[i];

        auto model = glm:: translate(glm::mat4(1.0f),pos);

        model= glm::rotate(model, glm::radians((float)glfwGetTime()*240.0f + 20.0f*(float)i),glm::vec3(1.0f,0.5f,0.0f));

        auto transform = projection * view * model;

        m_program ->SetUniform("transform",transform);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);


    }

}