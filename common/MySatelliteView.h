#pragma once

#include "SatelliteView.h"
#include <cstddef>
#include <vector>

class MySatelliteView : public SatelliteView {
public:
  MySatelliteView(std::vector<std::vector<char>> view);

  char getObjectAt(size_t x, size_t y) const override;

private:
  std::vector<std::vector<char>> view_;
  size_t rows_, cols_;
};
