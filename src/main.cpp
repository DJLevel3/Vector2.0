#include "main.hpp"
#include "PerlinNoise.h"
#include "structs.hpp"
#include "camera.hpp"
#include "camera2D.hpp"
#include "render.hpp"
#include "audio.hpp"
#include "UI.hpp"

using namespace vector2;

sounds soundObject;

int g_iWindowWidth = 1024;
int g_iWindowHeight = 576;
int g_iWindowHandle = 0;

int g_W, g_A, g_S, g_D, g_Q, g_E, g_left, g_right, g_up, g_down;
bool g_bShift = false;

glm::ivec2 g_MousePos;

glm::quat g_Rotation = glm::quat(1,0,0,0);

std::clock_t g_PreviousTicks;
std::clock_t g_CurrentTicks;

namespace vector2 {
	Camera g_Camera;
	Camera2D UI;
}

glm::vec3 g_InitialCameraPosition = glm::vec3( 0, 5, -10 );
glm::quat g_InitialCameraRotation = glm::quat(glm::cos( 0.61 / 2.0 ), glm::sin( 0.61 / 2.0 ), 0, 0) * glm::quat(0,0,1,0);

glm::vec3 camDeviation = glm::vec3(0,0,0);
double shakeAngle;
double progTime = 0;
PerlinNoise p;

shader vecShader;
shader vecShaderTile;
shader coverShader;
shader coverShaderTile;

float fMScale = 2;
int   fMSizeX = 6;
int   fMSizeZ = 10;

int   wMSizeZ = 5;

int   fWidth = 6;
int   fLength = 200;

double playfield = (fWidth/2) * fMSizeX;

/* Axes
vertex vertices[4] = {
		{glm::vec3(0,0,0), glm::vec3(1,1,1)},
		{glm::vec3(1,0,0), glm::vec3(1,0,0)},
		{glm::vec3(0,1,0), glm::vec3(0,1,0)},
		{glm::vec3(0,0,1), glm::vec3(0,0,1)},
};
int size = 3 * 3;
GLuint indices[3 * 3] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 3
};
*/

Empty scene = Empty({0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1});

std::vector<vertex> defaultVertices = {
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
		{glm::vec3( 0.00,  1.00,  0.00), glm::vec3(1, 1, 1)},
};

std::vector<GLuint> defaultIndices = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
};

letter nul = {defaultVertices, defaultIndices};

// Ship 1
std::vector<vertex> shipVertices = {
		{glm::vec3( 0.00,  1.70, -3.00), glm::vec3(0,1,0.2)},  // 0
		{glm::vec3( 0.00,  0.00,  4.00), glm::vec3(0,1,0.2)},  // 1
		{glm::vec3( 3.00,  0.00, -5.00), glm::vec3(0,1,0.2)},  // 2
		{glm::vec3( 0.00, -1.50, -2.00), glm::vec3(0,1,0.2)},  // 3
		{glm::vec3(-3.00,  0.00, -5.00), glm::vec3(0,1,0.2)},  // 4
		{glm::vec3( 0.65, -1.50, -5.00), glm::vec3(0,1,0.2)},  // 5
		{glm::vec3(-0.65, -1.50, -5.00), glm::vec3(0,1,0.2)},  // 6
		{glm::vec3( 0.65,  0.00, -5.00), glm::vec3(0,1,0.2)},  // 7
		{glm::vec3(-0.65,  0.00, -5.00), glm::vec3(0,1,0.2)}   // 8
};
std::vector<GLuint> shipIndices = {
		0, 0,    1,    2,  // 0
		0, 0,    1,    4,  // 1
		3, 3,    1,    2,  // 2
		3, 3,    1,    4,  // 3
		2, 2,    1,    4,  // 4
		3, 3,    5,    6,  // 5
		3, 3,    5,    7,  // 6
		3, 3,    6,    8   // 7
};
Object ship;
double shipCurrentZRotation = 0;
bool shipDead = 0;

double menuRotation = 0;

std::vector<vertex> particleVertices = {
		{glm::vec3(1, -1/SQRT3, -1/SQRT6),  glm::vec3(0, 1, 0.2)},
		{glm::vec3(-1, -1/SQRT3, -1/SQRT6), glm::vec3(0, 1, 0.2)},
		{glm::vec3(0, 2/SQRT3, -1/SQRT6),   glm::vec3(0, 1, 0.2)},
		{glm::vec3(0, 0, 3/SQRT6),          glm::vec3(0, 1, 0.2)}
};
std::vector<GLuint> particleIndices = {
		0, 0, 1, 2,
		1, 1, 2, 3,
		2, 2, 3, 0
};
Object explosionParticle = Object(&ship);
std::vector<glm::quat> ePSplash = {};

// Floor
std::vector<vertex> floorVertices = {
		{glm::vec3(-3.00,  0.00,  0.00), glm::vec3(1,0.3,0)},  // 0
		{glm::vec3( 3.00,  0.00,  0.00), glm::vec3(1,0.3,0)},  // 1
		{glm::vec3( 0.00,  0.00,  5.00), glm::vec3(1,0.3,0)},  // 2
		{glm::vec3( 0.00,  0.00, -5.00), glm::vec3(1,0.3,0)},  // 3
		{glm::vec3( 6.00,  0.00,  5.00), glm::vec3(1,0.3,0)},  // 4
		{glm::vec3( 6.00,  0.00, -5.00), glm::vec3(1,0.3,0)},  // 5
		{glm::vec3( 9.00,  0.00,  0.00), glm::vec3(1,0.3,0)},  // 6
};
std::vector<GLuint> floorIndices = {
		0, 0, 1, 2,  // 0
		0, 0, 1, 3,  // 1
		1, 1, 2, 4,  // 2
		1, 1, 3, 5,  // 3
		1, 1, 5, 6,  // 4
		1, 1, 4, 6,  // 4
};
std::vector<glm::vec3> floorOffsets = {};
TileObject objfloor;


std::vector<vertex> exhaustVertices = {
		{glm::vec3( 0.00,  0.00,  0.00), glm::vec3(0,1,0.2)},  // 0
		{glm::vec3( 0.50,  0.00, -1.00), glm::vec3(0,1,0.2)},  // 1
		{glm::vec3( 0.00,  0.50, -1.00), glm::vec3(0,1,0.2)},  // 2
		{glm::vec3(-0.50,  0.00, -1.00), glm::vec3(0,1,0.2)},  // 3
		{glm::vec3( 0.00, -0.50, -1.00), glm::vec3(0,1,0.2)},  // 4
		{glm::vec3( 0.00,  0.00, -4.00), glm::vec3(0,1,0.2)},  // 5
};
std::vector<GLuint> exhaustIndices = {
		0, 0, 1, 2,
		0, 0, 3, 4,
		5, 5, 2, 3,
		5, 5, 1, 4
};
Object exhaust = Object(ship);

// Wall
std::vector<vertex> wallVertices = {
		{glm::vec3( 0.00,  0.00,  0.00), glm::vec3(1,0,0)},  // 0
		{glm::vec3( 0.00,  0.00,  5.00), glm::vec3(1,0,0)},  // 1
		{glm::vec3( 0.00,  8.00,  5.00), glm::vec3(1,0,0)},  // 2
		{glm::vec3( 0.00,  8.00,  0.00), glm::vec3(1,0,0)}   // 3
};
std::vector<GLuint> wallIndices = {
		0,     1,     2,    3   // 0
};
std::vector<glm::vec3> wallOffsets = {};
TileObject objwall;

Object text;
Object2D text2D;
std::vector<std::string> let = {};

unsigned currentScene = 0;
glm::vec3 scenePosition = glm::vec3(0,0,0);
std::map<std::string, letter> alphabet;
bool letterKey = false;
bool lettersChanged = false;

/* Cube
vertex vertices[8] = {
    { glm::vec3(  1,  1,  1 ), glm::vec3( 1, 1, 1 ) }, // 0
    { glm::vec3( -1,  1,  1 ), glm::vec3( 0, 1, 1 ) }, // 1
    { glm::vec3( -1, -1,  1 ), glm::vec3( 0, 0, 1 ) }, // 2
    { glm::vec3(  1, -1,  1 ), glm::vec3( 1, 0, 1 ) }, // 3
    { glm::vec3(  1, -1, -1 ), glm::vec3( 1, 0, 0 ) }, // 4
    { glm::vec3( -1, -1, -1 ), glm::vec3( 0, 0, 0 ) }, // 5
    { glm::vec3( -1,  1, -1 ), glm::vec3( 0, 1, 0 ) }, // 6
    { glm::vec3(  1,  1, -1 ), glm::vec3( 1, 1, 0 ) }, // 7
};

GLuint indices[36] = {
    0, 1, 2, 2, 3, 0,           // Front face
    7, 4, 5, 5, 6, 7,           // Back face
    6, 5, 2, 2, 1, 6,           // Left face
    7, 0, 3, 3, 4, 7,           // Right face
    7, 6, 1, 1, 0, 7,           // Top face
    3, 2, 5, 5, 4, 3            // Bottom face
};
*/

float fDeltaTime, splashTime, explosionTime = 0;

int mainMenuCounter = 0;

glm::vec3 shakeSpeed = {0, 0, 0};

void resetGame()
{
	g_Camera.SetPosition(g_InitialCameraPosition);
	g_Camera.SetRotation(g_InitialCameraRotation);
	splashTime = explosionTime = 0;
	shipDead = false;
	currentScene = 1;
	scenePosition = glm::vec3(0,0,0);
	shipCurrentZRotation = 0;
	ship.setPosition({0,0,0});
	ship.setRotation({1,0,0,0});
	exhaust.setScale({1,1,1});
	ePSplash = {};
	shakeSpeed = {0,0,0};
}

void gameIdle(float fDeltaTime)
{
	//shakeAngle = moveAround(shakeAngle, 1, 0.1, camDeviation, (glm::vec2){0.0001,0});
	//double deltaX = glm::cos(shakeAngle) * fDeltaTime * shakeSpeed;
	//double deltaY = glm::sin(shakeAngle) * fDeltaTime * shakeSpeed;
	float t = progTime * 100;
	if (shakeSpeed.x < 0.8) {
		shakeSpeed += (glm::vec3){10 * fDeltaTime, 10 * fDeltaTime, 10 * fDeltaTime};
	}
	glm::vec3 newDeviation = (glm::vec3){(p.noise(t, 0, 0) - 0.5) * shakeSpeed.x, (p.noise(0, t, 0) - 0.5) * shakeSpeed.y, (p.noise(0, 0, t) - 0.5) * shakeSpeed.z};
	double deltaX = newDeviation.x - camDeviation.x;
	double deltaY = newDeviation.y - camDeviation.y;
	double deltaZ = newDeviation.z - camDeviation.z;
	camDeviation = newDeviation;
	g_Camera.Translate(glm::vec3(deltaX, deltaY, deltaZ));

	glm::vec3 p = scene.getPosition();

	double targetZRotation = (g_D - g_A) * (35/360.0) * 2 * glm::pi<double>();
	double deltaZR = (targetZRotation - shipCurrentZRotation) * 100 * fDeltaTime * (70/360.0) * 2 * glm::pi<double>();

	if (!shipDead) {
		shipCurrentZRotation += deltaZR;
		ship.rotate(glm::quat(glm::cos( deltaZR / 2.0 ), 0, 0, glm::sin( deltaZR / 2.0 )));

		double sDX = 3000 * fDeltaTime * shipCurrentZRotation;
		double sDZ = -2000 * fDeltaTime;
		if (p.z < -300) {sDZ += fMSizeZ * 2 * wMSizeZ;}

		float sizeDist = 4;

		if (p.x < (0 - (fWidth/2.0)) * (fMSizeX + sizeDist)) {
			scene.setPosition(glm::vec3((0 - (fWidth/2.0)) * (fMSizeX + sizeDist), p.y, p.z));
		}
		else if (p.x > (fWidth/2.0) * (fMSizeX + sizeDist)) {
			scene.setPosition(glm::vec3((fWidth/2.0) * (fMSizeX + sizeDist), p.y, p.z));
		}
		scenePosition += glm::vec3(sDX, 0, sDZ);
		if (scenePosition.x >= (0 - (fWidth/2.0)) * (fMSizeX + sizeDist) && scenePosition.x <= (fWidth/2.0) * (fMSizeX + sizeDist))
		{
			ship.setPosition(glm::vec3(0,0,0));
			scene.move({sDX, 0, 0});
		} else {
			ship.move(glm::vec3(-sDX,0,0));
			if ((scenePosition.x >= p.x + 10 || scenePosition.x <= p.x - 10))
			{
				Mix_PlayChannel(-1, soundObject.eff["Explosion"], 0);
				shipDead = true;
			}
		}
		scene.move({0, 0, sDZ});
	} else {
		if (explosionTime >= 110) {
			currentScene = 4;
		}
	}
}

void IdleGL()
{
    g_CurrentTicks = std::clock();
    float deltaTicks = (float)( g_CurrentTicks - g_PreviousTicks );
    g_PreviousTicks = g_CurrentTicks;

    fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;
    progTime += fDeltaTime;

    /*

    glm::vec3 shakeSpeed = {0.5,0.5,0.5};
	//shakeAngle = moveAround(shakeAngle, 1, 0.1, camDeviation, (glm::vec2){0.0001,0});
	//double deltaX = glm::cos(shakeAngle) * fDeltaTime * shakeSpeed;
	//double deltaY = glm::sin(shakeAngle) * fDeltaTime * shakeSpeed;
	float t = progTime * 100;
	glm::vec3 newDeviation = (glm::vec3){(p.noise(t, 0, 0) - 0.5) * shakeSpeed.x, (p.noise(0, t, 0) - 0.5) * shakeSpeed.y, (p.noise(0, 0, t) - 0.5) * shakeSpeed.z};
	double deltaX = newDeviation.x - camDeviation.x;
	double deltaY = newDeviation.y - camDeviation.y;
	double deltaZ = newDeviation.z - camDeviation.z;
	camDeviation = newDeviation;
	g_Camera.Translate(glm::vec3(deltaX, deltaY, deltaZ));
	*/

    switch (currentScene) {
    case 0:
    	break;
    case 1:
    	gameIdle(fDeltaTime);
    	break;
    case 2:
    	break;
    case 3:
    	break;
    case 4:
    	break;
    }

    //g_Camera.Translate( glm::vec3( g_D - g_A, g_Q - g_E, g_S - g_W ) * cameraSpeed * fDeltaTime );
    //g_Camera.Rotate(glm::angleAxis<float>( glm::radians(0.1) * 0.5f, glm::vec3(g_up - g_down, 0, 0) ));
	//g_Camera.Rotate(glm::angleAxis<float>( glm::radians(fDeltaTime * cRotSpeed) * 0.5f, glm::vec3(0, g_left - g_right, 0) ));

    glutPostRedisplay();
}

std::vector<std::string> getText(std::string line)
{
	std::vector<std::string> l;
	std::string c = "";
	while (line.size() > 0) {
		c = c.insert(0, 1, line.at(0));
		if (c == " ") c = "space";
		l.push_back(c);
		line.erase(0, 1);
		c = "";
	}
	return l;
}

void displayTextLine(std::string line, int x, int y, int height, int justifyX, int justifyY, float spacing)
{
	let = getText(line);
	int startX, startY;
	float charWidth = height * 2.0 / 3;
	float spacingM = charWidth * 0.25 * spacing;
	float totalWidth = (charWidth + spacingM) * let.size() - spacingM;
	text2D.setScale({height, height});
	switch ( justifyX ) {
	case 0:
		startX = x;
		break;
	case 1:
		startX = x - totalWidth/2;
		break;
	case 2:
		startX = x - totalWidth;
		break;
	}

	switch ( justifyY ) {
	case 0:
		startY = y;
		break;
	case 1:
		startY = y - height/2;
		break;
	case 2:
		startY = y - height;
		break;
	}

	text2D.setPosition({-startX, startY, 0});

	for (int i = 0; i < let.size(); i ++) {
		changeData(text2D, alphabet[let.at(i)], nul);
		drawModel(text2D, 0, vecShader, UI);
		text2D.move({charWidth + spacingM, 0, 0});
	}
}
void displayTextLine(int x, int y, int height, int justifyX, int justifyY, float spacing)
{
	int startX, startY;
	float charWidth = height * 2.0 / 3;
	float spacingM = charWidth * 0.25 * spacing;
	float totalWidth = (charWidth + spacingM) * let.size() - spacingM;
	text2D.setScale({height, height});
	switch ( justifyX ) {
	case 0:
		startX = x;
		break;
	case 1:
		startX = x - totalWidth/2;
		break;
	case 2:
		startX = x - totalWidth;
		break;
	}

	switch ( justifyY ) {
	case 0:
		startY = y;
		break;
	case 1:
		startY = y - height/2;
		break;
	case 2:
		startY = y - height;
		break;
	}

	text2D.setPosition({-startX, startY, 0});

	for (int i = 0; i < let.size(); i ++) {
		changeData(text2D, alphabet[let.at(i)], nul);
		drawModel(text2D, 0, vecShader, UI);
		text2D.move({charWidth + spacingM, 0, 0});
	}
}

void mainMenu()
{
	displayTextLine("raster racer", g_iWindowWidth / 2, g_iWindowHeight - (60) * g_iWindowHeight / 576, (75) * g_iWindowHeight / 576, 1, 2, 1);
	displayTextLine("vectorized", g_iWindowWidth / 2, g_iWindowHeight - (15 + 30 + 15 + 75 + 15 + 20) * g_iWindowHeight / 576, (50) * g_iWindowHeight / 576, 1, 2, 1);
	int p = (int)(progTime * 8) % 12;
	switch (p) {
	case 0:
		displayTextLine("game by dj_level_3", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		displayTextLine("press space", g_iWindowWidth / 2, 30 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 0, 1);
		break;
	case 1:
		displayTextLine("game by dj_level_3", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		break;
	case 4:
		displayTextLine("music by chris allen", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		displayTextLine("press space", g_iWindowWidth / 2, 30 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 0, 1);
		break;
	case 5:
		displayTextLine("music by chris allen", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		break;
	case 8:
		displayTextLine("made for voxelbee", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		displayTextLine("press space", g_iWindowWidth / 2, 30 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 0, 1);
		break;
	case 9:
		displayTextLine("made for voxelbee", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		break;
	case 2:
		displayTextLine("game by dj_level_3", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		displayTextLine("press space", g_iWindowWidth / 2, 30 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 0, 1);
		break;
	case 3:
		displayTextLine("game by dj_level_3", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		break;
	case 6:
		displayTextLine("music by chris allen", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		displayTextLine("press space", g_iWindowWidth / 2, 30 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 0, 1);
		break;
	case 7:
		displayTextLine("music by chris allen", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		break;
	case 10:
		displayTextLine("made for voxelbee", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		displayTextLine("press space", g_iWindowWidth / 2, 30 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 0, 1);
		break;
	case 11:
		displayTextLine("made for voxelbee", g_iWindowWidth / 2, 150 * g_iWindowHeight / 576, (40) * g_iWindowHeight / 576, 1, 1, 1);
		break;
	}
	menuRotation += 15 * fDeltaTime;
	ship.rotate(glm::quat(glm::cos( (15 * fDeltaTime) / 2.0 ), 0, glm::sin( (15 * fDeltaTime) / 2.0 ), 0));
	drawModel(ship, 0, vecShader, g_Camera);
}

void game()
{
	drawModel(objfloor, 0, coverShaderTile, g_Camera);
	drawModelOver(objfloor, 1, vecShaderTile, g_Camera);
	drawModel(objwall, 0, coverShaderTile, g_Camera);
	drawModelOver(objwall, 1, vecShaderTile, g_Camera);
	if (shipDead) {
		if (ePSplash.size() == 0) {
			explosionTime = 0.0;
			ePSplash = drawSplash(1000, 0.1, explosionParticle, 0, vecShader, g_Camera);
		} else {
			explosionTime += 300 * fDeltaTime;
			if (explosionTime < 100) {
				drawSplash(ePSplash, (int)(1000 - 10 * explosionTime), 0.1, explosionTime/10, explosionParticle, 0, vecShader, g_Camera);
			}
		}
	} else {
		drawModel(ship, 0, vecShader, g_Camera);
		drawModel(exhaust, 0, vecShader, g_Camera);
	}
}

void gameOver()
{
	g_Camera.SetPosition({0, 5, -10});
	ship.setPosition({0,0,0});
	exhaust.setScale({1,1,0} );
	displayTextLine("game over", g_iWindowWidth / 2, g_iWindowHeight - (60) * g_iWindowHeight / 576, (75) * g_iWindowHeight / 576, 1, 2, 1);
}

bool getInPosition()
{
	bool sceneReady = false;
	bool inPosition = false;
	bool inRotation = false;
	if (g_Camera.GetPosition().y - 60 * fDeltaTime > g_InitialCameraPosition.y)
	{
		g_Camera.Translate(glm::vec3(0,-60 * fDeltaTime,0), false);
	} else {
		g_Camera.SetPosition(g_InitialCameraPosition);
		inPosition = true;
	}

	if (scene.getPosition().y + 8000 * fDeltaTime < 0) {
		scene.move({0, 8000 * fDeltaTime, 0});
	} else {
		scene.setPosition({0,0,0});
		sceneReady = true;
	}

	drawModel(objfloor, 0, coverShaderTile, g_Camera);
	drawModelOver(objfloor, 1, vecShaderTile, g_Camera);
	drawModel(objwall, 0, coverShaderTile, g_Camera);
	drawModelOver(objwall, 1, vecShaderTile, g_Camera);

	menuRotation = fmod(menuRotation, 6.282);

	if (menuRotation + 35 * fDeltaTime < 6.282 && ship.getRotation() != glm::quat(1,0,0,0))
	{
		menuRotation += 35 * fDeltaTime;
		ship.rotate(glm::quat(glm::cos( (35 * fDeltaTime) / 2.0 ), 0, glm::sin( (35 * fDeltaTime) / 2.0 ), 0));
	} else {
		ship.setRotation(glm::quat(1,0,0,0));
		inRotation = true;
	}
	drawModel(ship, 0, vecShader, g_Camera);

	if (inPosition && inRotation && sceneReady) {
		return true;
	}
	return false;
}

float speed = 0;

bool thrustOn()
{
	scene.move({0,0,-speed});
	speed += 2000 * fDeltaTime;
	drawModel(objfloor, 0, coverShaderTile, g_Camera);
	drawModelOver(objfloor, 1, vecShaderTile, g_Camera);
	drawModel(objwall, 0, coverShaderTile, g_Camera);
	drawModelOver(objwall, 1, vecShaderTile, g_Camera);
	drawModel(ship, 0, vecShader, g_Camera);
	exhaust.setScale({1,1, exhaust.getScale().z + fDeltaTime * 20});
	drawModel(exhaust, 0, vecShader, g_Camera);
	if (exhaust.getScale().z < 1) return false;
	exhaust.setScale({1,1,1});
	speed = 0;
	return true;
}

void gameReady()
{
	if (getInPosition())
	{
		if (thrustOn()) {
			resetGame();
			currentScene = 1;
		}
	}
}

void DisplayGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    switch (currentScene) {
    case 0: {
    	mainMenu();
    	break;
    }
    case 1:
    	game();
		break;
    case 2:
    	displayTextLine(0, 0, 100, 0, 0, 1);
    	text2D.setPosition({0,0,0});
    	break;
    case 3:
    	if (ePSplash.size() == 0) {
    		explosionTime = 0.0;
    		ePSplash = drawSplash(1000, 0.3, explosionParticle, 0, vecShader, g_Camera);
    	} else {
    		explosionTime += 300 * fDeltaTime;
    		if (explosionTime < 100) {
    			drawSplash(ePSplash, (int)(1000 - 10 * explosionTime), 0.3, explosionTime/10, explosionParticle, 0, vecShader, g_Camera);
    		} else {
    			ePSplash = {};
    		}
    	}
    	break;
    case 4:
    	gameOver();
    	break;
    case 5:
    	gameReady();
    	break;
    }

    glutSwapBuffers();
}

void KeyboardGL( unsigned char c, int x, int y )
{
	int cVolume = Mix_Volume(-1, -1);
	switch (c) {
	case '-':
		Mix_Volume(-1, cVolume - (cVolume - 10 >= 0 ? 10 : 0));
		Mix_VolumeMusic(cVolume - (cVolume - 10 >= 0 ? 10 : 0));
		break;
	case '=':
		Mix_Volume(-1, cVolume + (cVolume + 10 <= 128 ? 10 : 0));
		Mix_VolumeMusic(cVolume + (cVolume + 10 <= 128 ? 10 : 0));
		break;
	}
	switch ( currentScene ) {
	case 0:
		switch ( c ) {
		case 27:
			glutLeaveMainLoop();
			break;
		case ' ':
			scene.setPosition((glm::vec3){0, -700, 0});
			Mix_PlayMusic(soundObject.mus["Exit Strategy"], -1);
			currentScene = 5;
			break;
		}
		break;
	case 1:
		switch ( c )
		{
		case 'w':
		case 'W':
			g_W = 1;
			break;
		case 'a':
		case 'A':
			g_A = 1;
			break;
		case 's':
		case 'S':
			g_S = 1;
			break;
		case 'd':
		case 'D':
			g_D = 1;
			break;
		case 'q':
		case 'Q':
			g_Q = 1;
			break;
		case 'e':
		case 'E':
			g_E = 1;
			break;
		case 'r':
		case 'R':
			g_Camera.SetPosition( g_InitialCameraPosition );
			g_Camera.SetRotation( g_InitialCameraRotation );
			g_Rotation = glm::quat(1,0,0,0);
			break;
		case 27:
			Mix_PlayChannel(-1, soundObject.eff["Explosion"], 0);
			shipDead = true;
			break;
		}
		break;
	case 2:
		switch ( c ) {
		case 27:
			if (let.size()) let.clear();
			else glutLeaveMainLoop();
			break;
		case ' ': {
			let.push_back("space");
			for (int i = 0; i < let.size(); i ++) std::cout << let.at(i) << std::endl;
			std::cout << std::endl;
			break;
		}
		default: {
			std::string l;
			l.insert(0, 1, c);
			let.push_back(l);
			for (int i = 0; i < let.size(); i ++) std::cout << let.at(i) << std::endl;
			std::cout << std::endl;
		}
		}
		break;
	case 4:
		switch ( c ) {
		case 27:
			currentScene = 0;
			ship.setRotation({1,0,0,0});
			g_Camera.SetPosition({0, 10, -10});
			Mix_PlayMusic(soundObject.mus["Moon Patrol"], -1);
			break;
		case ' ':
			scene.setPosition({0,-700,0});
			ship.setRotation({1,0,0,0});
			g_Camera.SetPosition({0, 10, -10});
			currentScene = 5;
			Mix_PlayMusic(soundObject.mus["Exit Strategy"], -1);
			break;
		}
		break;
	case 5:
		switch ( c ) {
		case 27:
			scene.setPosition(scenePosition);
			resetGame();
			currentScene = 1;
			break;
		case ' ':
			scene.setPosition(scenePosition);
			resetGame();
			currentScene = 1;
			break;
		}
		break;
	}
}

void KeyboardUpGL( unsigned char c, int x, int y )
{
    switch ( c )
    {
    case 'w':
    case 'W':
        g_W = 0;
        break;
    case 'a':
    case 'A':
        g_A = 0;
        break;
    case 's':
    case 'S':
        g_S = 0;
        break;
    case 'd':
    case 'D':
        g_D = 0;
        break;
    case 'q':
    case 'Q':
        g_Q = 0;
        break;
    case 'e':
    case 'E':
        g_E = 0;
        break;

    default:
        break;
    }
}

void SpecialGL( int key, int x, int y )
{
    switch( key )
    {
    case GLUT_KEY_SHIFT_L:
    case GLUT_KEY_SHIFT_R:
        {
            g_bShift = true;
        }
        break;
    case GLUT_KEY_LEFT:
		{
			g_left = true;
		}
		break;
	case GLUT_KEY_RIGHT:
		{
			g_right = true;
		}
		break;
	case GLUT_KEY_UP:
		{
			g_up = true;
		}
		break;
	case GLUT_KEY_DOWN:
		{
			g_down = true;
		}
		break;
	}
}

void SpecialUpGL( int key, int x, int y )
{
    switch( key )
    {
    case GLUT_KEY_SHIFT_L:
    case GLUT_KEY_SHIFT_R:
        {
            g_bShift = false;
        }
        break;
    case GLUT_KEY_LEFT:
		{
			g_left = false;
		}
		break;
	case GLUT_KEY_RIGHT:
		{
			g_right = false;
		}
		break;
	case GLUT_KEY_UP:
		{
			g_up = false;
		}
		break;
	case GLUT_KEY_DOWN:
		{
			g_down = false;
		}
		break;
	}
}

void MouseGL( int button, int state, int x, int y )
{
    g_MousePos = glm::ivec2(x,y);
}

void MotionGL( int x, int y )
{
    glm::ivec2 mousePos = glm::ivec2( x, y );
    glm::vec2 delta = glm::vec2( mousePos - g_MousePos );
    g_MousePos = mousePos;

    //std::cout << "dX: " << delta.x << " dy: " << delta.y << std::endl;

    //glm::quat rotX = glm::angleAxis<float>( glm::radians(delta.y) * 0.5f, glm::vec3(1, 0, 0) );
    //glm::quat rotY = glm::angleAxis<float>( glm::radians(delta.x) * 0.5f, glm::vec3(0, 1, 0) );

    //g_Camera.Rotate( rotX * rotY );
    //g_Rotation = ( rotX * rotY ) * g_Rotation;
}

void ReshapeGL( int w, int h )
{
	int rW, rH;
    if ( h == 0 )
    {
        h = 1;
    }
    if (w == 0)
    {
    	w = 1;
    }

    if (w / (float)h > 16/9.0) {
    	rH = h;
    	rW = (16 / 9.0) * rH;
    } else {
    	rW = w;
    	rH = (9.0 / 16) * rW;
    }

    g_iWindowWidth = rW;
    g_iWindowHeight = rH;

    g_Camera.SetViewport( 0, 0, rW, rH );
    g_Camera.SetProjectionRH( 80.0f, rW/(float)rH, 1.0f, 500.0f );

    UI.SetViewport( 0, 0, rW, rH );
    UI.SetProjectionRH(-1, rW - 1, 0, rH, -1, 10);

    glutPostRedisplay();
}

void InitGL( int argc, char* argv[] )
{
    std::cout << "Initialize OpenGL..." << std::endl;

    glutInit( &argc, argv );

    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

    int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);

    // Create an OpenGL 3.3 core forward compatible context.
    glutInitContextVersion( 3, 3 );
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );

    glutInitWindowPosition( ( iScreenWidth - g_iWindowWidth ) / 2, (iScreenHeight - g_iWindowHeight) / 2 );
    glutInitWindowSize( g_iWindowWidth, g_iWindowHeight );

    g_iWindowHandle = glutCreateWindow("OpenGL Template");

    // Register GLUT callbacks.
    glutIdleFunc(IdleGL);
    glutDisplayFunc(DisplayGL);
    glutKeyboardFunc(KeyboardGL);
    glutKeyboardUpFunc(KeyboardUpGL);
    glutSpecialFunc(SpecialGL);
    glutSpecialUpFunc(SpecialUpGL);
    glutMouseFunc(MouseGL);
    glutMotionFunc(MotionGL);
    glutReshapeFunc(ReshapeGL);

    glClearColor(0.0, 0.0, 0.0, 1.0 );
    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
    glEnable(GL_DEPTH_TEST);

    std::cout << "Initialize OpenGL Success!" << std::endl;
}

void InitGLEW()
{
    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK )
    {
        std::cerr << "There was a problem initializing GLEW. Exiting..." << std::endl;
        exit(-1);
    }

    // Check for 3.3 support.
    // I've specified that a 3.3 forward-compatible context should be created.
    // so this parameter check should always pass if our context creation passed.
    // If we need access to deprecated features of OpenGL, we should check
    // the state of the GL_ARB_compatibility extension.
    if ( !GLEW_VERSION_3_3 )
    {
        std::cerr << "OpenGL 3.3 required version support not present." << std::endl;
        exit(-1);
    }

#ifdef _WIN32
    if ( WGLEW_EXT_swap_control )
    {
        wglSwapIntervalEXT(0); // Disable vertical sync
    }
#endif
}

GLuint LoadShader( GLenum shaderType, const std::string& shaderFile )
{
    std::ifstream ifs;

    // Load the shader source file.
    ifs.open(shaderFile);

    if ( !ifs )
    {
        std::cerr << "Can not open shader file: \"" << shaderFile << "\"" << std::endl;
        return 0;
    }

    std::string source( std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) );
    ifs.close();

    GLuint shader = glCreateShader( shaderType );

    // Load the shader source for each shader object.
    const GLchar* sources[] = { source.c_str() };
    glShaderSource( shader, 1, sources, NULL );
    glCompileShader( shader );

    // Check for errors
	GLint compileStatus;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus != GL_TRUE )
	{
		GLint logLength;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog( shader, logLength, NULL, infoLog );

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		delete infoLog;
		return 0;
	}
	return shader;
}

GLuint CreateShaderProgram( std::vector<GLuint> &shaders )
{
    // Create a shader program.
    GLuint program = glCreateProgram();

    // Attach the appropriate shader objects.
    for( GLuint shader: shaders )
    {
        glAttachShader( program, shader );
    }

    // Link the programfMSizeZ
	glLinkProgram(program);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus != GL_TRUE )
	{
		GLint logLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog( program, logLength, NULL, infoLog );

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif

		delete infoLog;
		return 0;
	}
	return program;
}

int main( int argc, char* argv[] )
{
	p = PerlinNoise(static_cast <unsigned int> (time(0)));
	srand(static_cast <unsigned int> (time(0)));
	// audioTest(); return 0;

	g_PreviousTicks = std::clock();
	g_A = g_W = g_S = g_D = g_Q = g_E = 0;

	g_Camera.SetPosition( g_InitialCameraPosition );
	g_Camera.SetRotation( g_InitialCameraRotation );

	InitGL(argc, argv);
	InitGLEW();

	GLuint vertexShader,geoShader,fragmentShader;
	std::vector<GLuint> shaders;

	vertexShader = LoadShader( GL_VERTEX_SHADER, "../resources/shaders/vecshader/vertex.glsl" );
	geoShader = LoadShader( GL_GEOMETRY_SHADER, "../resources/shaders/vecshader/geometry.glsl" );
	fragmentShader = LoadShader( GL_FRAGMENT_SHADER, "../resources/shaders/vecshader/fragment.glsl" );

	shaders.push_back(vertexShader);
	shaders.push_back(geoShader);
	shaders.push_back(fragmentShader);

	// Create the shader program.
	vecShader.program = CreateShaderProgram( shaders );
	assert( vecShader.program != 0 );

	vecShader.attribIDPosition = glGetAttribLocation( vecShader.program, "inPosition" );
	vecShader.attribIDColor = glGetAttribLocation( vecShader.program, "inColor" );
	vecShader.mvp = glGetUniformLocation( vecShader.program, "mvp" );

	shaders.clear();

	vertexShader = LoadShader( GL_VERTEX_SHADER, "../resources/shaders/vecshadertile/vertex.glsl" );
	geoShader = LoadShader( GL_GEOMETRY_SHADER, "../resources/shaders/vecshadertile/geometry.glsl" );
	fragmentShader = LoadShader( GL_FRAGMENT_SHADER, "../resources/shaders/vecshadertile/fragment.glsl" );

	shaders.push_back(vertexShader);
	shaders.push_back(geoShader);
	shaders.push_back(fragmentShader);

	// Create the shader program.
	vecShaderTile.program = CreateShaderProgram( shaders );
	assert( vecShaderTile.program != 0 );

	vecShaderTile.attribIDPosition = glGetAttribLocation( vecShaderTile.program, "inPosition" );
	vecShaderTile.attribIDColor = glGetAttribLocation( vecShaderTile.program, "inColor" );
	vecShaderTile.attribIDOffset = glGetAttribLocation( vecShaderTile.program, "inOffset" );
	vecShaderTile.mvp = glGetUniformLocation( vecShaderTile.program, "mvp" );

	shaders.clear();

	vertexShader = LoadShader( GL_VERTEX_SHADER, "../resources/shaders/flatshadertile/vertex.glsl" );
	geoShader = LoadShader( GL_GEOMETRY_SHADER, "../resources/shaders/flatshadertile/geometry.glsl" );
	fragmentShader = LoadShader( GL_FRAGMENT_SHADER, "../resources/shaders/flatshadertile/fragment.glsl" );

	shaders.push_back(vertexShader);
	shaders.push_back(geoShader);
	shaders.push_back(fragmentShader);

	// Create the shader program.
	coverShaderTile.program = CreateShaderProgram( shaders );
	assert( coverShaderTile.program != 0 );

	coverShaderTile.attribIDPosition = glGetAttribLocation( coverShaderTile.program, "inPosition" );
	coverShaderTile.attribIDColor = glGetAttribLocation( coverShaderTile.program, "inColor" );
	coverShaderTile.attribIDOffset = glGetAttribLocation( coverShaderTile.program, "inOffset" );
	coverShaderTile.mvp = glGetUniformLocation( coverShaderTile.program, "mvp" );

	ship = Object();
	ship.setVertices(shipVertices);
	ship.setIndices(shipIndices);
	ship.setPosition((glm::vec3){0,0,0});
	ship.setRotation((glm::quat){1,0,0,0});
	ship.setScale((glm::vec3){1,1,1});
	ship.genVAO(0, vecShader);
	ship.getMatrix();

	exhaust = Object(&ship);
	exhaust.setVertices(exhaustVertices);
	exhaust.setIndices(exhaustIndices);
	exhaust.setPosition((glm::vec3){0,-0.75,-4.5});
	exhaust.setRotation((glm::quat){1,0,0,0});
	exhaust.setScale((glm::vec3){1,1,0});
	exhaust.genVAO(0, vecShader);
	exhaust.getMatrix();

	fMScale = 2;
	fMSizeX = 6;
	fMSizeZ = 10;

	wMSizeZ = 5;

	fWidth = 12;
	fLength = 200;

	for (double i = (0 - (fWidth/2.0)) * fMSizeX; i < (fWidth/2.0) * fMSizeX; i += fMSizeX)
	{
		for (double j = 40; j < fLength * fMSizeZ; j += fMSizeZ)
		{
			floorOffsets.push_back((glm::vec3){(int)i, 0, (int)j});
		}
	}

	for (double i = (0 - (fWidth/2.0)) * fMSizeX; i <= (fWidth/2.0) * fMSizeX; i += fWidth * fMSizeX)
	{
		for (double j = 40; j < fLength * fMSizeZ; j += wMSizeZ)
		{
			wallOffsets.push_back((glm::vec3){(int)i, 0, (int)j});
		}
	}

	//floorOffsets.push_back((glm::vec3){0,0,0});

	objfloor = TileObject(&scene);
	objfloor.setVertices(floorVertices);
	objfloor.setIndices(floorIndices);
	objfloor.setOffsets(floorOffsets);
	objfloor.setPosition((glm::vec3){0,-3.5,0});
	objfloor.setRotation((glm::quat){1,0,0,0});
	objfloor.setScale((glm::vec3){fMScale,fMScale,fMScale});
	objfloor.genVAO(0, coverShaderTile);
	objfloor.genVAO(1, vecShaderTile);
	objfloor.getMatrix();

	objwall = TileObject(&scene);
	objwall.setVertices(wallVertices);
	objwall.setIndices(wallIndices);
	objwall.setOffsets(wallOffsets);
	objwall.setPosition((glm::vec3){0,-3.5,0});
	objwall.setRotation((glm::quat){1,0,0,0});
	objwall.setScale((glm::vec3){fMScale,fMScale,fMScale});
	objwall.genVAO(0, coverShaderTile);
	objwall.genVAO(1, vecShaderTile);
	objwall.getMatrix();

	text = Object();
	text.setVertices(defaultVertices);
	text.setIndices(defaultIndices);
	text.setPosition((glm::vec3){0,0,0});
	text.setRotation((glm::quat){1,0,0,0});
	text.setScale((glm::vec3){2,2,2});
	text.genVAO(0, vecShader);
	text.getMatrix();

	text2D = Object2D();
	text2D.setVertices(defaultVertices);
	text2D.setIndices(defaultIndices);
	text2D.setPosition((glm::vec3){0,0,0});
	text2D.setRotation((glm::quat){1,0,0,0});
	text2D.setScale((glm::vec2){50,50});
	text2D.genVAO(0, vecShader);
	text2D.getMatrix();

	explosionParticle = Object(&ship);
	explosionParticle.setVertices(particleVertices);
	explosionParticle.setIndices(particleIndices);
	explosionParticle.setPosition((glm::vec3){0,0,0});
	explosionParticle.setRotation((glm::quat){1,0,0,0});
	explosionParticle.setScale((glm::vec3){1,1,1});
	explosionParticle.genVAO(0, vecShader);
	explosionParticle.getMatrix();

	alphabet = loadLetters("../resources/alphabet.ldf", alphabet);

	glLineWidth(8);

	g_Camera.SetRotation(g_InitialCameraRotation);
	g_Camera.SetPosition(glm::vec3(0, 10, -10));

	soundObject = openAudio(44100, 2, 1024);
	soundObject.mus = mapMusic("../resources/audio/Exit Strategy.ogg",           "Exit Strategy", soundObject.mus);
	soundObject.mus = mapMusic("../resources/audio/Moon Patrol De-Rastered.ogg", "Moon Patrol",   soundObject.mus);
	soundObject.eff = mapEffect("../resources/audio/bangLarge.wav",              "Explosion",     soundObject.eff);
	Mix_PlayMusic(soundObject.mus["Moon Patrol"], -1);

	glutMainLoop();

	Mix_HaltMusic();
	closeAudio(soundObject);
}
