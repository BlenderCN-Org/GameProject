#ifndef COLLISION_FUNCS_HPP
#define COLLISION_FUNCS_HPP

#include "Shapes/PlaneShape.hpp"
#include "Shapes/SphereShape.hpp"
#include "Shapes/AABBShape.hpp"
#include "Shapes/OOBBShape.hpp"

bool PlaneVsSphere(const PlaneShape& p, const SphereShape& s);

bool SphereVsSphere(const SphereShape& p, const SphereShape& s);

bool AABBVsAABB(const AABBShape& a1, const AABBShape& a2);

bool AABBVsOOBB(const AABBShape& a, const OOBBShape& o);

bool OOBBVsOOBB(const OOBBShape& o1, const OOBBShape& o2);

#endif