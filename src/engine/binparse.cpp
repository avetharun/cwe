
#include "engine/binparse.h"
#include <crtdbg.h>
using namespace cw::BinarySerializer;

Writer Writer::write_data(char* data, size_t length) {
	buffer.resize(buffer.size() + length);
	memcpy(buffer.data() + ptr, data, length);
	ptr += length;
	return *this;
}
Writer Writer::write_ushort(unsigned short i) {
	return this->write_data((char*) & i, sizeof(unsigned short));
}

Writer Writer::write_ulong(uint64_t i) {
	return this->write_data((char*) & i, sizeof(uint64_t));
}

Writer Writer::write_ubyte(unsigned char i) {
	return this->write_data((char*) & i, sizeof(unsigned char));
}

Writer Writer::write_uint(unsigned int i) {
	return this->write_data((char*) & i, sizeof(unsigned int));
}

Writer Writer::write_float(float i) { return this->write_class<float>(i); }
float Reader::read_float() { return this->read_class<float>(); }


Writer Writer::write_sint(signed int i) { return this->write_uint(i); }
Writer Writer::write_sshort(signed short i) { return this->write_ushort(i); }
Writer Writer::write_slong(int64_t i) { return this->write_ulong(i); }
Writer Writer::write_sbyte(signed char i) { return this->write_ubyte(i); }

Writer cw::BinarySerializer::Writer::write_json(ABTDataStub json) {
	return write_string(json.dump());
}

Writer cw::BinarySerializer::Writer::write_bson(ABTDataStub bson) {
	auto bData = nlohmann::json::to_bson(bson);
	return write_string(std::string((char*)bData.data(), bData.size()));
}

Writer cw::BinarySerializer::Writer::write_compound(ABTDataStub msgpk) {
	auto bData = ABTDataStub::to_msgpack(msgpk);
	return write_string(std::string((char*)bData.data(), bData.size()));
}

long Reader::get_read_ptr() { return ptr; }
long Writer::get_write_ptr() { return ptr; }

Writer Reader::to_writer() const {
	Writer w;
	w.buffer = buffer;
	return w;
}
Reader Writer::to_reader() const {
	return Reader(buffer);
}



Reader::Reader(char* buffer, size_t length) {
	this->buffer.reserve(length);
	memcpy(this->buffer.data(), buffer, length);
}
Reader::Reader(std::vector<char> buff) { 
	buffer = buff;
}


char* Reader::read_data(size_t length) {
	char* out = (buffer.data() + ptr);
	char* data = (char*)malloc(length+1);
	_ASSERT(data != 0);
	memcpy(data, out, length);
	ptr += length;
	return data;
}
ABTDataStub cw::BinarySerializer::Reader::read_json() {
	ABTDataStub b;
	b.update(nlohmann::json::parse(read_string()));
	return b;
}
ABTDataStub cw::BinarySerializer::Reader::read_bson() {
	ABTDataStub b;
	b.update(nlohmann::json::from_bson(read_string()));
	return b;
}
ABTDataStub cw::BinarySerializer::Reader::read_compound() {
	ABTDataStub b;
	b.update(nlohmann::json::from_msgpack(read_string()));
	return b;
}
unsigned int	Reader::read_uint(){
	int* d = ((int*)read_data(sizeof(int)));
	int f = *d;
	free(d);
	return f;
}
unsigned short	Reader::read_ushort(){
	short* d = ((short*)read_data(sizeof(short)));
	short f = *d;
	free(d);
	return f;
}
uint64_t Reader::read_ulong(){
	uint64_t* d = ((uint64_t*)read_data(sizeof(uint64_t)));
	uint64_t f = *d;
	free(d);
	return f;
}
unsigned char	Reader::read_ubyte(){
	char* d = ((char*)read_data(sizeof(char)));
	char f = *d;
	free(d);
	return f;
}
std::string Reader::read_string() {
	uint32_t len = read_uint();
	return std::string(read_data(len), len);
}
Writer Writer::write_string(std::string s) {
	uint32_t len = (uint32_t)s.size();
	write_uint(len);
	write_data(s.data(), len);
	return *this;
}

signed int	Reader::read_sint() {
	int* d = ((int*)read_data(sizeof(int)));
	int f = *d;
	free(d);
	return f;
}
signed short	Reader::read_sshort() {
	short* d = ((short*)read_data(sizeof(short)));
	short f = *d;
	free(d);
	return f;
}
int64_t Reader::read_slong() {
	int64_t* d = ((int64_t*)read_data(sizeof(int64_t)));
	int64_t f = *d;
	free(d);
	return f;
}
signed char	Reader::read_sbyte() {
	char* d = ((char*)read_data(sizeof(char)));
	char f = *d;
	free(d);
	return f;
}