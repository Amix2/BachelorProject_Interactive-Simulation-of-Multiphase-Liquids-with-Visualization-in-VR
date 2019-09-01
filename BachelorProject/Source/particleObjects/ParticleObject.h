#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <Logger.h>
#include <Configuration.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <particleObjects/ParticleGeometry.h>
#include <dataStructures/ParticleData.h>


struct ParticleObjectDetais {
	int fluidType;	// -1 -> glass | >0 -> index in fluid type array (TODO) | 0 -> reserved
	union {
		struct {
			// for liquid box
			float lowX, lowY, lowZ;
			float highX, highY, highZ;
		};
		struct {
			// for mugs / glass
			float cX, cY, cZ; // center of inner mug
			float innerRadius;
			float thickness;
			float height;
		};
	};
};

// Represents single object like mug etc
class ParticleObject {
	// index of glass particless from this object in global glass particle array (on gpu)

public:
	glm::vec3 m_currentPosition;	// 3
	glm::vec3 m_targetPosition;		// 6

	glm::vec3 m_currentVector;		// 9
	glm::vec3 m_targetVector;		// 12

	int m_indBegin;					// 13
	int m_indEnd;					// 14

	float _padding[2];	// force 4 x N variables (4 B each)

	void createMug(ParticleObjectDetais details, float positions[], float vectors[], int& numOfParts);
	static void fun() {}
	std::string print();
	ParticleObject();
};
