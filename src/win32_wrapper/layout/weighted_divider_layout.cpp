#include <win32_wrapper/layout/weighted_divider_layout.hpp>

#include <algorithm>
#include <numeric>

namespace win32w
{


std::vector<float> WeightedDividerLayout::normalizedRatios()
{
    int n = children_.size();
    std::vector<float> paddedRatios {  };

    if (n < ratios.size())
    {
        paddedRatios.resize(n);
        std::copy_n(ratios.begin(), n, paddedRatios.begin());
    } else if(n > ratios.size())
    {
        paddedRatios.resize(ratios.size());
        std::copy(ratios.begin(), ratios.end(), paddedRatios.begin());
        paddedRatios.resize(n, defaultRatio);
    }

    float sum = std::accumulate(paddedRatios.begin(), paddedRatios.end(), 0);
    std::vector<float> normalized{};
    normalized.resize(n);
    std::transform(paddedRatios.begin(), paddedRatios.end(), normalized.begin(), [&](float r) { return r/sum; });

    return normalized;
}

void WeightedDividerLayout::updateLayout()
{
    int N = children_.size();
    auto nratios = normalizedRatios();
    float runningSum = 0;

    if(orientation == HORIZONTAL) {
        for(int i = 0; i < N; i++)
        {
            float x = x_ + runningSum * width_ + i * padding;
            float y = y_ + padding;
            float w = ratios[i] * width_ - 2*padding;
            float h = height_ - 2*padding;

            children_[i]->setRectangle(x, y, w, h);
            runningSum += ratios[i];
        }
    } else if(orientation == VERTICAL) {
        for(int i = 0; i < N; i++)
        {
            float x = x_ + padding;
            float y = y_ + runningSum * height_ + i * padding;
            float w = width_ - 2*padding;
            float h = ratios[i] * height_ - 2*padding;

            children_[i]->setRectangle(x, y, w, h);
            runningSum += ratios[i];
        }
    }
}

}