#include <iostream>
#include "wad_loader/wad_loader.h"

int main(){
	wad_loader wad_l("DOOM.WAD");
	wad_l.load_wad();
	Map map("E1M1");
	wad_l.load_map_data(map);

	return 0;
}