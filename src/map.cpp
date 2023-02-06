#include "map.h"

#include <iostream>
#include <memory>
std::string Map::get_name() { return map_name; }

Map::Map(std::string s_name, Player *pPlayer)
    : map_name(s_name),
      autoScaleFactor(1),
      pPlayer(pPlayer){
  lumpIndex = -1;
}

Map::~Map() { delete pPlayer; }

int Map::GetLumpIndex() { return lumpIndex; }

void Map::SetLumpIndex(int i) { lumpIndex = i; }

void Map::add_vertex(Vertex &v) {
  if (m_verexes.size() == 0) {
    xMin = v.X_pos;
    xMax = v.X_pos;
    yMin = v.Y_pos;
    yMax = v.Y_pos;
  }
  m_verexes.push_back(v);

  xMax = xMax < v.X_pos ? v.X_pos : xMax;
  xMin = xMin > v.X_pos ? v.X_pos : xMin;

  yMax = yMax < v.Y_pos ? v.Y_pos : yMax;
  yMin = yMin > v.Y_pos ? v.Y_pos : yMin;
}

void Map::add_linedef(WADlinedef &l) { m_wad_linedefs.push_back(l); }

void Map::add_thing(Thing &t) {
  if (t.Type == pPlayer->GetID()) SetPlayer(t);
  m_things.push_back(t);
}

void Map::add_sidedef(WADSidedef &s){
  m_wad_Sidedef.push_back(s);
}

void Map::add_sector(WADSector &s){
  m_wad_Sectors.push_back(s);
}

void Map::add_node(Node &n) { m_Nodes.push_back(n); }

void Map::add_subsector(Subsector &s) { m_Subsectors.push_back(s); }

void Map::add_seg(WADSeg &s) { m_wad_Segs.push_back(s); }

void Map::SetPlayer(Thing &t) {
  pPlayer->SetX(t.XPosition);
  pPlayer->SetY(t.YPosition);
  pPlayer->SetZ(41);
  pPlayer->SetAngle(t.Angle);
}

Vertex &Map::get_vert(int i) {
  try {
    return m_verexes[i];
  } catch (const std::exception &e) {
    throw e;
  }
}




std::vector<std::pair<Vertex, Vertex>> Map::GetLvlWalls() {
  std::vector<std::pair<Vertex, Vertex>> ret;
  
  for (int i = 0; i < m_wad_linedefs.size(); i++) {
    auto p1 = m_verexes[m_wad_linedefs[i].start_vertex],
         p2 = m_verexes[m_wad_linedefs[i].end_vertex];
    Vertex p1_scaled, p2_scaled;
    
    ret.push_back({p1, p2});
  }
  return ret;
}


bool Map::IsPointOnLeftNodeSide(int X, int Y, int NodeId) {
  int dx = X - m_Nodes[NodeId].XPartition;
  int dy = Y - m_Nodes[NodeId].YPartition;
  return (dx * m_Nodes[NodeId].ChangeYPartition) -
             (dy * m_Nodes[NodeId].ChangeXPartition) <=
         0;
}

void Map::RenderBSPNodes(std::vector<ViewRendererDataWall> &dataWallsInFOV) { RenderBSPNodes(m_Nodes.size() - 1, dataWallsInFOV); }

void Map::RenderBSPNodes(int NodeID, std::vector<ViewRendererDataWall> &dataWallsInFOV) {
  if (NodeID & SUBSECTORIDENTIFIER) {
    RenderSubsector(NodeID & (~SUBSECTORIDENTIFIER), dataWallsInFOV);
    return;
  }
  bool onleft = IsPointOnLeftNodeSide(pPlayer->GetX(), pPlayer->GetY(), NodeID);
  if (onleft) {
    RenderBSPNodes(m_Nodes[NodeID].LeftChildID, dataWallsInFOV);
    RenderBSPNodes(m_Nodes[NodeID].RightChildID, dataWallsInFOV);
  } else {
    RenderBSPNodes(m_Nodes[NodeID].RightChildID, dataWallsInFOV);
    RenderBSPNodes(m_Nodes[NodeID].LeftChildID, dataWallsInFOV);
  }
}

void Map::RenderSubsector(int subID, std::vector<ViewRendererDataWall> &dataWallsInFOV) {
  Subsector subsector = m_Subsectors[subID];
  for (int i = 0; i < subsector.SegCount; i++) {
    Seg seg = m_Segs[subsector.FirstSegID + i];
    Angle a1, a2, v1AngleFomPlayer, v2AngleFromPlayer;
    if (pPlayer->IsLineInFOV(*(seg.pStartVertex), *(seg.pEndVertex), a1, a2, v1AngleFomPlayer, v2AngleFromPlayer)) {
      dataWallsInFOV.push_back({seg, a1, a2, v1AngleFomPlayer, v2AngleFromPlayer, subID});
    }
  }
}

void Map::Init(){
  BuildSectors();
  BuildSidedef();
  BuildLinedef();
  BuildSeg();
}

void Map::BuildSectors(){
  WADSector wadsector;
    Sector sector;
    for (int i = 0; i < m_wad_Sectors.size(); ++i){
        wadsector = m_wad_Sectors.at(i);
        sector.FloorHeight = wadsector.FloorHeight;
        sector.CeilingHeight = wadsector.CeilingHeight;
        strncpy(sector.FloorTexture, wadsector.FloorTexture, 8);
        sector.FloorTexture[8] = '\0';
        strncpy(sector.CeilingTexture, wadsector.CeilingTexture, 8);
        sector.CeilingTexture[8] = '\0';
        sector.Lightlevel = wadsector.Lightlevel;
        sector.Type = wadsector.Type;
        sector.Tag = wadsector.Tag;
        m_Sectors.push_back(sector);
    }
}

void Map::BuildSidedef(){
  WADSidedef wadsidedef;
    Sidedef sidedef;
    for (int i = 0; i < m_wad_Sidedef.size(); ++i){
        wadsidedef = m_wad_Sidedef.at(i);
        sidedef.XOffset = wadsidedef.XOffset;
        sidedef.YOffset = wadsidedef.YOffset;
        strncpy(sidedef.UpperTexture, wadsidedef.UpperTexture, 8);
        sidedef.UpperTexture[8] = '\0';
        strncpy(sidedef.LowerTexture, wadsidedef.LowerTexture, 8);
        sidedef.LowerTexture[8] = '\0';
        strncpy(sidedef.MiddleTexture, wadsidedef.MiddleTexture, 8);
        sidedef.MiddleTexture[8] = '\0';
        sidedef.pSector = &m_Sectors[wadsidedef.SectorID];
        m_Sidedefs.push_back(sidedef);
    }
}

void Map::BuildLinedef(){
  WADlinedef wadlinedef;
    Linedef linedef;

    for (int i = 0; i < m_wad_linedefs.size(); ++i)
    {
        wadlinedef = m_wad_linedefs.at(i);

        linedef.pStartVertex = &m_verexes[wadlinedef.start_vertex];
        linedef.pEndVertex = &m_verexes[wadlinedef.end_vertex];
        linedef.Flags = wadlinedef.flags;
        linedef.LineType = wadlinedef.line_type;
        linedef.SectorTag = wadlinedef.sector_tag;

        if (wadlinedef.right_sidedef == 0xFFFF){
            linedef.pRightSidedef = nullptr;
        }else{
            linedef.pRightSidedef = &m_Sidedefs[wadlinedef.right_sidedef];
           
        }

        if (wadlinedef.left_sidedef == 0xFFFF){
            linedef.pLeftSidedef = nullptr;
        }else{
            linedef.pLeftSidedef = &m_Sidedefs[wadlinedef.left_sidedef];
        }

        m_Linedefs.push_back(linedef);
    }

}

void Map::BuildSeg(){

    WADSeg wadseg;
    Seg seg;

    for (int i = 0; i < m_wad_Segs.size(); ++i){
        wadseg = m_wad_Segs.at(i);

        seg.pStartVertex = &m_verexes[wadseg.StartVertexID];
        seg.pEndVertex = &m_verexes[wadseg.EndVertexID];
        // 8.38190317e-8 is to conver from Binary angles (BAMS) to float
        seg.SlopeAngle = ((float)(wadseg.Angle << 16) * 8.38190317e-8);
        seg.pLinedef = &m_Linedefs[wadseg.LinedefID];
        seg.Direction = wadseg.Direction;
        seg.Offset = (float)(wadseg.Offset << 16) / (float)(1 << 16);

        Sidedef *pRightSidedef;
        Sidedef *pLeftSidedef;
        if (seg.Direction){
          pRightSidedef = seg.pLinedef->pLeftSidedef;
          pLeftSidedef = seg.pLinedef->pRightSidedef;
        }else{
          pRightSidedef = seg.pLinedef->pRightSidedef;
          pLeftSidedef = seg.pLinedef->pLeftSidedef;
        }

        if (pRightSidedef){
            seg.pRightSector = pRightSidedef->pSector;
        }else{
            seg.pRightSector = nullptr;
        }

        if (pLeftSidedef){
            seg.pLeftSector = pLeftSidedef->pSector;
        }else{
            seg.pLeftSector = nullptr;
        }
        m_Segs.push_back(seg);
    }

}


int Map::GetSectorFloorHeight(){
  int iSsectorID = m_Nodes.size()-1;
  while(!(iSsectorID & SUBSECTORIDENTIFIER)){
    iSsectorID = IsPointOnLeftNodeSide(pPlayer->GetX(), pPlayer->GetY(), iSsectorID) ? 
                 m_Nodes[iSsectorID].LeftChildID: m_Nodes[iSsectorID].RightChildID;
  }
  Subsector &ss = m_Subsectors[iSsectorID & (~SUBSECTORIDENTIFIER)];
  Seg &seg = m_Segs[ss.FirstSegID];
  return seg.pRightSector->FloorHeight;
}


std::vector <Seg> Map::GetSegBySubID(int subID){
  std::vector<Seg> result;
  for (int i = 0; i < m_Subsectors[subID].SegCount; i++) {
    Seg seg = m_Segs[m_Subsectors[subID].FirstSegID + i];
    result.push_back(seg);
  }
  return result;
}
