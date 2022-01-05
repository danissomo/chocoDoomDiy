#pragma once
#include "data_types.h"
#include "Player.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <SDL2/SDL.h>
class Map{
	public:
		Map(SDL_Renderer* pRenderer, std::string s_name, Player *pPlayer);
		~Map();

		std::string get_name();
		void add_vertex(vertex &v);
		void add_linedef(linedef &l);
		void add_thing(Thing &t);
		void add_node(Node &n);

		vertex& get_vert(int i);

		void RenderAutoMap();
		void RenderAutoMapPlayer();
		void RenderAutoMapWalls();
		void RenderAutoMapNode();
		int GetLumpIndex();
		void SetLumpIndex(int);
		//desmos visualisation
		std::string to_desmos();
	protected:
		std::string map_name;
		std::vector<vertex> m_verexes;
		std::vector<linedef> m_linedefs;
		std::vector<Thing> m_things;
		std::vector<Node> m_Nodes;
		
		int ScaleTranslate(int offset, int scale, int x_max, float param_cord);
		int RemapX(int x, int offset);
		int RemapY(int y, int offset);
		int xMin;
		int xMax;
	
		int yMin;
		int yMax;
		int autoScaleFactor;  

		int lumpIndex;

		Player *pPlayer;

		void SetPlayer(Thing &);

		SDL_Renderer *m_pRenderer;
};