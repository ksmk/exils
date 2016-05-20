#include "Terrain.hh"

Terrain::Terrain() { }
Terrain::~Terrain() { }

Vec cross_product(Vec a, Vec b)
{
    Vec c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}

inline bool fcmp(Vec a, Vec b) { return std::abs(a.y) > std::abs(b.y); }

void Terrain::ComputeTriangle(std::vector<Coord> &triangle)
{
    Tri tri;
    tri.vertices[0] = triangle[0];
    tri.vertices[1] = triangle[1];
    tri.vertices[2] = triangle[2];

    tri.center.x = (triangle[0].x + triangle[1].x + triangle[2].x)/3;
    tri.center.y = (triangle[0].y + triangle[1].y + triangle[2].y)/3;
    tri.center.z = (triangle[0].z + triangle[1].z + triangle[2].z)/3;

    Vec a,b;
    if(triangles.size()%2 != 0) std::swap(triangle[2],triangle[0]);
    a.x = triangle[1].x - triangle[0].x;
    a.y = triangle[1].y - triangle[0].y;
    a.z = triangle[1].z - triangle[0].z;

    b.x = triangle[2].x - triangle[1].x;
    b.y = triangle[2].y - triangle[1].y;
    b.z = triangle[2].z - triangle[1].z;
    if(triangles.size()%2 != 0) std::swap(triangle[2],triangle[0]);

    Vec aux = cross_product(a,b);
    float factor = sqrt( 1.0f/(aux.x*aux.x + aux.y*aux.y + aux.z*aux.z) );
    aux.x *= factor;
    aux.y *= factor;
    aux.z *= factor;
    tri.N = aux;

    triangles.push_back(tri);
    std::swap(triangle[1],triangle[0]);
    std::swap(triangle[2],triangle[1]);
    triangle.pop_back();
}

