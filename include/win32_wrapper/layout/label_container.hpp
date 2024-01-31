#pragma once

#include <win32_wrapper/layout/layout_element.hpp>

#include <win32_wrapper/gdi/gdi_text.hpp>

namespace win32w
{


class LabelContainer : public LayoutElement
{
private:
    std::shared_ptr<GDIText> gdiText_{ nullptr };

public:
    LabelContainer();
    LabelContainer(std::shared_ptr<GDIText> gdiText);

    // int alignment { TOP | LEFT };

    void setGDIText(std::shared_ptr<GDIText> gdiText);

    virtual void updateLayout() override;

};

}