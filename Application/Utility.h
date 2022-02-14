#pragma once
#include "Source/Vertex.h"
#include "Mtx44.h"

Position operator*(const Mtx44& lhs, const Position& rhs);