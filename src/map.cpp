#include "map.h"

#include <iostream>
#include <memory>
std::string Map::get_name() { return map_name; }

Map::Map(ViewRenderer *pViewRenderer, std::string s_name, Player *pPlayer)
    : map_name(s_name),
      autoScaleFactor(1),
      pPlayer(pPlayer),
      m_pViewRenderer(pViewRenderer) {
  lumpIndex = -1;
}

Map::~Map() { delete pPlayer; }

int Map::GetLumpIndex() { return lumpIndex; }

void Map::SetLumpIndex(int i) { lumpIndex = i; }

void Map::add_vertex(vertex &v) {
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

void Map::add_linedef(linedef &l) { m_linedefs.push_back(l); }

void Map::add_thing(Thing &t) {
  if (t.Type == pPlayer->GetID()) SetPlayer(t);
  m_things.push_back(t);
}

void Map::add_node(Node &n) { m_Nodes.push_back(n); }

void Map::add_subsector(Subsector &s) { m_Subsectors.push_back(s); }

void Map::add_seg(Seg &s) { m_Segs.push_back(s); }

void Map::SetPlayer(Thing &t) {
  pPlayer->SetX(t.XPosition);
  pPlayer->SetY(t.YPosition);
  pPlayer->SetAngle(t.Angle);
}

vertex &Map::get_vert(int i) {
  try {
    return m_verexes[i];
  } catch (const std::exception &e) {
    throw e;
  }
}

std::string Map::to_desmos() {
  std::string s;
  for (int i = 0; i < m_linedefs.size(); i++) {
    auto x1 = get_vert(m_linedefs[i].start_vertex).X_pos,
         x2 = get_vert(m_linedefs[i].end_vertex).X_pos;
    auto y1 = get_vert(m_linedefs[i].start_vertex).Y_pos,
         y2 = get_vert(m_linedefs[i].end_vertex).Y_pos;
    int size =
        std::snprintf(nullptr, 0, "\\operatorname{polygon}((%d,%d),(%d,%d))\n",
                      x1, y1, x2, y2) +
        1;
    auto buffer_string = std::make_unique<char[]>(size);
    std::sprintf(buffer_string.get(),
                 "\\operatorname{polygon}((%d,%d),(%d,%d))\n", x1, y1, x2, y2);
    s += buffer_string.get();
  }
  return s;
}

void Map::RenderAutoMap() {
  RenderAutoMapWalls();
}

void Map::RenderAutoMapWalls() {
  m_pViewRenderer->SetDrawColor(255, 255, 255);
  for (int i = 0; i < m_linedefs.size(); i++) {
    auto p1 = m_verexes[m_linedefs[i].start_vertex],
         p2 = m_verexes[m_linedefs[i].end_vertex];
    vertex p1_scaled, p2_scaled;

    m_pViewRenderer->DrawLine(p1.X_pos, p1.Y_pos, p2.X_pos, p2.Y_pos);
  }
}

void Map::RenderAutoMapNode(int NodeID) {
  // Get the last node
  Node node = m_Nodes[NodeID];
  m_pViewRenderer->SetDrawColor(0, 255, 0);
  m_pViewRenderer->DrawRect(node.RightBoxLeft, node.RightBoxTop,
                            node.RightBoxRight, node.RightBoxBottom);

  m_pViewRenderer->SetDrawColor(255, 0, 0);
  m_pViewRenderer->DrawRect(node.LeftBoxLeft, node.LeftBoxTop,
                            node.LeftBoxRight, node.LeftBoxLeft);

  m_pViewRenderer->SetDrawColor(0, 0, 255);
  m_pViewRenderer->DrawLine(node.XPartition, node.YPartition,
                            node.XPartition + node.ChangeXPartition,
                            node.YPartition + node.ChangeYPartition);
}

bool Map::IsPointOnLeftNodeSide(int X, int Y, int NodeId) {
  int dx = X - m_Nodes[NodeId].XPartition;
  int dy = Y - m_Nodes[NodeId].YPartition;
  return (dx * m_Nodes[NodeId].ChangeYPartition) -
             (dy * m_Nodes[NodeId].ChangeXPartition) <=
         0;
}

void Map::RenderBSPNodes() { RenderBSPNodes(m_Nodes.size() - 1); }

void Map::RenderBSPNodes(int NodeID) {
  if (NodeID & SUBSECTORIDENTIFIER) {
    RenderSubsector(NodeID & (~SUBSECTORIDENTIFIER));
    return;
  }
  bool onleft = IsPointOnLeftNodeSide(pPlayer->GetX(), pPlayer->GetY(), NodeID);
  if (onleft) {
    RenderBSPNodes(m_Nodes[NodeID].LeftChildID);
    RenderBSPNodes(m_Nodes[NodeID].RightChildID);
  } else {
    RenderBSPNodes(m_Nodes[NodeID].RightChildID);
    RenderBSPNodes(m_Nodes[NodeID].LeftChildID);
  }
}

void Map::RenderSubsector(int subID) {
  Subsector subsector = m_Subsectors[subID];
  m_pViewRenderer->SetDrawColor(rand() % 255, rand() % 255, rand() % 255);
  for (int i = 0; i < subsector.SegCount; i++) {
    Seg seg = m_Segs[subsector.FirstSegID + i];
    Angle a1, a2;
    if (pPlayer->IsLineInFOV(m_verexes[seg.StartVertexID],
                             m_verexes[seg.EndVertexID], a1, a2)) {
      m_pViewRenderer->AddWallInFOV(seg, a1, a2);
    }
  }
}