#include <cstdint>
#include "data_types.h"
#include "map.h"

class wad_reader{

	public:
		wad_reader();
		~wad_reader();

		void read_header_data			(const uint8_t* pWAD_data, int offset, header &head );
		void read_dir_data				(const uint8_t* pWAD_data, int offset, dir &d);
		void read_vertex_data			(const uint8_t *pWAD_data, int offset, Vertex &vert);
		void read_thing_data			(const uint8_t *pWAD_data, int offset, Thing &thing);
		void read_node_data				(const uint8_t *pWAD_data, int offset, Node&node);
		void read_subsector_data		(const uint8_t *pWAD_data, int offset, Subsector &subsector);
		void read_sidedef_data			(const uint8_t *pWAD_data, int offset, WADSidedef &side);
		void read_seg_data				(const uint8_t *pWAD_data, int offset, WADSeg &seg);
		void read_sector_data			(const uint8_t *pWAD_data, int offset, WADSector &sector);
		void read_linedef_data			(const uint8_t *pWAD_data, int offset, WADlinedef &ldef);
		void read_palette				(const uint8_t *pWAD_data, int offset, WADPalette &palette);
		void read_patch_header			(const uint8_t* pWAD_data, int offset, WADPatchHeader &patchHeader);
		void read_pname					(const uint8_t* pWAD_data, int offset, WADPNames &pNames);
		void read_texture_header		(const uint8_t* pWAD_data, int offset, WADTextureHeader &textureHeader);
		void read_texture_data			(const uint8_t* pWAD_data, int offset, WADTextureData &textureData);
		void read_texture_patch			(const uint8_t* pWAD_data, int offset, WADTexturePatch &texturePatch);
		void read_8_chars				(const uint8_t* pWAD_data, int offset, char *pName);

		int read_patch_column			(const uint8_t* pWAD_data, int offset, WADPatchColumn &patchColumn);

	protected:
	 	uint16_t read_2_bytes(const uint8_t* pWAD_data, int offset);
		uint32_t read_4_bytes(const uint8_t* pWAD_data, int offset);

};