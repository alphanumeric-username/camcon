#pragma once

#include <win32_wrapper/layout/layout_element.hpp>

#include <vector>

namespace win32w
{

enum WeightedDividerLayoutOrientation { HORIZONTAL, VERTICAL };

class WeightedDividerLayout: LayoutElement
{
public:
    WeightedDividerLayoutOrientation orientation{HORIZONTAL};
    float padding{10};
    std::vector<float> ratios {};
    float defaultRatio {1.0};

    // virtual void addChild(std::shared_ptr<LayoutElement> child) override;
    virtual void updateLayout() override;
    std::vector<float> normalizedRatios();
};

}