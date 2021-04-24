#ifndef RENDER_HPP_
#define RENDER_HPP_

#include "main.hpp"
#include "structs.hpp"
#include "object.hpp"
#include "camera.hpp"

namespace vector2 {
	extern Camera g_Camera;

	void drawModel(Object &o, shader &s, Camera &c);
}


#endif /* RENDER_HPP_ */
