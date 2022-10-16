#include "Player.h"
#include "Game.h"
#include "GeneralDefines.h"

Player::Player(glm::mat4 *project) {
    projection = project;
    collider = new Collision(project, Collision::Player);

    collisionSystem = CollisionSystem::getInstance();
    collisionSystem->addColliderIntoGroup(collider);
}

void Player::init(const glm::ivec2 &tileMapPos) {
    bJumping = false;
    spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, projection);
    sprite->setNumberAnimations(4);

        sprite->setAnimationSpeed(STAND_LEFT, 8);
        sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

        sprite->setAnimationSpeed(STAND_RIGHT, 8);
        sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

        sprite->setAnimationSpeed(MOVE_LEFT, 8);
        sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
        sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
        sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

        sprite->setAnimationSpeed(MOVE_RIGHT, 8);
        sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
        sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
        sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

    sprite->changeAnimation(0);
    tileMapDispl = tileMapPos;

    collider->addCollider(glm::ivec4(0, 0, 27, 32));
    collider->changePositionAbsolute(glm::ivec2(tileMapDispl.x + posPlayer.x, tileMapDispl.y + posPlayer.y));

#ifdef SHOW_HIT_BOXES
    collider->showHitBox();
#endif // SHOW_HIT_BOXES

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);
    if(Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
        if(sprite->animation() != MOVE_LEFT)
            sprite->changeAnimation(MOVE_LEFT);
        //posPlayer.x -= 2;
        //collider.changePositionRelative(glm::ivec2(-2, 0));
        map->moveMap(2);
        if(collisionSystem->isColliding(Player::collider)) {
            //posPlayer.x += 2;
            //collider.changePositionRelative(glm::ivec2(2, 0));
            map->moveMap(-2);
            sprite->changeAnimation(STAND_LEFT);
        }
    }
    else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
        if(sprite->animation() != MOVE_RIGHT)
            sprite->changeAnimation(MOVE_RIGHT);

        //posPlayer.x += 2;
        //collider.changePositionRelative(glm::ivec2(2, 0));
        map->moveMap(-2);
        if(collisionSystem->isColliding(Player::collider)) {
            //posPlayer.x -= 2;
            //collider.changePositionRelative(glm::ivec2(-2, 0));
            map->moveMap(2);
            sprite->changeAnimation(STAND_RIGHT);
        }
    }
    else {
        if(sprite->animation() == MOVE_LEFT)
            sprite->changeAnimation(STAND_LEFT);
        else if(sprite->animation() == MOVE_RIGHT)
            sprite->changeAnimation(STAND_RIGHT);
    }

    if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
        posPlayer.y += 2;
        collider->changePositionRelative(glm::ivec2(0, 2));
        if (collisionSystem->isColliding(Player::collider))
        {
            posPlayer.y -= 2;
            collider->changePositionRelative(glm::ivec2(0, -2));
        }
    }else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
        posPlayer.y -= 2;
        collider->changePositionRelative(glm::ivec2(0, -2));
        if (collisionSystem->isColliding(Player::collider))
        {
            posPlayer.y += 2;
            collider->changePositionRelative(glm::ivec2(0, 2));
        }
    }

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render() {
    sprite->render();

#ifdef SHOW_HIT_BOXES
    collider->render();
#endif // SHOW_HIT_BOXES
}

void Player::setTileMap(TileMap *tileMap) {
    map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
    posPlayer = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    collider->changePositionAbsolute(glm::ivec2(tileMapDispl.x + posPlayer.x, tileMapDispl.y + posPlayer.y));
}
