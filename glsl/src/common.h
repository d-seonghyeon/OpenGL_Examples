#ifndef __COMMON_H__
#define __COMMON_H__

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

//매크로
#define CLASS_PTR(klassName) \
class klassName; \
using klassName ## UPtr = std::unique_ptr<klassName>; \
using klassName ## Ptr = std::shared_ptr<klassName>; \
using klassName ## WPtr = std::weak_ptr<klassName>;


std::optional<std::string> LoadTextFile(const std::string& filename);
//값이 있으면 꺼내고 없으면 꺼내지 않음, 빈 파일 을 동적할당하고 free하지 않으면 메모리 누수가 생기기 때문에
//이를 방지하기위해서 optional 사용



#endif

//ifndef,define,endif 세트, main.cpp에서 #include "common.h" 3번 포함해도
//사이에 있는 코드가 한번만 붙게 해준다.
//pragma once 사용하기도 한다.