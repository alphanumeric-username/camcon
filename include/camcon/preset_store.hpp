#pragma once

#include <mf_wrapper/camera_controller.hpp>


#include <filesystem>
#include <string>
#include <vector>


namespace camcon
{

namespace fs = std::filesystem;

struct Preset
{
    std::wstring name {L""};
    std::vector<mfw::CameraControlPropertyValue> config {};
};

class PresetStore
{

public:
    fs::path root{"./"};
    std::vector<Preset> loadPresets();
    void savePreset(Preset preset);
};

}