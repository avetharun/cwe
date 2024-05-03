#pragma once
#include <string>
#include <vector>

#include "binparse.h"
struct PhysWorld;
#include "engine/physics.h"
struct CWLayout;

struct CWLayout {
#pragma pack(push, 2)
	struct CustomElement {
		uint8_t name_len;
		char name[255];
		char data[1024];
	};
#pragma pack(pop)
	std::string scene_name;
	// Physics (PhysWorld) leveldata
	ABTDataStub level_data;
	// Entity (Interactions / moving elements) leveldata
	ABTDataStub entity_data;
	// Sprites (textures only) leveldata
	ABTDataStub sprite_data;
	std::unordered_map<std::string, CustomElement> element;
PhysWorld phys_world;
	static CWLayout parse(std::string filename);
	static CWLayout parse(char* data, size_t length);
	// write out current scene data
	std::string dump();
};