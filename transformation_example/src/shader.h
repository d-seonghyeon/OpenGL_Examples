#ifndef __SHADER_H__
#define __SHADER_H__
#include "common.h"

CLASS_PTR(Shader);

class Shader{

    public:
        static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shaderType);

        ~Shader();
        uint32_t Get() const {return m_shader; }
    
    private:
        Shader() {}
        bool LoadFile(const std::string& filename, GLenum shaderType);
        uint32_t m_shader{0};//m_shader = opengl에서 만들어진 셰이더 객체의 핸들(ID)를 저장하는 변수
};





#endif