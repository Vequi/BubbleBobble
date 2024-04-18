#include "Object.h"
#include "StaticImage.h"

Sound objectPickup[3];

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = OBJECT_TILE_SIZE;
	switch (type)
	{
		case ObjectType::CHERRY: rc = {5*n, 0, n, n}; break;
		case ObjectType::CHILI: rc = {5*n, 3*n, n, n}; break;
			 
		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_OBJECTS), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::CHERRY) {
		objectPickup[0] = LoadSound("BubbleBobble_Audio&SFX/SFX_WAV/GrabFruitSFX.wav");
		PlaySound(objectPickup[0]);
		SetSoundVolume(objectPickup[0], 0.1f);
		return  POINTS_CHERRY;
	}
	else if (type == ObjectType::CHILI)	return POINTS_CHILI;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}