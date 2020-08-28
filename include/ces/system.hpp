#ifndef MOONLANDER_SYSTEM_HPP
#define MOONLANDER_SYSTEM_HPP

#include <string>
#include <vector>
#include <set>
#include <memory>
#include "utils.hpp"
#include "world.hpp"
#include "basesystem.hpp"
#include "entity.hpp"

//class Entity;
class World;

using utils::type_id;

/**
 * Holder for component types
*/
template <typename ...Args>
struct system_holder;

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
    explicit System()
    {
        fill_types<Args...>();
    }

    virtual ~System() = default;

    /**
     * Returns entities which corresponds to the componentTypes container filter
     * @return
     */
    auto getEntities() const
    {
        auto filtered = pWorld->getEntities();
        for (auto it = filtered.begin(); it != filtered.end();) {
            auto components = it->second->getComponents();
            if (std::any_of(componentTypes.begin(), componentTypes.end(), [&components](size_t t) {
                return components.find(t) == components.end();
            }))
                it = filtered.erase(it);
            else
                ++it;
        }

        return filtered;
    }

    template <class ComponentType>
    auto getEntitiesByTag() const
    {
        auto filtered = pWorld->getEntities();
        for (auto it = filtered.begin(); it != filtered.end();) {
            if (it->second->getComponent<ComponentType>() == nullptr)
                it = filtered.erase(it);
            else
                ++it;
        }

        return filtered;
    }

private:
    // Contains id's of each component type system can handle
    std::set<size_t> componentTypes;

    // Base case
    template <typename T>
    constexpr void fill_types()
    {
        componentTypes.insert(type_id<T>());
    }

    // Fill set componentTypes by types which that system can handle
    template <typename T, typename U, typename ...ComponentArgs>
    constexpr void fill_types()
    {
        fill_types<T>();
        fill_types<U, ComponentArgs...>();
    }
};

#endif //MOONLANDER_SYSTEM_HPP
