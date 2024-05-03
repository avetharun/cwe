#pragma once
#include <string>
#include <vector>
#include "cwabt.h"

namespace cw {
		/**
		* Formats: 
		* String: 
		-	uint16 : length
		-	char*  : string
		*/
		namespace BinarySerializer {
			struct Writer;
			struct Reader;
			struct Reader {
				std::vector<char> buffer = std::vector<char>();
				int ptr = 0;
				long			get_read_ptr();
				std::string		read_string();
				signed   int	read_sint();
				signed   short	read_sshort();
				int64_t			read_slong();
				signed   char	read_sbyte();
				unsigned int	read_uint();
				unsigned short	read_ushort();
				uint64_t		read_ulong();
				unsigned char	read_ubyte();
				float			read_float();
				char*			read_data(size_t length);
				ABTDataStub		read_json();
				ABTDataStub		read_bson();
				ABTDataStub  read_compound();
				template <typename T> T read_class() { return *(T*)read_data(sizeof(T)); }
				Reader(std::vector<char> buffer);
				Reader(char* buffer, size_t length);
				Writer to_writer() const;
			};
			struct Writer {
				std::vector<char> buffer = std::vector<char>();
				int ptr = 0;
				long get_write_ptr();
				Writer write_data(char* data, size_t length);
				Writer write_string(std::string str);
				Writer write_sint(signed int i);
				Writer write_sshort(signed short i);
				Writer write_slong(int64_t i);
				Writer write_sbyte(signed char i);
				Writer write_json(		ABTDataStub json);
				Writer write_bson(		ABTDataStub bson);
				Writer write_compound(	ABTDataStub msgpk);
				template <typename T> Writer write_class(T data) { return this->write_data((char*) & data, sizeof(T)); }


				Writer write_uint(unsigned int i);
				Writer write_ushort(unsigned short i);
				Writer write_ulong(uint64_t i);
				Writer write_ubyte(unsigned char i);
				Reader to_reader() const;
				Writer write_float(float i);
			};
		};
}