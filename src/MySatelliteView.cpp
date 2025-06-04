#include "../common/MySatelliteView.h"
#include <iostream>

MySatelliteView::MySatelliteView(std::vector<std::vector<char>> view)
    : view_(std::move(view)), rows_(view_.size()),
      cols_(rows_ > 0 ? view_[0].size() : 0) {}

char MySatelliteView::getObjectAt(size_t x, size_t y) const {
  if (x >= rows_ || y >= cols_) {
    return '&';
  }
  // The std::cout line can be removed if it was for debugging the old issue.
  // std::cout << "view size: " << view_.size() << " " << view_[x].size() <<
  // std::endl;
  return view_[x][y];
}
const std::vector<std::vector<char>> &MySatelliteView::get_grid() const {
  return view_;
}