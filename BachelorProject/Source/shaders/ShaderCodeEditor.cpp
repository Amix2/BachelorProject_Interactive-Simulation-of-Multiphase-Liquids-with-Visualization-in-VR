#include "ShaderCodeEditor.h"

std::string replaceAll(std::string str, const std::string& from, const std::string& to);

void ShaderCodeEditor::init()
{
	if (initialized) return;
	initialized = true;

	addVariable(	"MAX_FLUID",	Configuration.MAX_FLUID_PARTICLES	);
	addVariable(	"MAX_GLASS",	Configuration.MAX_GLASS_PARTICLES	);
	addVariable(	"MAX_SPEED",	Configuration.MAX_PARTICLE_SPEED	);
	addVariable(	"MAX_FLUID_TYPES",	Configuration.MAX_FLUID_TYPES	);
	addVariable(	"SORT_ARRAY_SIZE",	Configuration.SORT_ARRAY_SIZE	);
	addVariable("MAX_SCENE_X", Configuration.SCENE_SIZE_X);
	addVariable("MAX_SCENE_Z", Configuration.SCENE_SIZE_Y);
	addVariable("MAX_SCENE_Y", Configuration.SCENE_SIZE_Z);
	addVariable("DELTA_TIME", Configuration.DELTA_TIME);
	addVariable("GRAVITY_Y", Configuration.GRAVITY_Y);
	addVariable("MAX_FLUID_SPEED", Configuration.MAX_FLUID_SPEED);
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