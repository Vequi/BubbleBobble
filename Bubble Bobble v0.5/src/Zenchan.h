#pragma once
#include "Enemy.h"

#define ZENCHAN_SPEED			1

enum class ZenchanState { JUMPING, ROAMING, FALLING };

enum class ZenchanAnim {
	WALKING_RIGHT, WALKING_LEFT,
	WALKING_MADLEFT, WALKING_MADRIGHT,
	DEAD,
	NUM_ANIMATIONS
};

class Zenchan : public Enemy {
public:
	Zenchan(const Point& p, int width, int height, int frame_width, int frame_height);
	~Zenchan();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;


	//To avoid errors
	void GetShootingPosDir(Point* pos, Point* dir) const {};

	//Get enemy type
	virtual EnemyType GetEnemyType() const override;


private:

	//Movement
	void MoveX(const AABB& box);
	void MoveY(const AABB& box);
	void StartFalling();

	ZenchanState state;

};