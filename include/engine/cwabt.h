#pragma once
#include <alib.h>
#include <string>
#include <nlohmann/json/json.hpp>
using json = nlohmann::json;
struct ABTDataStub : public nlohmann::json {

	std::string get_string(std::string key);
	int get_int(std::string key);
	float get_float(std::string key);
	bool get_bool(std::string key);
	ABTDataStub get_compound(std::string key);
	std::string get_or_default_string(std::string key, std::string m_default);
	int get_or_default_int(std::string key, int m_default);
	float get_or_default_float(std::string key, float m_default);
	bool get_or_default_bool(std::string key, bool m_default);
	ABTDataStub get_or_default_compound(std::string key, ABTDataStub m_default);
	// Not very efficient! Call once per init please!
	std::vector<std::string> get_keys();
	bool m_contains(std::string key);
	void set_float(std::string k, float v);
	void set_int(std::string k, int v);
	void set_string(std::string k, std::string v);
	void set_bool(std::string k, bool v);
	void set_compound(std::string k, ABTDataStub v);
	template <typename T>
	void set(std::string k, T v) {
		(*this)[k] = v;
	}

	void add_float(std::string k);
	void add_int(std::string k);
	void add_string(std::string k);
	void add_bool(std::string k);
	void add_compound(std::string k);

};

template <typename V>
void nlohmann_json_setv(nlohmann::json* json, std::string key, V value) {
	(*json)[key] = value;
}

template <const int ABT_TYPE = 1>
struct ABT_RW {
	ABT_RW(ABTDataStub dataStub) {
		data = dataStub;
	}
	ABTDataStub data;
	void invalidate() {
		if (data.type() == json::value_t::null) { return; }
		data.clear();
	}
	void decompile(const char* filen) {
		if (ABT_TYPE == 1) {
			data.clear();
			data.update(nlohmann::json(alib_file_read(filen).c_str()));
		}
		else {
			data.clear();
			data.update(ABTDataStub::from_bson(alib_file_read(filen)));
		}
	}
	void compile(const char* filen) {
		alib_file_write(filen, getbuffer());
	}
	std::string getbuffer() {

		if (ABT_TYPE == 1) {
			return data.dump();
		}
		else {
			auto j = nlohmann::json::to_bson(data);
			return std::string((char*)j.data(), j.size());
		}
	}
};
