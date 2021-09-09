#include "handmade.h"
#include "handmade_intrinsics.h"

internal void
GameOutputSound(game_state *GameState, game_sound_output_buffer *SoundBuffer, int ToneHz) {
    int16_t ToneVolume = 3000;
    int WavePeriod = SoundBuffer->SamplesPerSecond / ToneHz;

    int16_t *SampleOut = SoundBuffer->Samples;

    for(int SampleIndex = 0; SampleIndex < SoundBuffer->SampleCount; ++SampleIndex) {
#if 0
        float SineValue = sinf(GameState->tSine);
        int16_t SampleValue = (int16_t)(SineValue * ToneVolume);
#else
        int16_t SampleValue = 0;
#endif
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;

#if 0
        GameState->tSine += 2.0f*Pi32 / (float)WavePeriod;

        if(GameState->tSine > 2.0f*Pi32) {
            GameState->tSine -= 2.0f*Pi32;
        }
#endif
    }
}

internal void
DrawRectangle(game_offscreen_buffer *Buffer,
        float RealMinX, float RealMinY, float RealMaxX, float RealMaxY,
        float R, float G, float B)
{
    int32_t MinX = RoundFloatToInt32(RealMinX);
    int32_t MaxX = RoundFloatToInt32(RealMaxX);
    int32_t MinY = RoundFloatToInt32(RealMinY);
    int32_t MaxY = RoundFloatToInt32(RealMaxY);

    if(MinX < 0) { MinX = 0; }
    if(MinY < 0) { MinY = 0; }
    if(MaxX > Buffer->Width) { MaxX = Buffer->Width; }
    if(MaxY > Buffer->Height) { MaxY = Buffer->Height; }

    uint32_t Color = ((RoundFloatToInt32(R * 255.0f) << 16)
            | (RoundFloatToUInt32(G * 255.0f) << 8)
            | (RoundFloatToUInt32(B * 255.0f) << 0));

    uint8_t *Row = ((uint8_t*)Buffer->Memory
            + (MinX * Buffer->BytesPerPixel)
            + (MinY * Buffer->Pitch));

    for(int Y = MinY; Y < MaxY; ++Y) {
        uint32_t *Pixel = (uint32_t*)Row;
        for(int X = MinX; X < MaxX; ++X) {
            *Pixel++ = Color;
        }

        Row += Buffer->Pitch;
    }
}

inline tile_map*
GetTileMap(world *World, int32_t TileMapX, int32_t TileMapY) {
    tile_map *TileMap = 0;
    if((TileMapX >= 0) && (TileMapX < World->TileMapCountX)
            && (TileMapY >= 0) && (TileMapY < World->TileMapCountY))
    {
        TileMap = &World->TileMaps[TileMapY * World->TileMapCountX + TileMapX];
    }

    return TileMap;
}

inline uint32_t
GetTileValueUnchecked(world *World, tile_map *TileMap, int32_t TileX, int32_t TileY) {
    Assert(TileMap);
    Assert((TileX >= 0) && (TileX < World->CountX)
            && (TileY >= 0) && (TileY < World->CountY));
    uint32_t TileMapValue = TileMap->Tiles[TileY * World->CountX + TileX];
    return TileMapValue;
}

inline bool32
IsTileMapPointEmpty(world *World, tile_map *TileMap, int32_t TestTileX, int32_t TestTileY) {
    bool32 Empty = false;

    if(TileMap) {
        if((TestTileX >= 0) && (TestTileX < World->CountX)
                && (TestTileY >= 0) && (TestTileY < World->CountY))
        {
            uint32_t TileMapValue = GetTileValueUnchecked(World, TileMap, TestTileX, TestTileY);
            Empty = (TileMapValue == 0);
        }
    }

    return Empty;
}

inline canonical_position
GetCanonicalPosition(world *World, raw_position Position) {
    canonical_position Result;

    Result.TileMapX = Position.TileMapX;
    Result.TileMapY = Position.TileMapY;

    float X = Position.X - World->UpperLeftX;
    float Y = Position.Y - World->UpperLeftY;
    Result.TileX = FloorFloatToInt32((X) / World->TileSideInPixels);
    Result.TileY = FloorFloatToInt32((Y) / World->TileSideInPixels);
    Result.TileRelX = X - Result.TileX * World->TileSideInPixels;
    Result.TileRelY = Y - Result.TileY * World->TileSideInPixels;

    Assert(Result.TileRelX >= 0);
    Assert(Result.TileRelY >= 0);
    Assert(Result.TileRelX < World->TileSideInPixels);
    Assert(Result.TileRelY < World->TileSideInPixels);

    if(Result.TileX < 0) {
        Result.TileX += World->CountX;
        --Result.TileMapX;
    }

    if(Result.TileY < 0) {
        Result.TileY += World->CountY;
        --Result.TileMapY;
    }

    if(Result.TileX >= World->CountX) {
        Result.TileX -= World->CountX;
        ++Result.TileMapX;
    }

    if(Result.TileY >= World->CountY) {
        Result.TileY -= World->CountY;
        ++Result.TileMapY;
    }

    return Result;
}

internal bool32
IsWorldPointEmpty(world *World, raw_position TestPos) {
    bool32 Empty = false;

    canonical_position CanPos = GetCanonicalPosition(World, TestPos);
    tile_map *TileMap = GetTileMap(World, CanPos.TileMapX, CanPos.TileMapY);
    Empty = IsTileMapPointEmpty(World, TileMap, CanPos.TileX, CanPos.TileY);

    return Empty;
}

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender) {
    Assert((&Input->Controllers[0].Terminator - &Input->Controllers[0].Buttons[0])
            == (ArrayCount(Input->Controllers[0].Buttons)));
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);

#define TILE_MAP_COUNT_X 17
#define TILE_MAP_COUNT_Y 9

    uint32_t Tiles00[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
        {1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  1, 0, 0, 0,  0, 0, 1, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  1, 0, 0, 0,  0, 1, 0, 0, 1},
        {1, 0, 0, 1,  0, 0, 1, 0,  0, 1, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 1, 0,  0, 0, 1, 0,  0, 1, 0, 0,  0, 0, 0, 0, 0},
        {1, 1, 0, 0,  0, 1, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0, 1},
        {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 1, 0, 0,  1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 1, 1, 1},
        {1, 1, 1, 1,  1, 1, 1, 1,  0, 1, 1, 1,  1, 1, 1, 1, 1}
    };

    uint32_t Tiles01[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
        {1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 1, 1, 1,  1, 1, 1, 1,  0, 1, 1, 1,  1, 1, 1, 1, 1}
    };

    uint32_t Tiles10[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
        {1, 1, 1, 1,  1, 1, 1, 1,  0, 1, 1, 1,  1, 1, 1, 1, 1},
        {1, 1, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 1, 0, 1},
        {1, 1, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 1, 0, 1},
        {1, 0, 1, 1,  0, 0, 1, 0,  0, 1, 0, 0,  0, 0, 1, 0, 1},
        {1, 0, 1, 0,  0, 0, 0, 0,  0, 1, 0, 0,  1, 0, 1, 0, 0},
        {1, 1, 0, 0,  0, 1, 0, 0,  0, 1, 0, 0,  1, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 1, 0, 0,  0, 1, 0, 0,  1, 0, 0, 0, 1},
        {1, 1, 0, 0,  1, 1, 0, 0,  0, 1, 0, 0,  1, 0, 0, 0, 1},
        {1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, 1}
    };

    uint32_t Tiles11[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
        {1, 1, 1, 1,  1, 1, 1, 1,  0, 1, 1, 1,  1, 1, 1, 1, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 1, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1, 1},
        {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1, 1},
        {1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, 1}
    };

    tile_map TileMaps[2][2];

    TileMaps[0][0].Tiles = (uint32_t*)Tiles00;
    TileMaps[0][1].Tiles = (uint32_t*)Tiles01;
    TileMaps[1][0].Tiles = (uint32_t*)Tiles10;
    TileMaps[1][1].Tiles = (uint32_t*)Tiles11;

    world World;
    World.TileMapCountX = 2;
    World.TileMapCountY = 2;
    World.CountX = TILE_MAP_COUNT_X;
    World.CountY = TILE_MAP_COUNT_Y;
    World.TileSideInMeters = 1.4f;
    World.TileSideInPixels = 60;

    World.UpperLeftX = -(float)World.TileSideInPixels / 2;
    World.UpperLeftY = 0;

    float PlayerWidth = 0.75f * World.TileSideInPixels;
    float PlayerHeight = (float)World.TileSideInPixels;

    World.TileMaps = (tile_map *)TileMaps;

    game_state *GameState = (game_state *)Memory->PermanentStorage;
    if(!Memory->IsInitialized) {
        GameState->PlayerX = 150;
        GameState->PlayerY = 120;

        Memory->IsInitialized = true;
    }

    tile_map *TileMap = GetTileMap(&World, GameState->PlayerTileMapX, GameState->PlayerTileMapY);
    Assert(TileMap);

    for(int ControllerIndex = 0;
            ControllerIndex < ArrayCount(Input->Controllers);
            ++ControllerIndex)
    {
        game_controller_input *Controller = GetController(Input, ControllerIndex);
        if(Controller->IsAnalog) {
        } else {
            float dPlayerX = 0.0f;
            float dPlayerY = 0.0f;
            if(Controller->MoveUp.EndedDown) {
                dPlayerY = -1.0f;
            }
            if(Controller->MoveDown.EndedDown) {
                dPlayerY = 1.0f;
            }
            if(Controller->MoveLeft.EndedDown) {
                dPlayerX = -1.0f;
            }
            if(Controller->MoveRight.EndedDown) {
                dPlayerX = 1.0f;
            }

            dPlayerX *= 64;
            dPlayerY *= 64;

            float NewPlayerX = GameState->PlayerX + dPlayerX * Input->dtFrame;
            float NewPlayerY = GameState->PlayerY + dPlayerY * Input->dtFrame;

            raw_position PlayerPosition = {
                GameState->PlayerTileMapX, GameState->PlayerTileMapY,
                NewPlayerX, NewPlayerY
            };

            raw_position PlayerLeft = PlayerPosition;
            PlayerLeft.X -= 0.5f * PlayerWidth;

            raw_position PlayerRight = PlayerPosition;
            PlayerRight.X += 0.5f * PlayerWidth;

            if(IsWorldPointEmpty(&World, PlayerPosition)
                    && IsWorldPointEmpty(&World, PlayerLeft)
                    && IsWorldPointEmpty(&World, PlayerRight))
            {
                canonical_position CanPos = GetCanonicalPosition(&World, PlayerPosition);
                GameState->PlayerTileMapX = CanPos.TileMapX;
                GameState->PlayerTileMapY = CanPos.TileMapY;

                GameState->PlayerX = World.UpperLeftX + World.TileSideInPixels * CanPos.TileX + CanPos.TileRelX;
                GameState->PlayerY = World.UpperLeftY + World.TileSideInPixels * CanPos.TileY + CanPos.TileRelY;
            }
        }
    }

    DrawRectangle(Buffer, 0, 0, (float)Buffer->Width, (float)Buffer->Height, 1.0f, 0.0f, 1.0f);

    for(int Row = 0; Row < 9; ++Row) {
        for(int Column = 0; Column < 17; ++Column) {
            uint32_t TileID = GetTileValueUnchecked(&World, TileMap, Column, Row);
            float Gray = 0.5f;

            if(TileID == 1) {
                Gray = 1.0f;
            }

            float MinX = World.UpperLeftX + ((float)Column) * World.TileSideInPixels;
            float MinY = World.UpperLeftY + ((float)Row) * World.TileSideInPixels;
            float MaxX = MinX + World.TileSideInPixels;
            float MaxY = MinY + World.TileSideInPixels;
            DrawRectangle(Buffer, MinX, MinY, MaxX, MaxY, Gray, Gray, Gray);
        }
    }

    float PlayerR = 1.0f;
    float PlayerG = 1.0f;
    float PlayerB = 0.0f;
    float PlayerLeft = GameState->PlayerX - 0.5f * PlayerWidth;
    float PlayerTop = GameState->PlayerY - PlayerHeight;

    DrawRectangle(Buffer,
            PlayerLeft, PlayerTop,
            PlayerLeft + PlayerWidth, PlayerTop + PlayerHeight,
            PlayerR, PlayerG, PlayerB);
}

extern "C" GAME_GET_SOUND_SAMPLES(GameGetSoundSamples) {
    game_state *GameState = (game_state *)Memory->PermanentStorage;
    GameOutputSound(GameState, SoundBuffer, 440);
}

#if HANDMADE_WIN32
#include "windows.h"
BOOL WINAPI DLLMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    return(TRUE);
}

#endif

// Old Functions
/*
   internal void
   RenderWeirdGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset) {
   uint8_t *Row = (uint8_t*)Buffer->Memory;
   for(int Y = 0; Y < Buffer->Height; ++Y) {
   uint32_t *Pixel = (uint32_t*)Row;
   for(int X = 0; X < Buffer->Width; ++X) {
   uint8_t Blue = (uint8_t)(X + BlueOffset);
   uint8_t Green = (uint8_t)(Y + GreenOffset);

 *Pixel++ = ((Green << 16) | Blue);
 }
 Row += Buffer->Pitch;
 }
 }
 */
