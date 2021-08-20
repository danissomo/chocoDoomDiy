#include "wad_reader.h"
#include <cstring>

wad_reader::wad_reader() {}

wad_reader::~wad_reader() {}

uint16_t wad_reader::read_2_bytes(const uint8_t* pWAD_data, int offset) {
	uint16_t read_val;
	std::memcpy(&read_val, (void*)(pWAD_data + offset), sizeof(read_val));
	return read_val;
}

uint32_t wad_reader::read_4_bytes(const uint8_t* pWAD_data, int offset) {
	uint32_t read_val;
	std::memcpy(&read_val, (void*)(pWAD_data + offset), sizeof(read_val));
	return read_val;
}

void wad_reader::read_header_data(const uint8_t* pWAD_data, int offset, header& head) {
	head.dir_count = read_4_bytes(pWAD_data, offset + 4);
	head.dir_offset = read_4_bytes(pWAD_data, offset + 8);

	head.WAD_type[0] = pWAD_data[offset++];
	head.WAD_type[1] = pWAD_data[offset++];
	head.WAD_type[2] = pWAD_data[offset++];
	head.WAD_type[3] = pWAD_data[offset++];
	head.WAD_type[4] = '\0';


}

void wad_reader::read_dir_data(const uint8_t* pWAD_data, int offset, dir& d) {
	d.lump_offset = read_4_bytes(pWAD_data, offset);
	d.lump_size = read_4_bytes(pWAD_data, offset + 4);
	offset += 8;
	d.lump_name[0] = pWAD_data[offset++];
	d.lump_name[1] = pWAD_data[offset++];
	d.lump_name[2] = pWAD_data[offset++];
	d.lump_name[3] = pWAD_data[offset++];
	d.lump_name[4] = pWAD_data[offset++];
	d.lump_name[5] = pWAD_data[offset++];
	d.lump_name[6] = pWAD_data[offset++];
	d.lump_name[7] = pWAD_data[offset++];
	d.lump_name[8] = '\0';


}

void wad_reader::read_vertex_data(const uint8_t* pWAD_data, int offset, vertex& vert) {
	vert.X_pos = read_2_bytes(pWAD_data, offset);
	vert.Y_pos = read_2_bytes(pWAD_data, offset + 2);
}

void wad_reader::read_linedef_data(const uint8_t* pWAD_data, int offset, linedef& ldef) {
	ldef.start_vertex = read_2_bytes(pWAD_data, offset);
	ldef.end_vertex = read_2_bytes(pWAD_data, offset + 2);
	ldef.flags = read_2_bytes(pWAD_data, offset + 4);
	ldef.line_type = read_2_bytes(pWAD_data, offset + 6);
	ldef.sector_tag = read_2_bytes(pWAD_data, offset + 8);
	ldef.right_sidedef = read_2_bytes(pWAD_data, offset + 10);
	ldef.left_sidedef = read_2_bytes(pWAD_data, offset + 12);
}
