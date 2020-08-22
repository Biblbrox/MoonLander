#ifndef MOONLANDER_PHYSICS_SYSTEM_H
#define MOONLANDER_PHYSICS_SYSTEM_H

#include <components/velocitycomponent.hpp>
#include "../ces/system.hpp"

class PhysicsSystem : public System<system_holder<VelocityComponent>> {
public:
    void update(size_t delta) override;
};

#endif //MOONLANDER_PHYSICS_SYSTEM_H
