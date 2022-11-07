#include "Worm.h"

Worm::Worm(glm::mat4 *project, int id, bool upOrDown, int bossID) :Character(project, id, Collision::Enemy) {
	this->upOrDown = upOrDown;
	live = 0;
	this->bossID = bossID;
	init();
}

void Worm::init() {

	for (int i = 0; i <= 8; ++i) {
		parts.push_back(new Part(projection,id+i+1,i));
	}

#pragma region DummySprite
	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Boss);

	sprite = Sprite::createSprite(glm::ivec2(26, 22), glm::vec2(1 / 2.0, 1 / 2.0), spritesheet, projection);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0625*1.0, 0.f));

	sprite->changeAnimation(0, false);
#pragma endregion

	if (upOrDown) currentRute = RouteUp;
	else currentRute = RouteDown;

	targetPosition = Routes[(int)currentRute][currentTarget];

	pos = spawnPoint;
	for (int i = 0; i <= 8; ++i) {
		parts[i]->setPosition(spawnPoint);
	}

}

void Worm::update(int deltaTime)
{
	for (auto part : parts) {
		if (!part->isdamaged()) part->shoot();
	}

	if (currentRute == SpawnPoint) {
		if (!goingToSpawn) CharacterFactory::getInstance()->wormRetun(this->id, bossID, upOrDown);
		goingToSpawn = true;
	}

	if (!goingToSpawn) {
		changeTarget();

		glm::vec2 dir = getDir(pos, targetPosition);
		setPosition(dir);
	}
	else {
		targetPosition = spawnPoint;
		
		pos = parts[0]->getPosition();
		glm::vec2 dir = getDir(pos, targetPosition);
		setPosition(dir);

		if (parts.size() > 0 && distance(parts[0]->getPosition(), spawnPoint) <= 3.0f) {
			parts[0]->deleteRoutine();
			delete parts[0];
			parts.erase(parts.begin());
		}

		if (parts.size() == 0) {
			CharacterFactory::getInstance()->destroyCharacter(id);
		}
	}
	
}

void Worm::render()
{
	for (int i = 0; i < (int)parts.size(); ++i) {
		parts[i]->render();
	}
}

void Worm::rotateSprite(Part *part, const glm::vec2 &vector) {
	float angle = atan2(vector.y, vector.x) * (180.0f / PI);
	angle += 180.0f;

	part->rotateSprite(glm::vec3(0.0f, 0.0f, angle));
}

void Worm::setPosition(const glm::vec2 &newDir) {
	for (int i = (int)parts.size() - 1; i > 0; --i) {
		if (distance(	parts[i]->getPosition() + parts[i]->anchorPoint, 
						parts[i - 1]->getPosition() + parts[i - 1]->anchorPoint) >= 13.0f) {
			glm::vec2 dir = getDir(	parts[i]->getPosition() + parts[i]->anchorPoint,
									parts[i - 1]->getPosition() + parts[i - 1]->anchorPoint);
			parts[i]->setPosition(parts[i]->getPosition() + 2.0f * dir);
			rotateSprite(parts[i], 2.0f * 2.0f * dir);
		}
	}

	this->pos = this->pos + 2.0f * newDir;
	parts[0]->setPosition(pos);
	rotateSprite(parts[0], 2.0f * newDir);
}

void Worm::changeTarget() {

	if (abs(distance(targetPosition, pos)) <= 1.0f) {
		ableToChange = true;
		if (currentTarget >= routesSize[(int)currentRute]) {
			currentRute = nextRoute();
			currentTarget = 0;
		}
		targetPosition = Routes[(int)currentRute][currentTarget];
		currentTarget++;

		//Offset to rectify deviation from script
		targetPosition.y -= 10.0f;
		targetPosition.x -= 10.0f;
	}

	if (ableToChange && currentTarget == 0) {
		ableToChange = false;

		currentRute = nextRoute();
		currentTarget = 0;

		targetPosition = Routes[(int)currentRute][currentTarget];
		currentTarget++;

		//Offset to rectify deviation from script
		targetPosition.y -= 10.0f;
		targetPosition.x -= 10.0f;
	}
}

Worm::routesEnum Worm::nextRoute() {
	srand(time(NULL));
	if (upOrDown) {
		if (currentRute == RouteUp && live == 7) currentRute = SpawnPoint;
		else {
			int newOptions = rand() % IAUp[(int)currentRute].size();
			currentRute = (routesEnum)IAUp[(int)currentRute][newOptions];
		}
	}
	else {
		if (currentRute == RouteDown && live == 7) currentRute = SpawnPoint;
		else {
			int newOptions = rand() % IADown[(int)currentRute - 3].size();
			currentRute = (routesEnum)IADown[(int)currentRute - 3][newOptions];
		}
	}
	return currentRute;
}

glm::vec2 Worm::getDir(const glm::vec2 &posA, const glm::vec2 &posB) {
	glm::vec2 vector = posB - posA;
	float angle = atan2(vector.y, vector.x);
	glm::vec2 dir = glm::vec2(cos(angle), sin(angle));
	return dir;
}

void Worm::damage(int dmg, int id) {
	for (int i = 1; i < parts.size() - 1; ++i) {
			if (id == parts[i]->getId() && !parts[i]->isdamaged()) {
			parts[i]->damage(dmg, id);
			live += 1;
		}
	}
}


Part::Part(glm::mat4 *project, int id, int idBody) :Character(project, id, Collision::Enemy) {
	this->idBody = idBody;
	damaged = false;
	init();
}

void Part::init() {
	bJumping = false;
	shootDelay = 60;

	spritesheet = TextureManager::getInstance()->getSpriteSheet(TextureManager::Textures::Boss);

	if (idBody == 0) {
		sprite = Sprite::createSprite(glm::ivec2(26, 22), glm::vec2(1.f / 9.34f, 1.f / 11.f), spritesheet, projection);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1.f / 9.34f*0.f, 1.f / 11.f*6.f));

		collider->addCollider(glm::ivec4(0, 0, 26, 22));
		collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

		anchorPoint = glm::vec2(18.0f, 11.0f);

	}
	else if (idBody == 8) {
		sprite = Sprite::createSprite(glm::ivec2(26, 22), glm::vec2(1.f / 9.34f, 1.f / 11.f), spritesheet, projection);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1.f / 9.34f*0.f, 1.f / 11.f*5.f));

		collider->addCollider(glm::ivec4(0, 0, 26, 22));
		collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

		anchorPoint = glm::vec2(8.0f, 11.0f);
	}
	else {
		sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f / 16.f, 1 / 16.0), spritesheet, projection);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(1.f/16.f*2.f, 1.f / 16.f*9.f));
		
		sprite->setAnimationSpeed(1, 8);
		sprite->addKeyframe(1, glm::vec2(1.f / 16.f*3.f, 1.f / 16.f*9.f));

		collider->addCollider(glm::ivec4(0, 0, 16, 16));
		collider->changePositionAbsolute(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

		anchorPoint = glm::vec2(8.0f, 8.0f);

	}
	

	sprite->changeAnimation(0, false);

	

#ifdef SHOW_HIT_BOXES
	collider->showHitBox();
#endif // SHOW_HIT_BOXES

	sprite->setPosition(this->pos);
}

void Part::update(int deltaTime) {
}

void Part::setPosition(const glm::vec2 &pos) {
	this->pos = pos;
	sprite->setPosition(this->pos);
	collisionSystem->updateCollider(collider, this->pos);
	collider->changePositionAbsolute(this->pos);
}

bool Part::isdamaged() {
	return damaged;
}

void Part::rotateSprite(glm::vec3 rotation) {
	rotate(rotation.x, rotation.y, rotation.z);
}

void Part::damage(int dmg, int id) {
	sprite->changeAnimation(1, false);
	damaged = true;
	ExplosionFactory::getInstance()->spawnExplosion(Explosion::ExplosionEnemy, projection, pos, getBoundingBox());
}

void Part::shoot() {
	if (shootDelay == 0) {
		shootDelay = 60;
		int valor = rand() % 20;
		if (valor == 1) {
			glm::vec2 playerpos;
			bool existsPlayer = CharacterFactory::getInstance()->getPlayerPos(playerpos);

			if (!existsPlayer) return;

			glm::vec2 dir = (playerpos + glm::vec2(16.f, 7.f)) - (pos + anchorPoint);

			float angle = atan2(dir.y, dir.x);
			dir = glm::vec2(cos(angle), sin(angle));

			float velocity = 1.5f;
			dir *= velocity;

			ProjectileFactory::getInstance()->spawnProjectile(pos + anchorPoint, dir, false, Projectile::EnemyProjectile);
		}
	}
	else shootDelay -= 1;
}
