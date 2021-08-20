#pragma once

#include <cstdint>

enum EMAPLUMPSINDEX
{
    eTHINGS = 1,
    eLINEDEFS,
    eSIDEDDEFS,
    eVERTEXES,
    eSEAGS,
    eSSECTORS,
    eNODES,
    eSECTORS,
    eREJECT,
    eBLOCKMAP,
    eCOUNT
};

enum ELINEDEFFLAGS
{
    eBLOCKING = 0,
    eBLOCKMONSTERS = 1,
    eTWOSIDED = 2,
    eDONTPEGTOP = 4,
    eDONTPEGBOTTOM = 8,
    eSECRET = 16,
    eSOUNDBLOCK = 32,
    eDONTDRAW = 64,
    eDRAW = 128
};

struct header{
	char WAD_type[5];
	uint32_t dir_count;
	uint32_t dir_offset;
};

struct dir {
	uint32_t lump_offset;
	uint32_t lump_size;
	char lump_name[9];
};
struct vertex{
	int16_t X_pos;
	int16_t Y_pos;
};

struct linedef{
	uint16_t start_vertex;
	uint16_t end_vertex;
	uint16_t flags;
	uint16_t line_type;
	uint16_t sector_tag;
	uint16_t right_sidedef;
	uint16_t left_sidedef;
};
