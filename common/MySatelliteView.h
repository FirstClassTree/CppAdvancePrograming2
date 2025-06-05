#pragma once

#include "SatelliteView.h"
#include "../common/Direction.h"
#include "../common/HashUtils.h"
#include <unordered_map>
#include <cstddef>
#include <vector>
#include <unordered_map>

class MySatelliteView : public SatelliteView {
public:
  MySatelliteView(std::vector<std::vector<char>> view);

  char getObjectAt(size_t x, size_t y) const override;
  const std::vector<std::vector<char>>& get_grid() const;


  // set direction map
void setDirectionMap(std::unordered_map<std::pair<size_t, size_t>, Direction> map);


  // Access the direction map
  const std::unordered_map<std::pair<size_t, size_t>, Direction>& get_directions() const;

private:
  std::vector<std::vector<char>> view_;
  // Used for visualizer
  std::unordered_map<std::pair<size_t, size_t>, Direction> tank_directions_;
  size_t rows_, cols_;
};
