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

void wad_reader::read_vertex_data(const uint8_t* pWAD_data, int offset, Vertex& vert) {
	vert.X_pos = read_2_bytes(pWAD_data, offset);
	vert.Y_pos = read_2_bytes(pWAD_data, offset + 2);
}

void wad_reader::read_linedef_data(const uint8_t* pWAD_data, int offset, WADlinedef& ldef) {
	ldef.start_vertex 		= read_2_bytes(pWAD_data, offset);
	ldef.end_vertex			= read_2_bytes(pWAD_data, offset + 2);
	ldef.flags 				= read_2_bytes(pWAD_data, offset + 4);
	ldef.line_type 			= read_2_bytes(pWAD_data, offset + 6);
	ldef.sector_tag 		= read_2_bytes(pWAD_data, offset + 8);
	ldef.right_sidedef 		= read_2_bytes(pWAD_data, offset + 10);
	ldef.left_sidedef 		= read_2_bytes(pWAD_data, offset + 12);
}

void wad_reader::read_thing_data(const uint8_t *pWAD_data, int offset, Thing &thing){
	thing.XPosition 		= read_2_bytes(pWAD_data, offset);
    thing.YPosition 		= read_2_bytes(pWAD_data, offset + 2);
    thing.Angle 			= read_2_bytes(pWAD_data, offset + 4);
    thing.Type 				= read_2_bytes(pWAD_data, offset + 6);
    thing.Flags 			= read_2_bytes(pWAD_data, offset + 8);
}

void wad_reader::read_node_data(const uint8_t *pWAD_data, int offset, Node &node){
	node.XPartition 		= read_2_bytes(pWAD_data, offset);
    node.YPartition 		= read_2_bytes(pWAD_data, offset + 2);
    node.ChangeXPartition 	= read_2_bytes(pWAD_data, offset + 4);
    node.ChangeYPartition 	= read_2_bytes(pWAD_data, offset + 6);

    node.RightBoxTop 		= read_2_bytes(pWAD_data, offset + 8);
    node.RightBoxBottom 	= read_2_bytes(pWAD_data, offset + 10);
    node.RightBoxLeft 		= read_2_bytes(pWAD_data, offset + 12);
    node.RightBoxRight 		= read_2_bytes(pWAD_data, offset + 14);

    node.LeftBoxTop 		= read_2_bytes(pWAD_data, offset + 16);
    node.LeftBoxBottom 		= read_2_bytes(pWAD_data, offset + 18);
    node.LeftBoxLeft 		= read_2_bytes(pWAD_data, offset + 20);
    node.LeftBoxRight 		= read_2_bytes(pWAD_data, offset + 22);

    node.RightChildID 		= read_2_bytes(pWAD_data, offset + 24);
    node.LeftChildID 		= read_2_bytes(pWAD_data, offset + 26);
}

void wad_reader::read_subsector_data(const uint8_t *pWAD_data, int offset, Subsector &subsector){
	subsector.SegCount 		= read_2_bytes(pWAD_data, offset);
    subsector.FirstSegID 	= read_2_bytes(pWAD_data, offset + 2);
}

void wad_reader::read_seg_data(const uint8_t *pWAD_data, int offset, WADSeg &seg){
	seg.StartVertexID 		= read_2_bytes(pWAD_data, offset);
    seg.EndVertexID 		= read_2_bytes(pWAD_data, offset + 2);
    seg.Angle 				= read_2_bytes(pWAD_data, offset + 4);
    seg.LinedefID 			= read_2_bytes(pWAD_data, offset + 6);
    seg.Direction 			= read_2_bytes(pWAD_data, offset + 8);
    seg.Offset 				= read_2_bytes(pWAD_data, offset + 10);
}

void wad_reader::read_sidedef_data(const uint8_t *pWAD_data, int offset, WADSidedef &side){
	side.XOffset 			= read_2_bytes(pWAD_data, offset);
    side.YOffset 			= read_2_bytes(pWAD_data, offset + 2);
    side.UpperTexture[0] 	= pWAD_data[offset +  4];
    side.UpperTexture[1] 	= pWAD_data[offset +  5];
    side.UpperTexture[2] 	= pWAD_data[offset +  6];
    side.UpperTexture[3] 	= pWAD_data[offset +  7];
    side.UpperTexture[4] 	= pWAD_data[offset +  8];
    side.UpperTexture[5] 	= pWAD_data[offset +  9];
    side.UpperTexture[6] 	= pWAD_data[offset + 10];
    side.UpperTexture[7] 	= pWAD_data[offset + 11];
	
    side.LowerTexture[0] 	= pWAD_data[offset + 12];
    side.LowerTexture[1] 	= pWAD_data[offset + 13];
    side.LowerTexture[2] 	= pWAD_data[offset + 14];
    side.LowerTexture[3] 	= pWAD_data[offset + 15];
    side.LowerTexture[4] 	= pWAD_data[offset + 16];
    side.LowerTexture[5] 	= pWAD_data[offset + 17];
    side.LowerTexture[6] 	= pWAD_data[offset + 18];
    side.LowerTexture[7] 	= pWAD_data[offset + 19];

    side.MiddleTexture[0] 	= pWAD_data[offset + 20];
    side.MiddleTexture[1] 	= pWAD_data[offset + 21];
    side.MiddleTexture[2] 	= pWAD_data[offset + 22];
    side.MiddleTexture[3] 	= pWAD_data[offset + 23];
    side.MiddleTexture[4] 	= pWAD_data[offset + 24];
    side.MiddleTexture[5] 	= pWAD_data[offset + 25];
    side.MiddleTexture[6] 	= pWAD_data[offset + 26];
    side.MiddleTexture[7] 	= pWAD_data[offset + 27];

    side.SectorID 			= read_2_bytes(pWAD_data, offset + 28);
}

void wad_reader::read_sector_data(const uint8_t *pWAD_data, int offset, WADSector &sector){
	sector.FloorHeight 			= read_2_bytes(pWAD_data, offset);
    sector.CeilingHeight 		= read_2_bytes(pWAD_data, offset + 2);

    sector.FloorTexture[0] 		= pWAD_data[offset + 4];
    sector.FloorTexture[1] 		= pWAD_data[offset + 5];
    sector.FloorTexture[2] 		= pWAD_data[offset + 6];
    sector.FloorTexture[3] 		= pWAD_data[offset + 7];
    sector.FloorTexture[4] 		= pWAD_data[offset + 8];
    sector.FloorTexture[5] 		= pWAD_data[offset + 9];
    sector.FloorTexture[6] 		= pWAD_data[offset + 10];
    sector.FloorTexture[7] 		= pWAD_data[offset + 11];

    sector.CeilingTexture[0] 	= pWAD_data[offset + 12];
    sector.CeilingTexture[1] 	= pWAD_data[offset + 13];
    sector.CeilingTexture[2] 	= pWAD_data[offset + 14];
    sector.CeilingTexture[3] 	= pWAD_data[offset + 15];
    sector.CeilingTexture[4] 	= pWAD_data[offset + 16];
    sector.CeilingTexture[5] 	= pWAD_data[offset + 17];
    sector.CeilingTexture[6] 	= pWAD_data[offset + 18];
    sector.CeilingTexture[7] 	= pWAD_data[offset + 19];

    sector.Lightlevel 			= read_2_bytes(pWAD_data, offset + 20);
    sector.Type 				= read_2_bytes(pWAD_data, offset + 22);
    sector.Tag					= read_2_bytes(pWAD_data, offset + 24);
}


void wad_reader::read_palette(const uint8_t *pWAD_data, int offset, WADPalette &palette){
    for(int i =0; i<256; i++){
        palette.colors[i].r = pWAD_data[offset++];
        palette.colors[i].g = pWAD_data[offset++];
        palette.colors[i].b = pWAD_data[offset++];
        palette.colors[i].a = 255;
    }
}


void wad_reader::read_patch_header(const uint8_t* pWAD_data, int offset, WADPatchHeader &patchHeader){
    patchHeader.width           = read_2_bytes(pWAD_data, offset);
    patchHeader.height          = read_2_bytes(pWAD_data, offset + 2);
    patchHeader.leftOffset      = read_2_bytes(pWAD_data, offset + 4);
    patchHeader.topOffset       = read_2_bytes(pWAD_data, offset + 6);
    patchHeader.columnOffset    = new uint32_t[patchHeader.width];

    offset+=8;
    for (int i = 0; i <patchHeader.width; i++){
        patchHeader.columnOffset[i] = read_4_bytes(pWAD_data, offset);
        offset+=4;
    }
}


int wad_reader::read_patch_column(const uint8_t* pWAD_data, int offset, WADPatchColumn &patchColumn){
    patchColumn.topDelta = pWAD_data[offset++];
    int idata = 0;
    if(patchColumn.topDelta != 0xFF){
        patchColumn.len         = pWAD_data[offset++];
        patchColumn.paddingPre  = pWAD_data[offset++];
        patchColumn.pColumnData = new uint8_t[patchColumn.len];
        for(int i =0; i < patchColumn.len; i++, offset++)
            patchColumn.pColumnData[i] = pWAD_data[offset];
        
        patchColumn.paddingPost = pWAD_data[offset++];
    }
    return offset;
}


void wad_reader::read_pname(const uint8_t* pWAD_data, int offset, WADPNames &pNames){
    pNames.PNameCount = read_4_bytes(pWAD_data, offset);
    pNames.PNameOffset = offset+4;
}


void wad_reader::read_texture_header(const uint8_t* pWAD_data, int offset, WADTextureHeader &textureHeader){
    textureHeader.TexturesCount         = read_4_bytes(pWAD_data, offset);
    textureHeader.TexturesOffset        = read_4_bytes(pWAD_data, offset + 4);
    textureHeader.pTexturesDataOffset   = new uint32_t[textureHeader.TexturesCount];
    offset+=4;
    for(int i = 0; i < textureHeader.TexturesCount; i++){
        textureHeader.pTexturesDataOffset[i] = read_4_bytes(pWAD_data, offset);
        offset += 4;
    }
}


void wad_reader::read_texture_data(const uint8_t* pWAD_data, int offset, WADTextureData &textureData){
    textureData.TextureName[0]  = pWAD_data[offset + 0];
    textureData.TextureName[1]  = pWAD_data[offset + 1];
    textureData.TextureName[2]  = pWAD_data[offset + 2];
    textureData.TextureName[3]  = pWAD_data[offset + 3];
    textureData.TextureName[4]  = pWAD_data[offset + 4];
    textureData.TextureName[5]  = pWAD_data[offset + 5];
    textureData.TextureName[6]  = pWAD_data[offset + 6];
    textureData.TextureName[7]  = pWAD_data[offset + 7];
    textureData.TextureName[8]  = '\0';

    textureData.Flags           = read_4_bytes(pWAD_data, offset +  8);
    textureData.Width           = read_2_bytes(pWAD_data, offset + 12);
    textureData.Height          = read_2_bytes(pWAD_data, offset + 14);
    textureData.ColumnDirectory = read_4_bytes(pWAD_data, offset + 16);
    textureData.PatchCount      = read_2_bytes(pWAD_data, offset + 20);
    textureData.pTexturePatch   = new WADTexturePatch[textureData.PatchCount];

    offset += 22;
    for(int i = 0; i < textureData.PatchCount; i++){
        read_texture_patch(pWAD_data, offset, textureData.pTexturePatch[i]);
        offset += 10;
    }

}


void wad_reader::read_texture_patch(const uint8_t* pWAD_data, int offset, WADTexturePatch &texturePatch){
    texturePatch.XOffset    = read_2_bytes(pWAD_data, offset + 0);
    texturePatch.YOffset    = read_2_bytes(pWAD_data, offset + 2);
    texturePatch.PNameIndex = read_2_bytes(pWAD_data, offset + 4);
    texturePatch.StepDir    = read_2_bytes(pWAD_data, offset + 6);
    texturePatch.ColorMap   = read_2_bytes(pWAD_data, offset + 8);
}


void wad_reader::read_8_chars(const uint8_t* pWAD_data, int offset, char *pName){
    pName[0] = pWAD_data[offset++];
    pName[1] = pWAD_data[offset++];
    pName[2] = pWAD_data[offset++];
    pName[3] = pWAD_data[offset++];
    pName[4] = pWAD_data[offset++];
    pName[5] = pWAD_data[offset++];
    pName[6] = pWAD_data[offset++];
    pName[7] = pWAD_data[offset++];
}

