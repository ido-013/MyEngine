#include "CollisionUtil.h"

bool IsCollisionPointSquare(const glm::vec2& _pointPos, const glm::vec2& _squarePos, const glm::vec2& _squareScale)
{
	float aX = _pointPos.x;
	float aY = _pointPos.y;

	float bX = _squarePos.x;
	float bY = _squarePos.y;
	float bW = _squareScale.x / 2;
	float bH = _squareScale.y / 2;

	if (aX > bX + bW) return false;
	if (aX < bX - bW) return false;
	if (aY > bY + bH) return false;
	if (aY < bY - bH) return false;

	return true;
}

bool IsCollisionAABB(const glm::vec2& _aPos, const glm::vec2& _aScale, const glm::vec2& _bPos, const glm::vec2& _bScale)
{
	float aX = _aPos.x;
	float aY = _aPos.y;
	float aW = _aScale.x / 2;
	float aH = _aScale.y / 2;

	float bX = _bPos.x;
	float bY = _bPos.y;
	float bW = _bScale.x / 2;
	float bH = _bScale.y / 2;

	if (aX - aW > bX + bW) return false;
	if (bX - bW > aX + aW) return false;
	if (aY - aH > bY + bH) return false;
	if (bY - bH > aY + aH) return false;

	return true;
}
