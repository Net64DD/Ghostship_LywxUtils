#include "mod.h"
#include "draw.h"

HM_API void Lib_TextureRect_RGBA32(Gfx** gfxPtr, u32* texture, u32 width, u32 height, f32 xPos, f32 yPos, f32 xScale,
                            f32 yScale) {
    gDPLoadTextureBlock((*gfxPtr)++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, height, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPWideTextureRectangle((*gfxPtr)++, (s32) (xPos * 4.0f), (s32) (yPos * 4.0f),
                            (s32) ((xPos + width * xScale) * 4.0f), (s32) ((yPos + height * yScale) * 4.0f),
                            G_TX_RENDERTILE, 0, 0, (s32) (1.0f / xScale * 1024.0f), (s32) (1.0f / yScale * 1024.0f));
}

HM_API void Lib_TextureRect_RGBA32_InModel(Gfx** gfxPtr, u32* texture, u32 width, u32 height, f32 xPos, f32 yPos, f32 xScale, f32 yScale) {
    gDPPipeSync((*gfxPtr)++);

    // ==========================================
    // 2. EXACT 3D RENDER STATE (From your XML)
    // ==========================================
    gSPSetGeometryMode((*gfxPtr)++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH);
    
    gDPSetCycleType((*gfxPtr)++, G_CYC_2CYCLE);
    gDPSetRenderMode((*gfxPtr)++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gDPSetDepthSource((*gfxPtr)++, G_ZS_PIXEL);
    
    gDPSetCombineLERP((*gfxPtr)++, 
        0, 0, 0, SHADE, 0, 0, 0, 1,
        COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED
    );
    
    gDPSetTexturePersp((*gfxPtr)++, G_TP_PERSP);
    gDPSetTextureDetail((*gfxPtr)++, G_TD_CLAMP);
    gDPSetTextureLOD((*gfxPtr)++, G_TL_TILE);
    gDPSetTextureFilter((*gfxPtr)++, G_TF_BILERP);
    gDPSetTextureConvert((*gfxPtr)++, G_TC_FILT);
    gDPSetAlphaCompare((*gfxPtr)++, G_AC_NONE);

    // Turn textures ON (Scale = 1.0)
    gSPTexture((*gfxPtr)++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetPrimColor((*gfxPtr)++, 0, 0, 255, 255, 255, 255);

    // ==========================================
    // 3. MANUAL TMEM LOADING (gDPSetTextureImage)
    // ==========================================
    
    // Step A: Point the RDP to your physical texture address in RAM
    gDPSetTextureImage((*gfxPtr)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, texture);

    // Step B: Set up the loading tile parameters
    gDPSetTile((*gfxPtr)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, 
               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, 
               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync((*gfxPtr)++);

    // Step C: Load the block of pixels into TMEM 
    // RGBA32 is 4 bytes per pixel, so we use CALC_DXT(width, 4)
    gDPLoadBlock((*gfxPtr)++, G_TX_LOADTILE, 0, 0, (width * height) - 1, CALC_DXT(width, 4));
    gDPPipeSync((*gfxPtr)++);

    // Step D: Set up the rendering tile parameters
    // The pitch calculation (((width * 4) + 7) >> 3) is mandatory hardware math for RGBA32
    gDPSetTile((*gfxPtr)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, (((width * 4) + 7) >> 3), 0, G_TX_RENDERTILE, 0, 
               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, 
               G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);

    // Step E: Set the exact dimensions of the rendering tile
    gDPSetTileSize((*gfxPtr)++, G_TX_RENDERTILE, 0, 0, 
                   (width - 1) << G_TEXTURE_IMAGE_FRAC, 
                   (height - 1) << G_TEXTURE_IMAGE_FRAC);
}