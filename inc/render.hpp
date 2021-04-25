#ifndef RENDER_HPP_
#define RENDER_HPP_

#include "main.hpp"
#include "structs.hpp"
#include "object.hpp"
#include "object2D.hpp"
#include "camera.hpp"
#include "camera2D.hpp"

namespace vector2 {
	extern Camera g_Camera;

	glm::quat rQuat();

	std::vector<glm::quat> splash(int instances);
	std::vector<glm::quat> drawSplash(int instances, float scale, Object o, int id, shader &s, Camera &c);
	void drawSplash(std::vector<glm::quat>, int instances, float scale, float movement, Object o, int id, shader &s, Camera &c);

	void drawModel(Object &o, int id, shader &s, Camera &c);
	void drawModel(TileObject &o, int id, shader &s, Camera &c);
	void drawModelOver(Object &o, int id, shader &s, Camera &c);
	void drawModelOver(TileObject &o, int id, shader &s, Camera &c);

	void drawModel(Object2D &o, int id, shader &s, Camera2D &c);
	void drawModel(TileObject2D &o, int id, shader &s, Camera2D &c);
	void drawModelOver(Object2D &o, int id, shader &s, Camera2D &c);
	void drawModelOver(TileObject2D &o, int id, shader &s, Camera2D &c);
}


#endif /* RENDER_HPP_ */
