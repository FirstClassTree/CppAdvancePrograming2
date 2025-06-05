// To be able to create a hashmap of pairs
#pragma once

#include <utility>
#include <cstddef>
#include <functional>   

// Specialize std::hash in the std namespace
namespace std {
    template <>
    struct hash<std::pair<size_t, size_t>> {
        std::size_t operator()(const std::pair<size_t, size_t>& p) const noexcept {
            return hash<size_t>{}(p.first) ^ (hash<size_t>{}(p.second) << 1);
        }
    };
}
