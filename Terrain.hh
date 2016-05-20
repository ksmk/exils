#pragma once 

#include "Definitions.hh"

struct Coord { Float x,y,z; };
struct Vec   { Float x,y,z; };
struct Tri   { Vec N; Coord center; Coord vertices[3]; };

#define MAP_X	 (1024)
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
	Float GetSegmentIntersection(Float x, Float y, Float z, Float vx, Float vy, Float vz, Float dst);
	std::vector<Vec> GetCollisionNormals(Coord &center, Float radius);
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