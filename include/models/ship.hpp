#include <iostream>
#include <texture.hpp>
#include "types.hpp"
#include "models/model.hpp"

class Ship: public Model {
public:
    explicit Ship();
    virtual ~Ship();

    v_type getVelX() const;
    v_type getVelY() const;

    Velocity getVel() const;

private:
    v_type vel_x;
    v_type vel_y;
};