#include "wad_loader.h"
#include <iostream>
#include <string.h>

wad_loader::wad_loader() :
	WAD_data(NULL){
}

void wad_loader::set_wad_path(std::string file){
	sWAD_file_path= file;
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
	dir d;

	for (uint i = 0; i < head.dir_count; i++) {
		reader.read_dir_data(WAD_data, head.dir_offset + i * 16, d);
		WAD_dirs.push_back(d);
	}
	return true;
}

bool wad_loader::load_map_data(Map *map){
	if(!read_map_vertex(map)){
		std::cout << "Error: Failed to load vertex data MAP: " << map->get_name() <<std::endl;
		return false;
	}	
	if(!read_map_linedef(map)){
		std::cout << "Error: Failed to load linedef data MAP: " << map->get_name()  << std::endl;
		return false; 
	}
	if(!read_map_thing(map)){
		std::cout << "Error: Failed to load things data MAP: " << map->get_name()  << std::endl;
		return false; 
	}
	if(!read_map_nodes(map)){
		std::cout << "Error: Failed to load nodes data MAP: " << map->get_name()  << std::endl;
		return false; 
	}
	if(!read_map_subsectors(map)){
		std::cout << "Error: Failed to load subsectors data MAP: " << map->get_name()  << std::endl;
		return false;
	}
	if(!read_map_segs(map)){
		std::cout << "Error: Failed to load segs data MAP: " << map->get_name()  << std::endl;
		return false;
	}
	return true;
}

bool wad_loader::read_map_vertex(Map *map){
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
		map->add_vertex(vert);
	} 
	return true;
}

bool wad_loader::read_map_linedef(Map *map){
	int map_index = find_map_index(map);
	if(map_index == -1) return false;

	map_index+=EMAPLUMPSINDEX::eLINEDEFS;
	if(strcmp(WAD_dirs[map_index].lump_name, "LINEDEFS")!=0) return false;

	int linedef_byte_size = sizeof(linedef);
	int linedef_count = WAD_dirs[map_index].lump_size/linedef_byte_size;

	linedef line;
	for(int i =0; i < linedef_count;i++){
		reader.read_linedef_data(WAD_data, WAD_dirs[map_index].lump_offset+i*linedef_byte_size, line);
		map->add_linedef(line);
	}
	return true;
}

int wad_loader::find_map_index(Map *map){
	if (map->GetLumpIndex() != -1) return map->GetLumpIndex();
	for(int i = 0; i < WAD_dirs.size(); i+=1){
		if(WAD_dirs[i].lump_name == map->get_name()){
			map->SetLumpIndex(i);
			return i;
		}
	}
	return -1;
}

bool wad_loader::read_map_thing(Map *map){
	int map_index = find_map_index(map);
	if (map_index == -1) return false;

	auto thing_index = map_index + EMAPLUMPSINDEX::eTHINGS;
	if(strcmp(WAD_dirs[thing_index].lump_name, "THINGS") != 0) return false;
	 
	int Thing_size_b = sizeof(Thing);
	int things_count = WAD_dirs[thing_index].lump_size/Thing_size_b;

	Thing thing;
	for(int i = 0; i < things_count; i++){
		reader.read_thing_data(WAD_data, WAD_dirs[thing_index].lump_offset +  i*Thing_size_b,thing);
		map->add_thing(thing);
	}
	return true;
}

bool wad_loader::read_map_nodes(Map *map){
	auto map_index = find_map_index(map);
	if (map_index == -1) return false;

	auto node_index = map_index + EMAPLUMPSINDEX::eNODES;

	if(strcmp(WAD_dirs[node_index].lump_name, "NODES") != 0) return false;
	int Node_size_b = sizeof(Node);
	int nodes_count = WAD_dirs[node_index].lump_size/Node_size_b;
	
	Node node;
	for(int  i=0; i < nodes_count; i++){
		reader.read_node_data(WAD_data, WAD_dirs[node_index].lump_offset + i*Node_size_b, node);
		map->add_node(node);
	}
	return true;
}

bool wad_loader::read_map_subsectors(Map *map){
	auto map_index = find_map_index(map);
	if (map_index == -1) return false;

	auto subsector_index = map_index + EMAPLUMPSINDEX::eSSECTORS;

	if(strcmp(WAD_dirs[subsector_index].lump_name, "SSECTORS") != 0) return false;
	int Subsector_size_b = sizeof(Subsector);
	int subsectors_count = WAD_dirs[subsector_index].lump_size/Subsector_size_b;

	Subsector subsector;
	for(int  i=0; i < subsectors_count; i++){
		reader.read_subsector_data(WAD_data, WAD_dirs[subsector_index].lump_offset + i*Subsector_size_b, subsector);
		map->add_subsector(subsector);
	}
	return true;
}

bool wad_loader::read_map_segs(Map *map){
	auto map_index = find_map_index(map);
	if (map_index == -1) return false;

	auto seg_index = map_index + EMAPLUMPSINDEX::eSEAGS;
	if(strcmp(WAD_dirs[seg_index].lump_name, "SEGS")) return false;
	int Seg_size_b = sizeof(Seg);
	int segs_count = WAD_dirs[seg_index].lump_size/Seg_size_b;

	Seg seg;
	for(int i = 0; i < segs_count; i++){
		reader.read_seg_data(WAD_data, WAD_dirs[seg_index].lump_offset + i*Seg_size_b, seg);
		map->add_seg(seg);
	}
	return true;
}
// template <typename T>
// bool wad_loader::read(Map*map,  void(*add)(T)){
// 	auto map_index=1;
// 	return true;
//  }