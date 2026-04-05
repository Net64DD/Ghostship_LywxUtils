#pragma once

#include "types.h"

extern void Lib_TextureRect_RGBA32(Gfx** gfxPtr, u32* texture, u32 width, u32 height, f32 xPos, f32 yPos, f32 xScale, f32 yScale);
extern void Lib_TextureRect_RGBA32_InModel(Gfx** gfxPtr, u32* texture, u32 width, u32 height, f32 xPos, f32 yPos, f32 xScale, f32 yScale);

#define Lib_TextureRect_RGBA32Symbol "Lib_TextureRect_RGBA32"
#define Lib_TextureRect_RGBA32_InModelSymbol "Lib_TextureRect_RGBA32_InModel"

typedef void (*Lib_TextureRect_RGBA32Func)(Gfx** gfxPtr, u32* texture, u32 width, u32 height, f32 xPos, f32 yPos, f32 xScale, f32 yScale);
typedef void (*Lib_TextureRect_RGBA32_InModelFunc)(Gfx** gfxPtr, u32* texture, u32 width, u32 height, f32 xPos, f32 yPos, f32 xScale, f32 yScale);