#ifndef HANDHADE_H
#define HANDMADE_H

#include "handmade_platform.h"

#define internal static
#define local_persist static
#define global_variable static

#define Pi32 3.14159265359f

#if HANDMADE_SLOW
#define Assert(Expression) if(!(Expression)) { *(int *)0 = 0; }
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

inline uint32_t
SafeTruncateUInt64(uint64_t Value) {
    Assert(Value <= 0xFFFFFFFF);
    uint32_t Result = (uint32_t)Value;
    return(Result);
}

inline game_controller_input *GetController(game_input *Input, unsigned int ControllerIndex) {
    Assert(ControllerIndex < ArrayCount(Input->Controllers));

    game_controller_input *Result = &Input->Controllers[ControllerIndex];
    return Result;
}

struct canonical_position {
  int32_t TileMapX;
  int32_t TileMapY;

  int32_t TileX;
  int32_t TileY;

  float TileRelX;
  float TileRelY;
};

struct raw_position {
  int32_t TileMapX;
  int32_t TileMapY;

  float X;
  float Y;
};

struct tile_map {
    uint32_t *Tiles;
};

struct world {
    float TileSideInMeters;
    int32_t TileSideInPixels;

    int32_t CountX;
    int32_t CountY;

    float UpperLeftX;
    float UpperLeftY;

    int32_t TileMapCountX;
    int32_t TileMapCountY;

    tile_map *TileMaps;
};

struct game_state {
    int32_t PlayerTileMapX;
    int32_t PlayerTileMapY;

    float PlayerX;
    float PlayerY;
};

#endif
