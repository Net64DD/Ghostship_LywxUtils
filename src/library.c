#include "library.h"

#include "mod.h"
#include "array.h"
#include "game/object_helpers.h"
#include "port/events/Events.h"
#include "game/level_update.h"
#include "engine/level_script.h"

#define MODEL_BASE 0xE1

static GenericArray gLoadedModels;
static ListenerID gLoadAreaListenerID;

extern void LoadModel(uint16_t modelId, const void* data, LoadedModelType type);

HM_API struct Object* SpawnObject(ModelID modelId, const BehaviorScript* behavior, s16 x, s16 y, s16 z, s32 param) {
    struct Object* object =
        spawn_object_abs_with_rot(&gMacroObjectDefaultParent, 0, modelId, behavior, x, y, z, 0, 0, 0);
    object->custom = true;
    return object;
}

HM_API ModelID RegisterModel(const char* name, const void* data, LoadedModelType type, ModelVisibility visibility) {
    ModelID modelId = MODEL_BASE + gLoadedModels.size;
    LoadedModel instance = { .name = name, .type = type, .model = data, .id = modelId, .visibility = visibility };

    Array_Push(&gLoadedModels, &instance);
    return modelId;
}

HM_API LoadedModel* GetModelByName(const char* name) {
    for (size_t i = 0; i < gLoadedModels.size; i++) {
        LoadedModel* loadedModel = (LoadedModel*)Array_Get(&gLoadedModels, i);
        if (strcmp(loadedModel->name, name) == 0) {
            return loadedModel;
        }
    }
    return NULL;
}

HM_API LoadedModel* GetModelByIdx(uint32_t idx) {
    if (idx >= gLoadedModels.size) {
        return NULL;
    }
    return (LoadedModel*)Array_Get(&gLoadedModels, idx);
}

HM_API void GetAllModels(LoadedModel** out) {
    for (size_t i = 0; i < gLoadedModels.size; i++) {
        out[i] = (LoadedModel*)Array_Get(&gLoadedModels, i);
    }
}

HM_API uint32_t GetModelCount() {
    return (uint32_t) gLoadedModels.size;
}

void OnLoadArea(IEvent* ev) {
    for (size_t i = 0; i < gLoadedModels.size; i++) {
        LoadedModel* loadedModel = (LoadedModel*)Array_Get(&gLoadedModels, i);
        LoadModel(MODEL_BASE + i, loadedModel->model, loadedModel->type);
    }
}

void LoadModel(uint16_t modelId, const void* data, LoadedModelType type) {
    switch (type) {
        case MODEL_DISPLAY_LIST:
            gLoadedGraphNodes[modelId] = (struct GraphNode *) init_graph_node_display_list(sLevelPool, 0, LAYER_OPAQUE, data);
            break;
        case MODEL_GEO_LAYOUT:
            gLoadedGraphNodes[modelId] = process_geo_layout(sLevelPool, data);
            break;
        default:
            LUSLOG_ERROR("Unknown model type for model ID %d\n", modelId);
    }
}

MOD_INIT() {
    LUSLOG_INFO("%s", "Initializing internal model registry");
    Array_Init(&gLoadedModels, 16, sizeof(LoadedModel));
    LUSLOG_INFO("%s", "Registering event listeners");
    gLoadAreaListenerID = REGISTER_LISTENER(LevelScriptBeginArea, EVENT_PRIORITY_NORMAL, OnLoadArea);
    LUSLOG_INFO("%s", "Initialization complete");
}

MOD_EXIT() {
    LUSLOG_INFO("%s", "Exiting LywxUtils");
    UNREGISTER_LISTENER(LevelScriptBeginArea, gLoadAreaListenerID);
    Array_Free(&gLoadedModels);
}