#include <cstdint>
#include "../data_types.h"
#include "map.h"

class wad_reader{

	public:
		wad_reader();
		~wad_reader();

		void read_header_data(const uint8_t* pWAD_data, int offset, header &head );
		void read_dir_data(const uint8_t* pWAD_data, int offset, dir &d);
		void read_vertex_data(const uint8_t *pWAD_data, int offset, vertex &vert);
		void read_linedef_data(const uint8_t *pWAD_data, int offset, linedef &ldef);
	protected:
	 	uint16_t read_2_bytes(const uint8_t* pWAD_data, int offset);
		uint32_t read_4_bytes(const uint8_t* pWAD_data, int offset);

};