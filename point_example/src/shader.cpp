#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType){

    auto shader = ShaderUPtr(new Shader()); //createfromfile은 내부함수여서 Shader 직접 부를 수 있다.
    if(!shader->LoadFile(filename,shaderType))
        return nullptr; //loadfile이 실패하면 false return하고 nullptr 반환, shader는 unique pointer이기 때문에 
    
    
    return std::move(shader); //c++17 이상에서는 그냥 return shader로 써도됨


}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType){

    auto result = LoadTextFile(filename);
    if (!result.has_value())// has_value: 값이 들어있는지 없는지 확인하는 함수
        return false;

    auto& code = result.value(); 
    //std::string code = result.value();(not good) -> result 라는 값이 함수가 끝날때 까지 살아있는데 굳이 메모리 복사를 할 필요 없음
    //std:: string &code =result.value() -> 레퍼런스 타입으로 받는게 좋음, code가 result.value를 가르킴, 복사 일어나지 않아서 성능 손해x, code 고치면 value도 고쳐짐
    //== auto &code= result.value()

    const char* codePtr =code.c_str();
    int32_t codeLength = (int32_t)code.length();

    /* 앞에 자료형으로 쓰는게 프로그래밍 하는데 더도움이됨(비트수 나옴)
    
    uint8_t = unsigned char //0~255
    int8_t = char   //-128~127
    uint16_t = unsigned short   //0~65535
    int16_t = short //-32768 ~32767
    uint32_t = unsigned int //  0 ~ 4294967295
    int32_t = int //    -2147483648 ~2147483647
    uint64_t = unsigned long long   
    int64_t = long long

    float = 4바이트 //소수점 6~7자리 까지 정확
    double = 8바이트 //소수점 15자리 까지 정확
    long double = 8~16 바이트 //소수점 18~19자리이상, 리눅스는 16바이트, 윈도우에서는 double과 동일할 수 있음
    */


    m_shader=glCreateShader(shaderType);
    glShaderSource(m_shader,1,(const GLchar * const*)&codePtr, &codeLength);
    glCompileShader(m_shader);

    int success =0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);

    if(!success){

        char infoLog[1024];
        glGetShaderInfoLog(m_shader,1024,nullptr,infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"",filename);
        SPDLOG_ERROR("reason: {}", infoLog);

        return false;

    }

    return true;

}