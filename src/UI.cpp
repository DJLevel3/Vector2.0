#include "UI.hpp"

using namespace vector2;

std::map<std::string, letter> vector2::loadLetters(std::string inFile, std::map<std::string, letter> outMap)
{
	std::string token;
	std::fstream in(inFile);
	std::string label;
	std::vector<vertex> v;
	vertex vt;
	std::vector<GLuint> l;
	letter let;

	in >> token;
	while (!(in.eof()) && token != "Q")
	{
		while (token != "X")
		{
			label = token;
			in >> token;
		}
		double p[3];
		double c[3];

		in >> token;
		while (token != "X")
		{
			p[0] = std::stod(token) / 3;
			in >> token;
			p[1] = std::stod(token) / 3;
			in >> token;
			p[2] = std::stod(token) / 3;
			in >> token;

			if (token == "#") {in >> token; in >> token;}

			c[0] = std::stod(token);
			in >> token;
			c[1] = std::stod(token);
			in >> token;
			c[2] = std::stod(token);
			in >> token;

			if (token == "#") {in >> token; in >> token;}

			vt = {glm::vec3(-p[0],p[1],p[2]), glm::vec3(c[0],c[1],c[2])};

			v.push_back(vt);
		}

		in >> token;
		while (token != "X")
		{
			l.push_back(std::stoul(token));
			in >> token;

			if (token == "#") {in >> token; in >> token;}
		}

		let = (letter){v, l};

		outMap[label] = let;
		v.clear();
		l.clear();
		in >> token;
	}
	return outMap;
}



