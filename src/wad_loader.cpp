#include "wad_loader.h"
#include "AssetsManager.h"
#include "DisplayManager.h"

#include <string.h>
#include <iostream>

const char *wad_loader::LUMPNAMES[] = {
    "THINGS",     // eTHINGS,
    "LINEDEFS",   // eLINEDEFS,
    "SIDEDEFS",  // eSIDEDDEFS,
    "VERTEXES",   // eVERTEXES,
    "SEGS",       // eSEAGS,
    "SSECTORS",   // eSSECTORS,
    "NODES",      // eNODES,
    "SECTORS",    // eSECTORS,
    "REJECT",     // eREJECT,
    "BLOCKMAP"    // eBLOCKMAP,
};
wad_loader::wad_loader() : WAD_data(NULL) {}

void wad_loader::set_wad_path(std::string file) { sWAD_file_path = file; }

bool wad_loader::open_load() {
  WAD_file.open(sWAD_file_path, std::ifstream::binary);
  if (!WAD_file.is_open()) {
    std::cout << "Error: Failed top open WAD file " << sWAD_file_path
              << std::endl;
    return false;
  }

  WAD_file.seekg(0, WAD_file.end);
  size_t length = WAD_file.tellg();
  WAD_data = new uint8_t[length];
  if (WAD_data == NULL) {
    std::cout << "Error: Failed to allocate memory for WAD data of size "
              << length << std::endl;
  }

  WAD_file.seekg(std::ifstream::beg);
  WAD_file.read((char *)WAD_data, length);

  WAD_file.close();


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

int wad_loader::find_map_index(Map *map) {
  if (map->GetLumpIndex() != -1) return map->GetLumpIndex();
  for (int i = 0; i < WAD_dirs.size(); i += 1) {
    if (WAD_dirs[i].lump_name == map->get_name()) {
      map->SetLumpIndex(i);
      return i;
    }
  }
  return -1;
}

int wad_loader::find_lump_index(std::string s){
  for (int i = 0; i < WAD_dirs.size(); i++){ 
    if (strcmp(WAD_dirs[i].lump_name, s.c_str()) == 0) 
      return i;
  }
  return -1;
}

bool wad_loader::load_map_data(Map *map) {
  if (!read_map_vertex(map)) {
    std::cout << "Error: Failed to load vertex data MAP: " << map->get_name()
              << std::endl;
    return false;
  }
  if (!read_map_linedef(map)) {
    std::cout << "Error: Failed to load linedef data MAP: " << map->get_name()
              << std::endl;
    return false;
  }
  if (!read_map_thing(map)) {
    std::cout << "Error: Failed to load things data MAP: " << map->get_name()
              << std::endl;
    return false;
  }
  if (!read_map_nodes(map)) {
    std::cout << "Error: Failed to load nodes data MAP: " << map->get_name()
              << std::endl;
    return false;
  }
  if (!read_map_subsectors(map)) {
    std::cout << "Error: Failed to load subsectors data MAP: "
              << map->get_name() << std::endl;
    return false;
  }
  if (!read_map_segs(map)) {
    std::cout << "Error: Failed to load segs data MAP: " << map->get_name()
              << std::endl;
    return false;
  }
  if (!read_map_sector(map)) {
    std::cout << "Error: Failed to load sector data MAP: " << map->get_name()
              << std::endl;
    return false;
  }
  if (!read_map_sidedef(map)) {
    std::cout << "Error: Failed to load sidef data MAP: " << map->get_name()
              << std::endl;
    return false;
  }

  return true;
}

bool wad_loader::load_palette(){
  std::cout << "Info: Loading palette" << std::endl;
  int iPlayPal = find_lump_index("PLAYPAL");

  if(iPlayPal == -1) return -1;

  if(strcmp(WAD_dirs[iPlayPal].lump_name, "PLAYPAL") != 0)  return false;
  WADPalette palette;
  for(int i =0; i< 14; i++){
    reader.read_palette(WAD_data, WAD_dirs[iPlayPal].lump_offset + (i*3*256), palette);
    DisplayManager::GetInstance()->AddColorPalette(palette);
  }
  return true;
}


bool wad_loader::load_patch(std::string &patchName){
  AssetsManager *assetsManager = AssetsManager::GetInstance();

  int ipatch = find_lump_index(patchName);
  if(ipatch == -1) return false;
  if(strcmp(WAD_dirs[ipatch].lump_name, patchName.c_str())!=0)
    return false;

  WADPatchHeader patchHeader;
  reader.read_patch_header(WAD_data, WAD_dirs[ipatch].lump_offset, patchHeader);

  Patch *patch = assetsManager->AddPatch(patchName, patchHeader);

  WADPatchColumn patchColumn; 
  for (int i = 0; i< patchHeader.width; i++){
    int offset = WAD_dirs[ipatch].lump_offset + patchHeader.columnOffset[i];
    patch->AppendColumnStartIndex();
    do{
      offset = reader.read_patch_column(WAD_data, offset, patchColumn);
      patch->AddPatchCol(patchColumn);
    }while(patchColumn.topDelta != 0xFF);
  }
  return true;
}

bool wad_loader::load_texture(std::string textureName){
  auto assetInst = AssetsManager::GetInstance();
  int itexture = find_lump_index(textureName);

  if (itexture < 0) return false;
  if(strcmp(WAD_dirs[itexture].lump_name, textureName.c_str()) !=0) return false;

  WADTextureHeader textureHeader;
  reader.read_texture_header(WAD_data, WAD_dirs[itexture].lump_offset, textureHeader);

  WADTextureData textureData;
  for(int i = 0; i < textureHeader.TexturesCount; i++){
    reader.read_texture_data(WAD_data, WAD_dirs[itexture].lump_offset + textureHeader.pTexturesDataOffset[i], textureData);
    assetInst->AddTexture(textureData);
    delete[] textureData.pTexturePatch;
    textureData.pTexturePatch = nullptr;
  }
  
  delete[] textureHeader.pTexturesDataOffset;
  textureHeader.pTexturesDataOffset = nullptr;
  return true;
}


bool wad_loader::load_pnames(){
  auto assetInst = AssetsManager::GetInstance();
  int iPName = find_lump_index("PNAMES");
  if(iPName == -1) return false;
   
  if(strcmp(WAD_dirs[iPName].lump_name, "PNAMES") != 0) return false;

  WADPNames pnames;
  reader.read_pname(WAD_data, WAD_dirs[iPName].lump_offset, pnames);
  char name[9];
  name[8] = '\0';
  for(int i = 0; i< pnames.PNameCount; i++){
    reader.read_8_chars(WAD_data, pnames.PNameOffset, name);
    std::string buff = name;
    assetInst->AddPName(buff);
    pnames.PNameOffset+=8;
  }
  return true;
}

template <typename T, void (wad_reader::*read_)(const uint8_t *, int, T &),
          void (Map::*push)(T &), EMAPLUMPSINDEX eLUMP>
bool wad_loader::read(Map *map) {
  auto map_index = find_map_index(map);
  if (map_index == -1) return false;

  auto elem_index = map_index + eLUMP;

  if (strcmp(WAD_dirs[elem_index].lump_name, wad_loader::LUMPNAMES[eLUMP - 1]))
    return false;
  int elem_size_b = sizeof(T);
  int elem_count = WAD_dirs[elem_index].lump_size / elem_size_b;
  T elem;
  for (int i = 0; i < elem_count; i++) {
    (reader.*read_)(WAD_data, WAD_dirs[elem_index].lump_offset + i * elem_size_b, elem);
    (map->*push)(elem);
  }
  return true;
}

bool wad_loader::read_map_vertex(Map *map) {
  return read<	Vertex, 
  				&wad_reader::read_vertex_data, 
				&Map::add_vertex,
             	EMAPLUMPSINDEX::eVERTEXES>(map);
}

bool wad_loader::read_map_linedef(Map *map) {
  return read<	WADlinedef, 
  				&wad_reader::read_linedef_data, 
				&Map::add_linedef,
              	EMAPLUMPSINDEX::eLINEDEFS>(map);
}

bool wad_loader::read_map_thing(Map *map) {
  return read<	Thing, 
  				&wad_reader::read_thing_data, 
				&Map::add_thing,
              	EMAPLUMPSINDEX::eTHINGS >(map);
}

bool wad_loader::read_map_nodes(Map *map) {
  return read<	Node, 
  				&wad_reader::read_node_data, 
				&Map::add_node,
              	EMAPLUMPSINDEX::eNODES >(map);
}

bool wad_loader::read_map_subsectors(Map *map) {
  return read<	Subsector, 
  				&wad_reader::read_subsector_data, 
				&Map::add_subsector,
              	EMAPLUMPSINDEX::eSSECTORS >(map);
}

bool wad_loader::read_map_segs(Map *map) {
  return read<	WADSeg, 
  				&wad_reader::read_seg_data, 
				&Map::add_seg,
              	EMAPLUMPSINDEX::eSEAGS >(map);
}

bool wad_loader::read_map_sector(Map *map){
  return read<	WADSector, 
  				&wad_reader::read_sector_data, 
				&Map::add_sector,
              	EMAPLUMPSINDEX::eSECTORS >(map);
}

bool wad_loader::read_map_sidedef(Map *map){
  return read<	WADSidedef, 
  				&wad_reader::read_sidedef_data, 
				&Map::add_sidedef,
              	EMAPLUMPSINDEX::eSIDEDDEFS >(map);
}
