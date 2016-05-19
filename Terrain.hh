#pragma once 

#include "Definitions.hh"

struct Coord { Float x,y,z; };
struct Vec   { Float x,y,z; };
struct Tri   { Vec N; Coord center; Coord vertices[3]; };

#define MAP_SIZE (1024*1024)

class Terrain
{
public:
	Terrain();
	~Terrain();
	void Load();
	void Display();
	void Normals();
	Float GetHeight(Float x, Float z);
	Float GetSegmentIntersectionLambda();

private:
	GLUbyte heightmap[MAP_SIZE];

	std::vector<Tri> triangles;
	Int Terrain_id;
	Int Normals_id;

	void  ComputeTriangle(std::vector<Coord> &tri);
	void  SetPerVertexNormal(Int x, Int y, Int z);

	bool CollisionCheck(Coord P, Float radius, Tri tri, Coord &center);
	bool CollisionCheck(Coord P, Coord Q, 	   Tri tri, Float &lambda);

	Float GetVertexHeight(Int x, Int z);
};