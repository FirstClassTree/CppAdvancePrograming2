#include "../common/MySatelliteView.h"

MySatelliteView::MySatelliteView(const std::vector<std::vector<char>>& view)
    : view_(view), rows_(view.size()), cols_(rows_ > 0 ? view[0].size() : 0) {}

char MySatelliteView::getObjectAt(size_t x, size_t y) const {
    if (x >= rows_ || y >= cols_) {
        return '&';
    }
    return view_[x][y];
}
