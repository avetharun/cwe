#include "engine/cwabt.h"
 std::string ABTDataStub::get_string(std::string key) {
	return this->contains(key) ? this->at(key) : "nil";
}

 int ABTDataStub::get_int(std::string key) {
	return this->contains(key) ? this->at(key).get<int>() : 0;
}

 float ABTDataStub::get_float(std::string key) {
	return this->contains(key) ? this->at(key).get < float >() : 0;
}

 bool ABTDataStub::get_bool(std::string key) {
	return this->contains(key) ? this->at(key).get < bool >() : false;
}

 ABTDataStub ABTDataStub::get_compound(std::string key)
 {
	 return ABTDataStub();
 }

 std::string ABTDataStub::get_or_default_string(std::string key, std::string m_default) {
	return this->contains(key) ? this->at(key).get<std::string>() : m_default;
}

 int ABTDataStub::get_or_default_int(std::string key, int m_default) {
	return this->contains(key) ? this->at(key).get<int>() : m_default;
}

 float ABTDataStub::get_or_default_float(std::string key, float m_default) {
	return this->contains(key) ? this->at(key).get < float >() : m_default;
}

 bool ABTDataStub::get_or_default_bool(std::string key, bool m_default) {
	return this->contains(key) ? this->at(key).get < bool >() : m_default;
}

 ABTDataStub ABTDataStub::get_or_default_compound(std::string key, ABTDataStub m_default)
 {
	 return ABTDataStub();
 }

// Not very efficient! Call once per init please!

 std::vector<std::string> ABTDataStub::get_keys() {
	std::vector<std::string> out;
	for (auto it = this->begin(); it != this->end(); ++it) {
		out.push_back(it.key());
	}
	return out;
}

 bool ABTDataStub::m_contains(std::string key) {
	return this->contains(key);
}

 void ABTDataStub::set_float(std::string k, float v) {
	(*this)[k] = v;
}

 void ABTDataStub::set_int(std::string k, int v) {
	(*this)[k] = v;
}

 void ABTDataStub::set_string(std::string k, std::string v) {
	(*this)[k] = v;
}

 void ABTDataStub::set_bool(std::string k, bool v) {
	(*this)[k] = v;
}

 void ABTDataStub::set_compound(std::string k, ABTDataStub v) {
	 (*this)[k] = v;
 }

 void ABTDataStub::add_float(std::string k) {
	(*this)[k] = 0.0f;
}

 void ABTDataStub::add_int(std::string k) {
	(*this)[k] = 0;
}

 void ABTDataStub::add_string(std::string k) {
	(*this)[k] = "nil";
}

 void ABTDataStub::add_bool(std::string k) {
	(*this)[k] = false;
}

 void ABTDataStub::add_compound(std::string k) {
	 (*this)[k] = ABTDataStub();
 }

