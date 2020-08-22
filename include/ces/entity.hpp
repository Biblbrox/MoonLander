#ifndef MOONLANDER_ENTITY_HPP
#define MOONLANDER_ENTITY_HPP

#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include "component.hpp"
#include "../utils.hpp"
//#include "world.hpp"

class World;
//class Component;

using Utils::type_id;

class Entity {
public:
    /**
     * Create new component and return
     * ComponentType must be child of Component class
     * @tparam ComponentType
     * @return
     */
    template <class ComponentType>
    std::shared_ptr<Component> addComponent()
    {
        components[type_id<ComponentType>()] =
                std::static_pointer_cast<Component>(std::make_shared<ComponentType>());
        return components[type_id<ComponentType>()];
    }

    /**
     * Get component by type
     * @tparam ComponentType
     * @return
     */
    template <class ComponentType>
    std::unordered_map<size_t, std::shared_ptr<Component>>::iterator getComponentIt()
    {
        auto it = components.find(type_id<ComponentType>());
        return it;
    }

    /**
     * Get component by type
     * @tparam ComponentType
     * @return
     */
    template <class ComponentType>
    std::shared_ptr<ComponentType> getComponent()
    {
        auto it = components.find(type_id<ComponentType>());
        if (it == components.end())
            return std::shared_ptr<ComponentType>(nullptr);

        return std::dynamic_pointer_cast<ComponentType>((*it).second);
    }

    const std::unordered_map<size_t, std::shared_ptr<Component>>& getComponents() const;

    void setWorld(std::shared_ptr<World> world);

    void activate();

private:
    std::unordered_map<size_t, std::shared_ptr<Component>> components;
    std::shared_ptr<World> pWorld;
};

#endif //MOONLANDER_ENTITY_HPP
