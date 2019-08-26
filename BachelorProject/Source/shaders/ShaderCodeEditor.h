#pragma once
#include <unordered_map>
#include <string>
#include <sstream>      
#include <iostream>
#include <algorithm>
#include <Configuration.h>

class ShaderCodeEditor
{
	inline const static std::string m_placeIndicator = "#define INSERT_VARIABLES_HERE";
	inline static std::stringstream m_valueStream;
	inline static bool initialized = false;

public:
	
	static void init();

	template <typename V>	static void addVariable(std::string name, V value);

	static void insertVariables(std::string &shaderStr);

};
