#include "PAK.h"

void PAK::LoadPak(std::string filepath)
{
	m_pakfile = fopen(filepath.c_str(), "r");

	if (m_pakfile == NULL)
	{
		printf("PAK file not found");
		exit(-1);
	}

	// get file lenght
	fseek(m_pakfile, 0, SEEK_END);
	size_t lenght = ftell(m_pakfile);
	fseek(m_pakfile, 0, SEEK_SET);

	// load header
	fread(&m_pak_hdr, sizeof(m_pak_hdr), 1, m_pakfile);

	// alloc and populate all file found in pack
	int numfiles = m_pak_hdr.size / sizeof(pak_file_t);
	m_pak_files_stream = (pak_file_t*)malloc(m_pak_hdr.size);

	// load pak file in RAM
	fseek(m_pakfile, 0, SEEK_SET);
	pak_stream = (uint8_t*)malloc(lenght);
	fread(pak_stream, lenght, 1, m_pakfile);

	printf("[INFO]: PAK file loaded!\n");

	fclose(m_pakfile);

	for (int i = 0; i < numfiles; i++)
	{
		SDL_memcpy(&m_pak_files_stream[i], pak_stream + m_pak_hdr.offset + 64 * i, 64);
		//printf("[%d]\t%s\n", i, m_pak_files_stream[i].name);
	}
}

pak_header_t PAK::GetPakHeader(void)
{
	return m_pak_hdr;
}

uint8_t* PAK::GetPakStream(void)
{
	return pak_stream;
}

pak_file_t* PAK::GetPakFileStream(void)
{
	return m_pak_files_stream;
}
