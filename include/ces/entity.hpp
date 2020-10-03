#ifndef MOONLANDER_ENTITY_HPP
#define MOONLANDER_ENTITY_HPP

#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include "utils/utils.hpp"

class World;
class Component;

using utils::type_id;

/**
 * Entity class
 * Each entity may contain several unique components
 */
class Entity
{
public:

    Entity() : m_alive(false) {}

    /**
     * Create new component and return
     * ComponentType must be child of Component class
     * @tparam ComponentType
     * @return
     */
    template <class ComponentType>
    std::shared_ptr<Component> addComponent()
    {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "Template parameter class must be child of Component");
        m_components[type_id<ComponentType>()] =
                std::static_pointer_cast<Component>(std::make_shared<ComponentType>());
        return m_components[type_id<ComponentType>()];
    }

    /**
     * Get component by type
     * @tparam ComponentType
     * @return
     */
    template <class ComponentType>
    std::unordered_map<size_t, std::shared_ptr<Component>>::iterator
    getComponentIt()
    {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "Template parameter class must be child of Component");
        auto it = m_components.find(type_id<ComponentType>());
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
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "Template parameter class must be child of Component");

        auto it = m_components.find(type_id<ComponentType>());
        if (it == m_components.end())
            return std::shared_ptr<ComponentType>(nullptr);

        return std::dynamic_pointer_cast<ComponentType>(it->second);
    }

    template <class ComponentType>
    void removeComponent()
    {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "Template parameter class must be child of Component");

        auto it = m_components.find(type_id<ComponentType>());
        if (it != m_components.end())
            m_components.erase(it);
    }

    const std::unordered_map<size_t, std::shared_ptr<Component>>&
    getComponents() const;

    void setWorld(std::shared_ptr<World> world);

    void activate();
    bool isActivate();
    void kill();

private:
    std::unordered_map<size_t, std::shared_ptr<Component>> m_components;
    std::shared_ptr<World> m_world;
    bool m_alive;
};

#endif //MOONLANDER_ENTITY_HPP
