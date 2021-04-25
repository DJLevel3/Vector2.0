#ifndef INC_UI_HPP_
#define INC_UI_HPP_

#include "main.hpp"
#include "structs.hpp"

namespace vector2 {
	std::map<std::string, letter> loadLetters(std::string inFile, std::map<std::string, letter> outMap);
	void render2D();
}

#endif /* INC_UI_HPP_ */
