#ifndef MOONLANDER_SYSTEM_HPP
#define MOONLANDER_SYSTEM_HPP

#include <string>
#include <vector>
#include <set>
#include <memory>
#include "utils.hpp"

class Entity;
class World;

using Utils::type_id;

/**
 * Holder for component types
*/
template <typename ...Args>
struct system_holder;

/**
 * System base class
 */
class BaseSystem {};

/**
 * Base template
 * Need for system_holder specialization
 */
template <typename ...Args>
class System;

/**
 * Specialization
 */
template <typename ...Args>
class System<system_holder<Args...>> : public BaseSystem {
public:
    System();

    virtual ~System() = default;

    /**
     * Returns entities which corresponds to the componentFilter
     * @return
     */
    virtual const std::vector<Entity>& getEntities() const final;

    virtual void setWorld(std::shared_ptr<World> world) final;

private:
    // Contains id's of each component type system can handle
    std::set<short> componentTypes;

    template <typename T, typename ...ComponentArgs>
    void fill_types()
    {
        componentTypes.insert(type_id<T>());
        fill_types<ComponentArgs...>();
    }

    // Base case
    template <typename T>
    void fill_types()
    {
        componentTypes.insert(type_id<T>());
    }

    std::shared_ptr<World> pWorld;
};

#endif //MOONLANDER_SYSTEM_HPP
