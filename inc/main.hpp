#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <ctime>

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h> // For wglSwapInterval
#endif

#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

#endif /* MAIN_HPP_ */
