#ifndef MOONLANDER_SYSTEM_HPP
#define MOONLANDER_SYSTEM_HPP

#include <string>
#include <vector>
#include <set>
#include <memory>
#include "utils.hpp"
#include "world.hpp"
#include "basesystem.h"
#include "entity.hpp"

//class Entity;
class World;

using Utils::type_id;

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
    System()
    {
        fill_types<Args...>();
    }

    virtual ~System() = default;

    /**
     * Returns entities which corresponds to the componentTypes container filter
     * @return
     */
    virtual const std::vector<Entity> getEntities() const
    {
        const std::vector<Entity>& all_entities = pWorld->getEntities();
        std::vector<Entity> filtered;

        // Filter Entities which contains requires Components
        std::copy_if(all_entities.begin(), all_entities.end(), std::back_inserter(filtered),
                     [this](const Entity& el){
                         auto components = el.getComponents();
                         return !std::any_of(componentTypes.begin(), componentTypes.end(),
                                              [&components](size_t t) { return components.find(t) == components.end(); });
                     });

        return filtered;
    }

    template <class ComponentType>
    std::vector<Entity> getEntitiesByTag() const
    {
        std::vector<Entity> filtered = pWorld->getEntities();
        filtered.erase(std::remove_if(filtered.begin(), filtered.end(), [](Entity& e) {
            return e.getComponent<ComponentType>() == nullptr;
        }), filtered.end());

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
