#include "ShaderCodeEditor.h"

std::string replaceAll(std::string str, const std::string& from, const std::string& to);

void ShaderCodeEditor::init()
{
	if (initialized) return;
	initialized = true;

	addVariable(	"MAX_FLUID",	Configuration.MAX_PARTICLES	);
	addVariable(	"MAX_GLASS",	Configuration.MAX_GLASS_PARTICLES	);
	addVariable(	"MAX_FLUID_TYPES",	Configuration.MAX_FLUID_TYPES	);
	addVariable(	"SORT_ARRAY_SIZE",	Configuration.SORT_ARRAY_SIZE	);

	addVariable("MAX_SCENE_X", Configuration.SCENE_SIZE_X - Configuration.DEFAULT_POSITION_OFFSET.x);
	addVariable("MAX_SCENE_Z", Configuration.SCENE_SIZE_Y - Configuration.DEFAULT_POSITION_OFFSET.y);
	addVariable("MAX_SCENE_Y", Configuration.SCENE_SIZE_Z - Configuration.DEFAULT_POSITION_OFFSET.z);
	addVariable("MIN_SCENE_X", -Configuration.DEFAULT_POSITION_OFFSET.x);
	addVariable("MIN_SCENE_Y", -Configuration.DEFAULT_POSITION_OFFSET.y);
	addVariable("MIN_SCENE_Z", -Configuration.DEFAULT_POSITION_OFFSET.z);

	addVariable("DELTA_TIME", Configuration.DELTA_TIME);
	addVariable("GRAVITY_Y", Configuration.GRAVITY_Y);
	addVariable("MAX_PARTICLE_SPEED", Configuration.MAX_PARTICLE_SPEED);
	addVariable("BOUNCE_DISTANCE", Configuration.BOUNCE_DISTANCE);
	addVariable("EMITER_FLUID_PARTICLE_BUILD_GAP", Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP);
	addVariable("MAX_EMITERS", Configuration.MAX_EMITERS);
}

template<typename V>
inline void ShaderCodeEditor::addVariable(std::string name , V value)
{
	m_valueStream <<"#undef " << name << "\n" << "#define " << name << " " << value<<"\n";
}


void ShaderCodeEditor::insertVariables(std::string &shaderStr)
{
	std::string changedStr = replaceAll(shaderStr, m_placeIndicator, m_valueStream.str());
	shaderStr.swap(changedStr);

}

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}