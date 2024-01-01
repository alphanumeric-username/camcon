#include <camcon/preset_store.hpp>

#include <system/io.hpp>

namespace camcon
{

std::vector<Preset> PresetStore::loadPresets()
{
    auto folder { root / L"presets" };
    
    if(!fs::exists(folder))
    {
        fs::create_directory(folder);
    }

    fs::directory_iterator dit{folder};


    std::vector<Preset> ps {};

    for(auto& f : dit)
    {
        if(f.is_regular_file() && f.path().extension().compare(".txt") == 0)
        {
            auto f_lines { sys::read_file_lines(f) };
            Preset p{};
            p.name = f.path().stem();
            // std::vector<mfw::CameraControlPropertyValue> ccpvs{};

            for(auto l : f_lines)
            {
                auto prop {mfw::parsePropConfigString(l)};
                if(prop.valid)
                {
                    p.config.emplace_back(prop);
                }
            }

            ps.emplace_back(p);
        }
    }

    return ps;
}

void PresetStore::savePreset(Preset preset)
{
    auto folder { root / L"presets" };
    std::string contents;

    for(auto& prop : preset.config)
    {
        contents += prop.toString() + '\n';
    }

    sys::save_file(folder / (preset.name + L".txt"), contents);
}

}