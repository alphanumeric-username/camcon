#include <win32_wrapper/layout/label_container.hpp>


namespace win32w
{

win32w::LabelContainer::LabelContainer() { }

win32w::LabelContainer::LabelContainer(std::shared_ptr<GDIText> gdiText)
{
    setGDIText(gdiText);
}

void LabelContainer::setGDIText(std::shared_ptr<GDIText> gdiText)
{
    gdiText_ = gdiText;
}

void LabelContainer::updateLayout()
{
    gdiText_->useRectangle = true;

    gdiText_->x = x_;
    gdiText_->y = y_;
    gdiText_->width = width_;
    gdiText_->height = height_;
    
    // gdiText_->alignment = alignment;
}

}