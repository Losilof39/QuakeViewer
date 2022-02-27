#pragma once

#include <string>
#include "quakedef.h"

class PAK
{
public:
	void LoadPak(std::string filepath);
	pak_header_t GetPakHeader(void);
	uint8_t* GetPakStream(void);
	pak_file_t* GetPakFileStream(void);

private:
	FILE* m_pakfile;
	pak_header_t m_pak_hdr;
	pak_file_t* m_pak_files_stream;
	uint8_t* pak_stream;
};

