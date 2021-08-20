#include "wad_loader.h"
#include <iostream>
#include <string.h>
wad_loader::wad_loader(std::string sWAD_file) :
	WAD_data(NULL), sWAD_file_path(sWAD_file) {
}


bool wad_loader::open_load() {
	WAD_file.open(sWAD_file_path, std::ifstream::binary);
	if (!WAD_file.is_open()) {
		std::cout << "Error: Failed top open WAD file " << sWAD_file_path << std::endl;
		return false;
	}

	WAD_file.seekg(0, WAD_file.end);
	size_t length = WAD_file.tellg();
	WAD_data = new uint8_t[length];
	if (WAD_data == NULL) {
		std::cout << "Error: Failed to allocate memory for WAD data of size " << length << std::endl;
	}

	WAD_file.seekg(std::ifstream::beg);
	WAD_file.read((char*)WAD_data, length);

	WAD_file.close();

	#ifdef DEBUG_BUILD
	std::cout << "Info: Loading complete" << std::endl;
	#endif 

	return true;
}

bool wad_loader::load_wad() {
	if (!open_load()) return false;
	if (!read_dirs()) return false;
	return true;
}

wad_loader::~wad_loader() {
	delete[] WAD_data;
}

bool wad_loader::read_dirs() {
	wad_reader reader;

	header head;
	reader.read_header_data(WAD_data, 0, head);

	#ifdef DEBUG_BUILD
	std::cout << head.WAD_type << std::endl;
	std::cout << head.dir_count << std::endl;
	std::cout << head.dir_offset << std::endl;
	std::cout << std::endl << std::endl;
	#endif
	dir d;

	for (uint i = 0; i < head.dir_count; i++) {
		reader.read_dir_data(WAD_data, head.dir_offset + i * 16, d);

		WAD_dirs.push_back(d);
		#ifdef DEBUG_BUILD
		std::cout << d.lump_offset << std::endl;
		std::cout << d.lump_size << std::endl;
		std::cout << d.lump_name << std::endl;
		std::cout << std::endl << std::endl;
		#endif
	}
}

bool wad_loader::load_map_data(Map &map){
	if(!read_map_vertex(map)){
		std::cout << "Error: Failed to load vertex data MAP: " << map.get_name() <<std::endl;
		return false;
	}
	
	if(!read_map_linedef(map)){
		std::cout << "Error: Failed to load linedef data MAP: " << map.get_name()  << std::endl;
		return false; 
	}
	return true;
}

bool wad_loader::read_map_vertex(Map &map){
	int iMap_index = find_map_index(map);
	if(iMap_index == -1)
		return false;
	iMap_index += EMAPLUMPSINDEX::eVERTEXES;
	if(strcmp(WAD_dirs[iMap_index].lump_name, "VERTEXES")!=0)
		return false;
	int iVertexSizeInBytes = sizeof(vertex);
	int iVertexesCount = WAD_dirs[iMap_index].lump_size/iVertexSizeInBytes;

	vertex vert;
	for(int i = 0; i < iVertexesCount; i++){
		reader.read_vertex_data(WAD_data, WAD_dirs[iMap_index].lump_offset+i*iVertexSizeInBytes, vert );
		map.add_vertex(vert);

		#ifdef DEBUG_BUILD
		//std::cout << "("  << vert.X_pos << "," << vert.Y_pos << ")" << std::endl;
		#endif
	} 
	return true;
}

bool wad_loader::read_map_linedef(Map &map){
	int map_index = find_map_index(map);
	if(map_index == -1) return false;

	map_index+=EMAPLUMPSINDEX::eLINEDEFS;
	if(strcmp(WAD_dirs[map_index].lump_name, "LINEDEFS")!=0) return false;

	int linedef_byte_size = sizeof(linedef);
	int linedef_count = WAD_dirs[map_index].lump_size/linedef_byte_size;

	linedef line;
	for(int i =0; i < linedef_count;i++){
		reader.read_linedef_data(WAD_data, WAD_dirs[map_index].lump_offset+i*linedef_byte_size, line);
		map.add_linedef(line);
		#ifdef DEBUG_BUILD
		auto x1 = map.get_vert(line.start_vertex).X_pos, x2 = map.get_vert(line.end_vertex).X_pos;
		auto y1 = map.get_vert(line.start_vertex).Y_pos, y2 = map.get_vert(line.end_vertex).Y_pos;
		
		//std::printf("\\operatorname{polygon}((%d,%d),(%d,%d))\n", x1, y1, x2,y2);
			
			
		// std::cout << line.start_vertex << std::endl;
		// std::cout << line.end_vertex << std::endl;
		// std::cout << line.flags << std::endl;
		// std::cout << line.line_type << std::endl;
		// std::cout << line.sector_tag << std::endl;
		// std::cout << line.right_sidedef << std::endl;
		// std::cout << line.left_sidedef << std::endl;
		#endif
	}
	return true;


}

int wad_loader::find_map_index(Map &map){
	for(int i = 0; i < WAD_dirs.size(); i++){
		if(WAD_dirs[i].lump_name == map.get_name()){
			return i;
		}
	}
	return -1;
}
