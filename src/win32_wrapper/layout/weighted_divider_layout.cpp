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
        paddedRatios.resize(ratios.size(), 0);
        std::copy(ratios.begin(), ratios.end(), paddedRatios.begin());
        paddedRatios.resize(n, defaultRatio);
    } else {
        paddedRatios.resize(n, 0);
        std::copy(ratios.begin(), ratios.end(), paddedRatios.begin());
    }

    float sum = std::accumulate(paddedRatios.begin(), paddedRatios.end(), 0.0f);
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
    float runningLenSum = 0;

    float x0 = x_ + margin;
    float y0 = y_ + margin;
    float effWidth = width_ - 2 * margin;
    float effHeight = height_ - 2 * margin;

    if(orientation == HORIZONTAL) {

        float tightWidth {
            effWidth - (N - 1) * padding
        };

        for(int i = 0; i < N; i++)
        {
            // float x = x_ + runningLenSum + (i + 1) * padding;
            float x = x0 + runningLenSum + i * padding;
            // float y = y_ + padding;
            float y = y0;
            // float w = nratios[i] * width_ - 2*padding;
            float w = nratios[i] * tightWidth;
            // float h = height_ - 2*padding;
            float h = effHeight;

            children_[i]->setRectangle(x, y, w, h);
            runningLenSum += w;
        }
    } else if(orientation == VERTICAL) {
        float tightHeight {
            effHeight - (N - 1) * padding
        };

        for(int i = 0; i < N; i++)
        {
            // float x = x_ + padding;
            float x = x0;
            // float y = y_ + runningLenSum + (i + 1) * padding;
            float y = y0 + runningLenSum + i * padding;
            // float w = width_ - 2*padding;
            float w = effWidth;
            // float h = nratios[i] * height_ - 2*padding;
            float h = nratios[i] * tightHeight;

            children_[i]->setRectangle(x, y, w, h);
            runningSum += nratios[i];
            runningLenSum += h;
        }
    }
}

}