#pragma once

class EntityArchetypeBase {
};

template<typename T>
class EntityArchetype : EntityArchetypeBase {
    static EntityArchetype& instance()
    {
        static EntityArchetype singleton;
        return singleton;
    }

    EntityArchetype(const EntityArchetype&) = delete;
    EntityArchetype(const EntityArchetype&&) = delete;
    EntityArchetype& operator=(EntityArchetype&) = delete;
    EntityArchetype& operator=(EntityArchetype&&) = delete;

private:
    EntityArchetype();
    ~EntityArchetype() = default;
};

template<typename T>
EntityArchetype<T>::EntityArchetype()
{

}