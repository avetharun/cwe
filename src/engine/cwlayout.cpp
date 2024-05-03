#include "engine/cwlayout.h"
#include <alib.h>
CWLayout CWLayout::parse(std::string filename) {
	std::string data = alib_file_read(filename.c_str());
	return parse((char*)data.c_str(), data.length());
}

CWLayout CWLayout::parse(char* data, size_t length) {
	auto l = CWLayout();
	cw::BinarySerializer::Reader reader(data, length);
	//l.header = reader.read_class<CWLayout::header_data_t>();
	l.scene_name = reader.read_string();
	l.level_data = reader.read_compound();
	l.entity_data = reader.read_compound();
	l.sprite_data = reader.read_compound();
	return l;
}

std::string CWLayout::dump() {
	cw::BinarySerializer::Writer writer;
	writer.write_string(scene_name);
	writer.write_compound(level_data);
	return std::string(writer.buffer.data(), writer.buffer.size());
}
