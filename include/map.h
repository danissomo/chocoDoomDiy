#pragma once
#include "data_types.h"
#include "Player.h"


#include <string>
#include <vector>
#include <stdexcept>
#include <SDL2/SDL.h>

struct ViewRendererDataWall{
	Seg seg;
	Angle V1Angle;
	Angle V2Angle;
	Angle v1AngleFpl;
	Angle V2AngleFpl;
};

class Map{
	public:
		Map(std::string s_name, Player *pPlayer);
		~Map();
		void Init();
		std::string get_name();

	
		void add_vertex(Vertex &v);
		void add_linedef(WADlinedef &l);
		void add_node(Node &n);
		void add_subsector(Subsector &s);
		void add_seg(WADSeg &s);
		void add_sidedef(WADSidedef &s);
		void add_sector(WADSector &s);
		void add_thing(Thing &t);
		
		
		Vertex& get_vert(int i);
		std::vector<std::pair<Vertex, Vertex>> GetLvlWalls();

	
		void RenderBSPNodes(std::vector<ViewRendererDataWall> &dataWallsInFOV);

		int GetLumpIndex();
		void SetLumpIndex(int);

		int GetXmax(){return xMax;}
		int GetXmin(){return xMin;}
		int GetYmax(){return yMax;}
		int GetYmin(){return yMin;}
		int GetSectorFloorHeight();

		
		
	protected:
		std::string map_name;
		std::vector<Vertex> m_verexes;
		std::vector<Thing> m_things;
		std::vector<Node> m_Nodes;
		std::vector<Subsector> m_Subsectors;
		std::vector<Sector> m_Sectors;
		std::vector<Seg> m_Segs;
		std::vector<Linedef> m_Linedefs;
		std::vector<Sidedef> m_Sidedefs;

		std::vector<WADSeg> m_wad_Segs;
		std::vector<WADlinedef> m_wad_linedefs;
		std::vector<WADSector> m_wad_Sectors;
		std::vector<WADSidedef> m_wad_Sidedef;

		void RenderAutoMapWalls();
		void RenderAutoMapNode(int NodeID);
		void RenderBSPNodes(int NodeID,  std::vector<ViewRendererDataWall> &dataWallsInFOV);
		void RenderSubsector(int subID,  std::vector<ViewRendererDataWall> &dataWallsInFOV);


		bool IsPointOnLeftNodeSide(int X, int Y, int NodeId);

		int xMin;
		int xMax;
		int yMin;
		int yMax;

		int autoScaleFactor;  

		int lumpIndex;

		Player *pPlayer;

		void SetPlayer(Thing &);


		void BuildSectors();
		void BuildSidedef();
		void BuildLinedef();
		void BuildSeg();
	
};