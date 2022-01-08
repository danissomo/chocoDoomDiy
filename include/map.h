#pragma once
#include "data_types.h"
#include "Player.h"
#include "ViewRenderer.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <SDL2/SDL.h>
class Map{
	public:
		Map(ViewRenderer *pViewRenderer, std::string s_name, Player *pPlayer);
		~Map();

		std::string get_name();
		void add_vertex(vertex &v);
		void add_linedef(linedef &l);
		void add_thing(Thing &t);
		void add_node(Node &n);
		void add_subsector(Subsector &s);
		void add_seg(Seg &s);
		vertex& get_vert(int i);

		void RenderAutoMap();
		void RenderBSPNodes();

		int GetLumpIndex();
		void SetLumpIndex(int);

		int GetXmax(){return xMax;}
		int GetXmin(){return xMin;}
		int GetYmax(){return yMax;}
		int GetYmin(){return yMin;}
		//desmos visualisation
		std::string to_desmos();
	protected:
		std::string map_name;
		std::vector<vertex> m_verexes;
		std::vector<linedef> m_linedefs;
		std::vector<Thing> m_things;
		std::vector<Node> m_Nodes;
		std::vector<Subsector> m_Subsectors;
		std::vector<Seg> m_Segs;

		//void RenderAutoMapPlayer();
		void RenderAutoMapWalls();
		void RenderAutoMapNode(int NodeID);
		void RenderBSPNodes(int NodeID);
		void RenderSubsector(int subID);

		//int ScaleTranslate(int offset, int scale, int x_max, float param_cord);
		//int RemapX(int x, int offset);
		//int RemapY(int y, int offset);

		bool IsPointOnLeftNodeSide(int X, int Y, int NodeId);
		int xMin;
		int xMax;
	
		int yMin;
		int yMax;
		int autoScaleFactor;  

		int lumpIndex;

		Player *pPlayer;

		void SetPlayer(Thing &);

		ViewRenderer *m_pViewRenderer;
};