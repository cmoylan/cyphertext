#include "CollisionManager.h"


void
CollisionManager::registerEntity(Entity *entity)
{
    entities.push_back(entity);
}


void
CollisionManager::registerBulletRegistry(BulletRegistry *registry)
{
    bulletRegistries.push_back(registry);
}


bool
overlapping(int minA, int maxA, int minB, int maxB)
{
    // TODO: memoize
    return minB <= maxA && minA <= maxB;
}


void
CollisionManager::tick()
{
    // iterate over entities
    // see if any have collided with a projectile
    // call #hit events on projectile and entity
    std::vector<Bullet>::iterator bullet;
    std::vector<Bullet> *bullets = &BulletRegistry::getInstance().bullets;

    // NOTE: not crazy about the BulletRegistry singleton...
    for (bullet = bullets->begin(); bullet != bullets->end();) {
        //printf("bullet iteration \n");
        int bulletLeft = bullet->element.origin.x;
        int bulletRight = bulletLeft + bullet->element.size.x;
        int bulletBottom = bullet->element.origin.y;
        int bulletTop = bulletBottom + bullet->element.size.y;

        for (Entity *entity : entities) {
            // check if there is a collision
            // TODO: memoize
            if (overlapping(entity->origin.x, entity->origin.x + entity->size.x,
                            bulletLeft, bulletRight)
                    && overlapping(entity->origin.y, entity->origin.y + entity->size.y,
                                   bulletBottom, bulletTop)) {
                entity->hit();
                bullet->removed = true;
            }
        }

        ++bullet;
    }

    BulletRegistry::getInstance().flush();
}
