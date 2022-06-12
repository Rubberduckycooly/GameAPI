#include "Game.h"

// -------------------------
// ENGINE VARIABLES
// -------------------------

RSDKFunctionTable RSDK;
#if MANIA_USE_PLUS
APIFunctionTable API;
#endif
#if RETRO_USE_MOD_LOADER
ModFunctionTable Mod;
#endif

RSDKSceneInfo *SceneInfo = NULL;

RSDKGameInfo *GameInfo = NULL;
#if MANIA_USE_PLUS
RSDKSKUInfo *SKU = NULL;
#endif

RSDKControllerState *ControllerInfo = NULL;
RSDKAnalogState *AnalogStickInfoL   = NULL; // should be called "AnalogStickInfo" for Pre-Plus but its easier to be consistent this way
#if MANIA_USE_PLUS
RSDKAnalogState *AnalogStickInfoR = NULL;
RSDKTriggerState *TriggerInfoL    = NULL;
RSDKTriggerState *TriggerInfoR    = NULL;
#endif
RSDKTouchInfo *TouchInfo = NULL;

#if MANIA_USE_PLUS
RSDKUnknownInfo *UnknownInfo = NULL;
#endif

RSDKScreenInfo *ScreenInfo = NULL;

// -------------------------
// GAME VARIABLES
// -------------------------

GlobalVariables *globals;

// -------------------------
// LINK GAME/MOD LOGIC
// -------------------------

void InitGameLogic(void);

#if MANIA_USE_PLUS
void LinkGameLogicDLL(EngineInfo *info)
{
    memset(&API, 0, sizeof(APIFunctionTable));
    memset(&RSDK, 0, sizeof(RSDKFunctionTable));

    if (info->functionTable)
        memcpy(&RSDK, info->functionTable, sizeof(RSDKFunctionTable));

    if (info->APITable)
        memcpy(&API, info->APITable, sizeof(APIFunctionTable));

#if RETRO_USE_MOD_LOADER
    if (info->modTable)
        memcpy(&Mod, info->modTable, sizeof(ModFunctionTable));
#endif

    GameInfo       = info->gameInfo;
    SKU            = info->currentSKU;
    SceneInfo      = info->sceneInfo;
    ControllerInfo = info->controllerInfo;

    AnalogStickInfoL = info->stickInfoL;
    AnalogStickInfoR = info->stickInfoR;
    TriggerInfoL     = info->triggerInfoL;
    TriggerInfoR     = info->triggerInfoR;
    TouchInfo        = info->touchInfo;

    UnknownInfo = info->unknownInfo;
    ScreenInfo  = info->screenInfo;
}
#else
void LinkGameLogicDLL(EngineInfo info)
{
    // Actual params are:
    // void LinkGameLogicDLL(void *functionTable, RSDKGameInfo *gameInfo, RSDKSceneInfo *sceneInfo, RSDKControllerState *controllerInfo,
    //                       RSDKAnalogState *stickInfoL, RSDKTouchInfo *touchInfo, RSDKScreenInfo *screenInfo)
    // But by passing EngineInfo by value we can create a sorta "container" for it, to keep things consistent & easy to manage

    memset(&RSDK, 0, sizeof(RSDKFunctionTable));

    if (info.functionTable)
        memcpy(&RSDK, info.functionTable, sizeof(RSDKFunctionTable));

#if RETRO_USE_MOD_LOADER
    if (info.modTable)
        memcpy(&Mod, info.modTable, sizeof(ModFunctionTable));
#endif

    GameInfo         = info.gameInfo;
    SceneInfo        = info.sceneInfo;
    ControllerInfo   = info.controllerInfo;
    AnalogStickInfoL = info.stickInfoL;
    TouchInfo        = info.touchInfo;
    ScreenInfo       = info.screenInfo;
}
#endif

#if RETRO_USE_MOD_LOADER
#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))

void InitModAPI(void);

bool32 LinkModLogic(EngineInfo *info, const char *id)
{
#if MANIA_USE_PLUS
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif

    globals = Mod.GetGlobals();

    InitModAPI();

    return true;
}
#endif