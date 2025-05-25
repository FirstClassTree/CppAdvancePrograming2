#ifndef IJSON_H
#define IJSON_H

#include <string>
#include "../include/json.hpp"

class IJson {
public:
    virtual ~IJson() = default;
    virtual std::string serialize() = 0;
};

#endif
