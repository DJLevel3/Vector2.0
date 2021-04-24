#include "main.hpp"
#include "PerlinNoise.h"
#include "structs.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "audio.hpp"

using namespace vector2;

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
}

glm::vec3 g_InitialCameraPosition = glm::vec3( 0, 5, -10 );;
glm::quat g_InitialCameraRotation = glm::quat(0,0,1,0) * glm::quat(0.991, -0.131, 0, 0);

glm::vec3 camDeviation = glm::vec3(0,0,0);
double shakeAngle;
double progTime = 0;
PerlinNoise p;

shader vecShader;

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
		0,     1,    2,  // 0
		0,     1,    4,  // 1
		3,     1,    2,  // 2
		3,     1,    4,  // 3
		2,     1,    4,  // 4
		3,     5,    6,  // 5
		3,     5,    7,  // 6
		3,     6,    8,  // 7
};
Object ship;

// Floor
std::vector<vertex> floorVertices = {
		{glm::vec3(-1.00,  0.00,  0.00), glm::vec3(1,0,0)},  // 0
		{glm::vec3( 1.00,  0.00,  0.00), glm::vec3(1,0,0)},  // 1
		{glm::vec3( 0.00,  0.00,  1.00), glm::vec3(1,0,0)},  // 2
		{glm::vec3( 0.00,  0.00, -1.00), glm::vec3(1,0,0)},  // 3
		{glm::vec3(-1.00,  0.00,  2.00), glm::vec3(1,0,0)},  // 4
		{glm::vec3( 1.00,  0.00,  2.00), glm::vec3(1,0,0)},  // 5
};
std::vector<GLuint> floorIndices = {
		0,     1,    2,  // 0
		0,     1,    3,  // 1
		0,     4,    2,  // 2
		1,     5,    2   // 3
};
Object objfloor;

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

double moveAround(double angle, double maxDeviation, double gravity, glm::vec2 position, glm::vec2 center)
{
	double pi = glm::pi<double>();
	double radFromCenter;
	if (position != center) radFromCenter = glm::mod(-angle + glm::atan(center.y - position.y, center.x - position.x) + pi, (2 * pi)) - pi;
	else radFromCenter = 0;
	double distFromCenter = glm::distance(center, position);
	double pull = gravity * distFromCenter * radFromCenter;
	double r = p.noise(progTime/100, 0, 0);
	double deviation = 2 * maxDeviation * r - maxDeviation;
	return glm::mod((angle + deviation + pi) + pull, (2 * pi)) - pi;
}

void IdleGL()
{
    g_CurrentTicks = std::clock();
    float deltaTicks = (float)( g_CurrentTicks - g_PreviousTicks );
    g_PreviousTicks = g_CurrentTicks;

    float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;
    progTime += fDeltaTime;

    glm::vec3 shakeSpeed = {1.8,1,1};
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

    //g_Camera.Translate( glm::vec3( g_D - g_A, g_Q - g_E, g_S - g_W ) * cameraSpeed * fDeltaTime );
    //g_Camera.Rotate(glm::angleAxis<float>( glm::radians(0.1) * 0.5f, glm::vec3(g_up - g_down, 0, 0) ));
	//g_Camera.Rotate(glm::angleAxis<float>( glm::radians(fDeltaTime * cRotSpeed) * 0.5f, glm::vec3(0, g_left - g_right, 0) ));


    glutPostRedisplay();
}

void DisplayGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    drawModel(objfloor, vecShader, g_Camera);
    drawModel(ship, vecShader, g_Camera);

    glutSwapBuffers();
}

void KeyboardGL( unsigned char c, int x, int y )
{
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
        glutLeaveMainLoop();
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

    std::cout << "dX: " << delta.x << " dy: " << delta.y << std::endl;

    //glm::quat rotX = glm::angleAxis<float>( glm::radians(delta.y) * 0.5f, glm::vec3(1, 0, 0) );
    //glm::quat rotY = glm::angleAxis<float>( glm::radians(delta.x) * 0.5f, glm::vec3(0, 1, 0) );

    //g_Camera.Rotate( rotX * rotY );
    //g_Rotation = ( rotX * rotY ) * g_Rotation;
}

void ReshapeGL( int w, int h )
{
    if ( h == 0 )
    {
        h = 1;
    }

    g_iWindowWidth = w;
    g_iWindowHeight = h;

    g_Camera.SetViewport( 0, 0, w, h );
    g_Camera.SetProjectionRH( 80.0f, w/(float)h, 1.0f, 1000.0f );

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

    glClearColor(0.0, 0.0, 0.1, 1.0 );
    glClearDepth(1.0);
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

    // Link the program
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
	// audioTest(); return 0;

	g_PreviousTicks = std::clock();
	g_A = g_W = g_S = g_D = g_Q = g_E = 0;

	g_Camera.SetPosition( g_InitialCameraPosition );
	g_Camera.SetRotation( g_InitialCameraRotation );

	InitGL(argc, argv);
	InitGLEW();

	GLuint vertexShaderV,geoShaderV,fragmentShaderV;

	vertexShaderV = LoadShader( GL_VERTEX_SHADER, "../resources/shaders/vecshader/vertex.glsl" );
	geoShaderV = LoadShader( GL_GEOMETRY_SHADER, "../resources/shaders/vecshader/geometry.glsl" );
	fragmentShaderV = LoadShader( GL_FRAGMENT_SHADER, "../resources/shaders/vecshader/fragment.glsl" );

	//vertexShaderV = LoadShader( GL_VERTEX_SHADER, "../resources/shaders/flatshader/vertex.glsl" );
	//fragmentShaderV = LoadShader( GL_FRAGMENT_SHADER, "../resources/shaders/flatshader/fragment.glsl" );

	std::vector<GLuint> shadersV;

	shadersV.push_back(vertexShaderV);
	shadersV.push_back(geoShaderV);
	shadersV.push_back(fragmentShaderV);

	//shadersV.push_back(vertexShaderV);
	//shadersV.push_back(fragmentShaderV);

	// Create the shader program.
	vecShader.program = CreateShaderProgram( shadersV );
	assert( vecShader.program != 0 );

	vecShader.attribIDPosition = glGetAttribLocation( vecShader.program, "inPosition" );
	vecShader.attribIDColor = glGetAttribLocation( vecShader.program, "inColor" );
	vecShader.mvp = glGetUniformLocation( vecShader.program, "mvp" );

	ship = Object(vecShader);
	ship.setVertices(shipVertices);
	ship.setIndices(shipIndices);
	ship.setPosition((glm::vec3){0,0,0});
	ship.setRotation((glm::quat){1,0,0,0});
	ship.setScale((glm::vec3){1,1,1});
	ship.genVAO();
	ship.getMatrix();

	objfloor = Object(vecShader);
	objfloor.setVertices(floorVertices);
	objfloor.setIndices(floorIndices);
	objfloor.setPosition((glm::vec3){0,-3.5,0});
	objfloor.setRotation((glm::quat){1,0,0,0});
	objfloor.setScale((glm::vec3){1,1,1});
	objfloor.genVAO();
	objfloor.getMatrix();

	glLineWidth(8);

	glutMainLoop();
}
