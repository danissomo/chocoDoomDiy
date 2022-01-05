#include <fstream>
#include <vector>
#include <string>

#include "data_types.h"
#include "wad_reader.h"
class wad_loader{
	public:
		wad_loader();
		void set_wad_path(std::string);
		bool load_wad();
		bool load_map_data(Map *map);
		
		~wad_loader();

	protected:
		bool open_load();
		bool read_dirs();
		bool read_map_vertex(Map *map);
		bool read_map_linedef(Map *map);
		bool read_map_thing(Map *map);
		bool read_map_nodes(Map *map);

		int find_map_index(Map *map);

		std::string sWAD_file_path;
		std::ifstream WAD_file;
		uint8_t *WAD_data;
		std::vector<dir> WAD_dirs;
		wad_reader reader;
};