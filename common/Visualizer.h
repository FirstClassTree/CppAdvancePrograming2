#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <vector>
#include <memory>

class SatelliteView;

class Visualizer {
public:
    void add_snapshot(std::unique_ptr<SatelliteView> view);
    void run();  // Interactive visual playback with ←/→ support

private:
    std::vector<std::unique_ptr<SatelliteView>> snapshots_;
    void draw_view(const SatelliteView& view, size_t round_index) const;
};

#endif
