<h1>Usage</h1>

C++17 Entity Component System implementation.

The core of ECS is the EcsManager class. It contains all systems
and entities.
Each Entity hold Components as a std::shared_ptr to base Component class.

Each system definition looks like:


``` c++
class RendererSystem : public System<SpriteComponent, PositionComponent>
{
public:
    explicit RendererSystem();

private:
    /**
     * Virtual protected method called each time when public 
     * method BaseSystem::update called
     */
    void update_state(size_t delta) override;
}; 
```

Method update_state() need to be overridden in child class. It 
will be called each time update() method of BaseSystem called.

System class has template method getEntitiesByTags() where template arguments need
to specify desired Component types of Entity(AND operation applied).
System also has method getEntities() which return Entities which
have Component types that specified in System template declaration.

To create Entity or System you need to use createEntity(size_t name) or
createSystem() method of EcsManager class.
Each system update need to be called in update() method of EcsManager class.

To add component to Entity you to do following, for example:


```c++

Entity &ship = createEntity(ship_id);
ship.addComponents<PositionComponent, SpriteComponent, VelocityComponent,
KeyboardComponent, AnimationComponent, CollisionComponent,
        LifeTimeComponent>();
ship.activate();

auto shipSprite = ship.getComponent<SpriteComponent>();
shipSprite->sprite = make_shared<Sprite>(
        utils::getResourcePath("lunar_lander_bw.png"));
shipSprite->sprite->addClipSprite({0, 32, SHIP_WIDTH, SHIP_HEIGHT});
shipSprite->sprite->addClipSprite({20, 32, SHIP_WIDTH, SHIP_HEIGHT});
shipSprite->sprite->addClipSprite({40, 32, SHIP_WIDTH, SHIP_HEIGHT});
shipSprite->sprite->generateDataBuffer()

```

where method getComponent returns shared_ptr to component.
It is important to define Entity as reference because it need to
be modified.

In entity.hpp file:

```c++
namespace ecs
{
typedef robin_hood::unordered_map<size_t, std::shared_ptr<Component>> ComponentMap;
/*
 * Other content
*/
}
```

you can use any unordered_map implementation instead of robin_hood::unordered_map.
Just replace this line with desired unordered_map if you need.

<h1>Installation</h1>
Just copy header files to desired directory.
