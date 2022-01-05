#include "map.h"
#include <memory>
#include <iostream>
std::string Map::get_name(){
	return map_name;
}

Map::Map(SDL_Renderer *pRenderer, std::string s_name, Player *pPlayer): 
	map_name(s_name), autoScaleFactor(1), pPlayer(pPlayer), m_pRenderer(pRenderer){
		lumpIndex = -1;
	 
}

Map::~Map(){
	delete pPlayer;
}

int Map::GetLumpIndex(){
	return lumpIndex;
}

void Map::SetLumpIndex(int i){
	lumpIndex = i;
}

void Map::add_vertex(vertex &v){
	if (m_verexes.size() == 0){
		xMin = v.X_pos ;
		xMax = v.X_pos ;
		yMin = v.Y_pos;
		yMax = v.Y_pos;
	}
	m_verexes.push_back(v);

	xMax = xMax < v.X_pos ? v.X_pos : xMax;
	xMin = xMin > v.X_pos ? v.X_pos : xMin;
	
	yMax = yMax < v.Y_pos ? v.Y_pos : yMax;
	yMin = yMin > v.Y_pos ? v.Y_pos : yMin;
}

void Map::add_linedef(linedef &l){
	m_linedefs.push_back(l);
}

void Map::add_thing(Thing &t){
	if (t.Type == pPlayer->GetID()) SetPlayer(t);
	m_things.push_back(t);
}

void Map::add_node(Node &n){
	m_Nodes.push_back(n);
}

void Map::SetPlayer(Thing &t){
	pPlayer->SetX(t.XPosition);
	pPlayer->SetY(t.YPosition);
	pPlayer->SetAngle(t.Angle);
}

vertex& Map::get_vert(int i) {
			
	try{
		return m_verexes[i];
	}
	catch(const std::exception& e){
		throw e;
	}
			
}

std::string Map::to_desmos(){
	std::string s;
	for(int i =0; i < m_linedefs.size();i++){
		auto x1 = get_vert(m_linedefs[i].start_vertex).X_pos, x2 = get_vert(m_linedefs[i].end_vertex).X_pos;
		auto y1 = get_vert(m_linedefs[i].start_vertex).Y_pos, y2 = get_vert(m_linedefs[i].end_vertex).Y_pos;
		int size = std::snprintf(nullptr, 0, "\\operatorname{polygon}((%d,%d),(%d,%d))\n", x1, y1, x2,y2) + 1;
		auto buffer_string = std::make_unique<char[]>(size);
		std::sprintf(buffer_string.get(), "\\operatorname{polygon}((%d,%d),(%d,%d))\n", x1, y1, x2,y2);
		s+= buffer_string.get();
	}
	return s;
}

void Map::RenderAutoMap(){
	RenderAutoMapWalls();
	RenderAutoMapPlayer();
	RenderAutoMapNode();
}

void Map::RenderAutoMapPlayer(){
	SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	std::pair<int, int> Direction[] = {
        std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(+1, -1), // Above row
        std::make_pair(-1,  0), std::make_pair(0,  0), std::make_pair(+1,  0),    // Center Row
        std::make_pair(-1, +1), std::make_pair(0, +1), std::make_pair(+1, +1)  // Bottom Row
    };
	vertex p_scaled;
    for (int i = 0; i < 9; ++i){
        p_scaled.X_pos = RemapX(pPlayer->GetX(), Direction[i].first);
		p_scaled.Y_pos = RemapY(pPlayer->GetY(), Direction[i].second);
		SDL_RenderDrawPoint(m_pRenderer, p_scaled.X_pos, p_scaled.Y_pos);
    }
	
}

void Map::RenderAutoMapWalls(){
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for(int i = 0; i < m_linedefs.size(); i++){
		auto p1 = m_verexes[ m_linedefs[i].start_vertex], 
			 p2 = m_verexes[ m_linedefs[i].end_vertex];
		vertex p1_scaled, p2_scaled;
	

		p1_scaled.X_pos = RemapX(p1.X_pos, 0);
		p1_scaled.Y_pos = RemapY(p1.Y_pos, 0);

		p2_scaled.X_pos = RemapX(p2.X_pos, 0);
		p2_scaled.Y_pos = RemapY(p2.Y_pos, 0);

		SDL_RenderDrawLine(m_pRenderer, p1_scaled.X_pos, p1_scaled.Y_pos, p2_scaled.X_pos, p2_scaled.Y_pos);
	}
}

void Map::RenderAutoMapNode(){
	// Get the last node
    Node node = m_Nodes[m_Nodes.size()-1];

    SDL_Rect RightRect = {
        RemapX(node.RightBoxLeft, 0),
        RemapY(node.RightBoxTop, 0),
        RemapX(node.RightBoxRight, 0) - RemapX(node.RightBoxLeft, 0) + 1,
        RemapY(node.RightBoxBottom, 0) - RemapY(node.RightBoxTop, 0) + 1
    };

    SDL_Rect LeftRect = {
        RemapX(node.LeftBoxLeft, 0),
        RemapY(node.LeftBoxTop, 0),
        RemapX(node.LeftBoxRight, 0) - RemapX(node.LeftBoxLeft, 0) + 1,
        RemapY(node.LeftBoxBottom, 0)  - RemapY(node.LeftBoxTop, 0) + 1
    };

    SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE); 
    SDL_RenderDrawRect(m_pRenderer, &RightRect);
    SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(m_pRenderer, &LeftRect);

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(m_pRenderer,
        RemapX(node.XPartition, 0),
        RemapY(node.YPartition, 0),
        RemapX(node.XPartition + node.ChangeXPartition, 0),
        RemapY(node.YPartition + node.ChangeYPartition, 0) );

}

int Map::ScaleTranslate(int offset, int scale, int x_max, float param_cord){
	return offset + x_max/2 + (-x_max/2 + x_max*param_cord)/scale;
}	

int Map::RemapX(int x, int offset){
	int iRenderXSize;
    int iRenderYSize;

    // Read what is the resolution we are using
    SDL_RenderGetLogicalSize(m_pRenderer, &iRenderXSize, &iRenderYSize);
	iRenderYSize --;
	iRenderXSize --;
	return ScaleTranslate(offset, autoScaleFactor, iRenderXSize, float(x- xMin)/(xMax - xMin));
}

int Map::RemapY(int y, int offset){
	int iRenderXSize;
    int iRenderYSize;

    // Read what is the resolution we are using
    SDL_RenderGetLogicalSize(m_pRenderer, &iRenderXSize, &iRenderYSize);
	iRenderYSize --;
	iRenderXSize --;
	return ScaleTranslate(offset, autoScaleFactor, iRenderYSize, float(-y + yMax)/(yMax - yMin));
}