#include "PostSsdResultsEngine.h"

HIAI_StatusT PostSsdResultsEngine::Init(const hiai::AIConfig& config, const std::vector<hiai::AIModelDescription>& model_desc) {
    return HIAI_OK;
}

HIAI_IMPL_ENGINE_PROCESS("PostSsdResultsEngine", PostSsdResultsEngine, DST_INPUT_SIZE){
    if (arg0 == nullptr){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[DstEngine]  The input arg0 is nullptr");
        return HIAI_ERROR;
    }
    auto inputArg = std::static_pointer_cast<DeviceStreamData>(arg0);

    SendData(0, "DeviceStreamData", static_pointer_cast<void>(inputArg));
    return HIAI_OK;
}
