#include <string>
#include <vector>
namespace constants {
	const float SEA_LEVEL = 0.01f;
	const float WAVE_SPEED = 0.03f;
	const int REFLECTION_WIDTH = 320;
	const int REFLECTION_HEIGHT = 180;
	const int REFRACTION_WIDTH = 1280;
	const int REFRACTION_HEIGHT = 720;

	const std::string SHADER_PATH = "../shaders/";
	const std::string WATER_PATH = "../water/";
	const std::string TEXTURE_PATH = "../textures/";
	const std::string SKYBOX_PATH = "../skybox/";

	const std::vector<const GLchar*> SKYBOX_FACES = {
		(SKYBOX_PATH + "right.jpg").c_str(), 
		(SKYBOX_PATH + "left.jpg").c_str(), 
		(SKYBOX_PATH + "top.jpg").c_str(),
		(SKYBOX_PATH + "bottom.jpg").c_str(), 
		(SKYBOX_PATH + "back.jpg").c_str(), 
		(SKYBOX_PATH + "front.jpg").c_str()
	};

	const std::vector<std::string> TERRAIN_TEXTURE_NAMES = {
		"ocean.jpg", "sand.jpg", "grass_4.jpg", "grass_3.jpg", 
		"grass_rock.jpg", "snow.jpg"
	};

	const std::string WATER_DUDV_NAME = "waterDUDV.png";

	const GLchar* SKYBOX_VERT_SHADER = (SHADER_PATH + "skybox.vert").c_str();
	const GLchar* SKYBOX_FRAG_SHADER = (SHADER_PATH + "skybox.frag").c_str();

	const GLchar* WATER_VERT_SHADER = (SHADER_PATH + "water.vert").c_str();
	const GLchar* WATER_FRAG_SHADER = (SHADER_PATH + "water.frag").c_str();

	const GLchar* NORMALS_VERT_SHADER = (SHADER_PATH + "normals.vert").c_str();
	const GLchar* NORMALS_FRAG_SHADER = (SHADER_PATH + "normals.frag").c_str();
	const GLchar* NORMALS_GEOM_SHADER = (SHADER_PATH + "normals.gs").c_str();
}