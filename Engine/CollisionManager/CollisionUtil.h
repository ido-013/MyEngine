#pragma once

#include <glm/glm.hpp>

bool IsCollisionPointSquare(const glm::vec2& _pointPos, const glm::vec2& _squarePos, const glm::vec2& _squareScale);
bool IsCollisionAABB(const glm::vec2& _aPos, const glm::vec2& _aScale, const glm::vec2& _bPos, const glm::vec2& _bScale);