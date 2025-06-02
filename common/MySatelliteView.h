#pragma once

#include "SatelliteView.h"
#include <vector>
#include <cstddef>

class MySatelliteView : public SatelliteView {
public:
    MySatelliteView(const std::vector<std::vector<char>>& view);

    char getObjectAt(size_t x, size_t y) const override;

private:
    const std::vector<std::vector<char>>& view_;
    size_t rows_, cols_;
};
