#ifndef MOONLANDER_COMPONENT_HPP
#define MOONLANDER_COMPONENT_HPP

class CesManager;

/**
 * Base class for component
 */
class Component
{
public:
    Component() : m_cesManager(nullptr) {};
    virtual ~Component() = default;

    virtual void setCesManager(CesManager* cesMan) final;

private:
    CesManager* m_cesManager;
};

#endif //MOONLANDER_COMPONENT_HPP
