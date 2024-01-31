#pragma once

#include <win32_wrapper/layout/layout_element.hpp>
#include <win32_wrapper/window.hpp>

#include <memory>

namespace win32w
{

class LayoutManager
{
public:
    std::shared_ptr<LayoutElement> root{ nullptr };
    std::shared_ptr<Window> win{ nullptr };

    bool stateIsValid() const;
    void update();
};

}