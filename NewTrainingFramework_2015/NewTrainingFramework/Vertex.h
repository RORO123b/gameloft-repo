#pragma once
#include "Math.h"

struct Vertex
{
	Vector3 pos;
	Vector3 color;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;

	Vertex()
		: pos(0, 0, 0), color(1, 1, 1), norm(0, 0, 0), binorm(0, 0, 0), tgt(0, 0, 0), uv(0, 0) {
	}

	Vertex(const Vertex& other)
		: pos(other.pos), color(other.color), norm(other.norm),
		binorm(other.binorm), tgt(other.tgt), uv(other.uv) {
	}
};