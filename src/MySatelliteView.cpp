#include "../common/MySatelliteView.h"
#include <iostream>
#include <functional>
#include "../common/Direction.h"


MySatelliteView::MySatelliteView(std::vector<std::vector<char>> view)
    : view_(std::move(view)), rows_(view_.size()),
      cols_(rows_ > 0 ? view_[0].size() : 0) {}

char MySatelliteView::getObjectAt(size_t x, size_t y) const {
  if (x >= rows_ || y >= cols_) {
    return '&';
  }
  return view_[x][y];
}
const std::vector<std::vector<char>> &MySatelliteView::get_grid() const {
  return view_;
}

void MySatelliteView::setDirectionMap(std::unordered_map<std::pair<size_t, size_t>, Direction> map) {
  tank_directions_ = std::move(map);
}

// Retrieve all tank directions
const std::unordered_map<std::pair<size_t, size_t>, Direction>&
MySatelliteView::get_directions() const {
  return tank_directions_;
}