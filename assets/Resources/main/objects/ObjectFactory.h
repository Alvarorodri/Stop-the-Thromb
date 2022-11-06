#ifndef _OBJECT_FACTORY_INCLUDE
#define _OBJECT_FACTORY_INCLUDE

#include <map>
#include <set>

#include "Object.h"
#include "PowerUp.h"
#include "TextureManager.h"

class ObjectFactory {

private:

	static ObjectFactory *instance_;

	ObjectFactory();
    ~ObjectFactory();

    void lateDestroyObject();

public:

	static ObjectFactory *getInstance();

    void init();
    void update(int deltaTime);
    void render();

    void setProjection(glm::mat4 *project);

    void spawnObject(const glm::vec2 &pos, Object::ObjectType type);
    void destroyObject(const int &id);
	void destroyAllObjects();

public:

	float mapSpeed;

private:

    glm::mat4 *projection;

    map<int, Object *> objects;
    int last_id = 0;

    set<int> pendingToBeDestroyed;
};

#endif // _OBJECT_FACTORY_INCLUDE
