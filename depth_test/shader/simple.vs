//vertex shader code
#version 330 core
layout (location =0) in vec3 aPos; //location=0 이 곧 VAO attribute 0을 의미

uniform mat4 transform;

void main(){
    gl_Position = transform * vec4(aPos,1.0);
}
