#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"

CLASS_PTR(Context)
class Context{
    public:
        static ContextUPtr Create();
        void Render();
    private:
        Context(){}
        bool Init();
        ProgramUPtr m_program;

        uint32_t m_vertexArrayObject;
        uint32_t m_vertexBuffer;//VBO를 담아둘 uint32_t형 멤버 변수를 context 클래스에 선언
};



#endif