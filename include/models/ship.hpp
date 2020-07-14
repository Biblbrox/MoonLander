#include <iostream>
#include <texture.hpp>
#include "types.hpp"
#include "models/model.hpp"

class Ship: public Model {
public:
    explicit Ship();
    virtual ~Ship();
};