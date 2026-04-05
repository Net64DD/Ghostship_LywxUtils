#include "mod.h"
#include "sm64.h"
#include "engine/math_util.h"
#include "game/object_list_processor.h"

typedef u32 ModelID;

typedef enum {
    MODEL_DISPLAY_LIST,
    MODEL_GEO_LAYOUT,
} LoadedModelType;

typedef enum {
    MODEL_PRIVATE,
    MODEL_PUBLIC,
} ModelVisibility;

typedef struct {
    const char* name;
    LoadedModelType type;
    void* model;
    ModelID id;
    ModelVisibility visibility;
} LoadedModel;

#define SpawnObjectSymbol "SpawnObject"
#define RegisterModelSymbol "RegisterModel"
#define GetModelByNameSymbol "GetModelByName"
#define GetModelByIdxSymbol "GetModelByIdx"
#define GetAllModelsSymbol "GetAllModels"
#define GetModelCountSymbol "GetModelCount"

typedef struct Object* (*SpawnObjectFunc)(ModelID modelId, const BehaviorScript* behavior, s16 x, s16 y, s16 z, s32 param);
typedef ModelID (*RegisterModelFunc)(const char* name, const void* data, LoadedModelType type, ModelVisibility visibility);
typedef LoadedModel* (*GetModelByNameFunc)(const char* name);
typedef LoadedModel* (*GetModelByIdxFunc)(uint32_t idx);
typedef void (*GetAllModelsFunc)(LoadedModel** out);
typedef uint32_t (*GetModelCountFunc)();

#define CallUtil(func, ...) CALL_FUNC("LywxUtils", func, __VA_ARGS__)