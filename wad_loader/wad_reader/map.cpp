#include "map.h"

std::string Map::get_name(){
	return map_name;
}

Map::Map(std::string s_name): map_name(s_name){
	 
}

Map::~Map(){}

void Map::add_vertex(vertex &v){
	m_verexes.push_back(v);
}

void Map::add_linedef(linedef &l){
	m_linedefs.push_back(l);
}