
#include <systems/animationsystem.hpp>

void AnimationSystem::update(size_t delta)
{
    std::vector<Entity> entities = getEntities();
    for (auto& en: entities) {
        en.getComponent<SpriteComponent>()->sprite->setIdx(en.getComponent<AnimationComponent>()->cur_state);
    }
}