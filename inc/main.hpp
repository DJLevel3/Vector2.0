#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h> // For wglSwapInterval
#endif

#include <GL/freeglut.h>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#ifdef WINDOWS
#include <direct.h>
#include <Windows.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

#endif /* MAIN_HPP_ */
