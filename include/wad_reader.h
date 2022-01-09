#include <cstdint>
#include "data_types.h"
#include "map.h"

class wad_reader{

	public:
		wad_reader();
		~wad_reader();

		void read_header_data(const uint8_t* pWAD_data, int offset, header &head );
		void read_dir_data(const uint8_t* pWAD_data, int offset, dir &d);
		void read_vertex_data(const uint8_t *pWAD_data, int offset, Vertex &vert);
		void read_thing_data(const uint8_t *pWAD_data, int offset, Thing &thing);
		void read_node_data(const uint8_t *pWAD_data, int offset, Node&node);
		void read_subsector_data(const uint8_t *pWAD_data, int offset, Subsector &subsector);
		void read_sidedef_data(const uint8_t *pWAD_data, int offset, WADSidedef &side);
		void read_seg_data(const uint8_t *pWAD_data, int offset, WADSeg &seg);
		void read_sector_data(const uint8_t *pWAD_data, int offset, WADSector &sector);
		void read_linedef_data(const uint8_t *pWAD_data, int offset, WADlinedef &ldef);
		

	protected:
	 	uint16_t read_2_bytes(const uint8_t* pWAD_data, int offset);
		uint32_t read_4_bytes(const uint8_t* pWAD_data, int offset);

};