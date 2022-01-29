#pragma once

#include <cstdint>

#include "Angle.h"
// Subsector Identifier is the 16th bit which
// indicate if the node ID is a subsector.
// The node ID is stored as uint16
// 0x8000 in binary 1000000000000000
#define SUBSECTORIDENTIFIER 0x8000
#define PI 3.14159265359
enum EMAPLUMPSINDEX {
  eTHINGS = 1,
  eLINEDEFS,
  eSIDEDDEFS,
  eVERTEXES,
  eSEAGS,
  eSSECTORS,
  eNODES,
  eSECTORS,
  eREJECT,
  eBLOCKMAP,
  eCOUNT
};

enum ELINEDEFFLAGS {
  eBLOCKING = 0,
  eBLOCKMONSTERS = 1,
  eTWOSIDED = 2,
  eDONTPEGTOP = 4,
  eDONTPEGBOTTOM = 8,
  eSECRET = 16,
  eSOUNDBLOCK = 32,
  eDONTDRAW = 64,
  eDRAW = 128
};

enum THINGTYPE { ePLAYER = 1 };

struct header {
  char WAD_type[5];
  uint32_t dir_count;
  uint32_t dir_offset;
};

struct dir {
  uint32_t lump_offset;
  uint32_t lump_size;
  char lump_name[9];
};

struct Thing {
  int16_t XPosition;
  int16_t YPosition;
  uint16_t Angle;
  uint16_t Type;
  uint16_t Flags;
};

struct Vertex {
  int16_t X_pos;
  int16_t Y_pos;
};

struct WADSector {
  int16_t FloorHeight;
  int16_t CeilingHeight;
  char FloorTexture[8];
  char CeilingTexture[8];
  uint16_t Lightlevel;
  uint16_t Type;
  uint16_t Tag;
};

struct Sector {
  int16_t FloorHeight;
  int16_t CeilingHeight;
  char FloorTexture[9];
  char CeilingTexture[9];
  uint16_t Lightlevel;
  uint16_t Type;
  uint16_t Tag;
};

struct WADSidedef {
  int16_t XOffset;
  int16_t YOffset;
  char UpperTexture[8];
  char LowerTexture[8];
  char MiddleTexture[8];
  uint16_t SectorID;
};

struct Sidedef {
  int16_t XOffset;
  int16_t YOffset;
  char UpperTexture[9];
  char LowerTexture[9];
  char MiddleTexture[9];
  Sector *pSector;
};

struct WADlinedef {
  uint16_t start_vertex;
  uint16_t end_vertex;
  uint16_t flags;
  uint16_t line_type;
  uint16_t sector_tag;
  uint16_t right_sidedef;
  uint16_t left_sidedef;
};

struct Linedef {
  Vertex *pStartVertex;
  Vertex *pEndVertex;
  uint16_t Flags;
  uint16_t LineType;
  uint16_t SectorTag;
  Sidedef *pRightSidedef;
  Sidedef *pLeftSidedef;
};

struct WADSeg {
  uint16_t StartVertexID;
  uint16_t EndVertexID;
  uint16_t Angle;
  uint16_t LinedefID;
  uint16_t Direction;  // 0 same as , 1 opposite of
  uint16_t Offset;     // distance along  to start of seg
};

struct Seg {
  Vertex *pStartVertex;
  Vertex *pEndVertex;
  Angle SlopeAngle;
  Linedef *pLinedef;
  uint16_t Direction;  // 0 same as linedef, 1 opposite of linedef
  uint16_t Offset;     // distance along linedef to start of seg
  Sector *pRightSector;
  Sector *pLeftSector;
};

struct Subsector {
  uint16_t SegCount;
  uint16_t FirstSegID;
};

struct Node {
  int16_t XPartition;
  int16_t YPartition;
  int16_t ChangeXPartition;
  int16_t ChangeYPartition;

  int16_t RightBoxTop;
  int16_t RightBoxBottom;
  int16_t RightBoxLeft;
  int16_t RightBoxRight;

  int16_t LeftBoxTop;
  int16_t LeftBoxBottom;
  int16_t LeftBoxLeft;
  int16_t LeftBoxRight;

  uint16_t RightChildID;
  uint16_t LeftChildID;
};

