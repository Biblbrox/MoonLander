#ifndef MOONLANDER_ENTITY_HPP
#define MOONLANDER_ENTITY_HPP

#include <vector>
#include <map>
#include <memory>
#include "component.hpp"
#include "world.hpp"

class World;
class Component;

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

    /**
     * Create new component and return
     * ComponentType must be child of Component class
     * @tparam ComponentType
     * @return
     */
    template <class ComponentType>
    const Component& addComponent() const
    {
        ComponentType newComponent;
        components.insert({{typeid(newComponent).hash_code(), newComponent}});
        return (*components.rbegin()).second;
    }

    /**
     * Get component by type
     * @tparam ComponentType
     * @return
     */
    template <class ComponentType>
    const Component& getComponent()
    {
        auto it = components.find(typeid(ComponentType).hash_code());
        if (it == components.end()) {
            // throw error
        }
        return (*it).second;
    }

    virtual void setWorld(std::shared_ptr<World> world) final;

    void activate();

private:
    std::map<size_t, Component> components;
    std::shared_ptr<World> pWorld;
};

#endif //MOONLANDER_ENTITY_HPP
