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
    Float factor = sqrt( 1.0f/(aux.x*aux.x + aux.y*aux.y + aux.z*aux.z) );
    aux.x *= factor;
    aux.y *= factor;
    aux.z *= factor;
    tri.N = aux;

    triangles.push_back(tri);
    std::swap(triangle[1],triangle[0]);
    std::swap(triangle[2],triangle[1]);
    triangle.pop_back();
}

void Terrain::Display()
{
    glCallList(Terrain_id);
}

Float Terrain::GetVertexHeight(Int x, Int z)
{
    return heightmap[(Int)z * MAP_X + (Int)x] / FACTOR;
}

bool Terrain::CollisionCheck(Coord P, Float radius, Tri tri, Coord &center)
{
    Coord Q;
    Q.x = P.x + radius*(-tri.N.x);
    Q.y = P.y + radius*(-tri.N.y);
    Q.z = P.z + radius*(-tri.N.z);
    Vector V;
    V.x = Q.x - P.x;
    V.y = Q.y - P.y;
    V.z = Q.z - P.z;

    Float D = -(tri.N.x*tri.vertices[0].x + tri.N.y*tri.vertices[0].y + tri.N.z*tri.vertices[0].z);
    Float lambda = -(tri.N.x*P.x + tri.N.y*P.y + tri.N.z*P.z + D) / (tri.N.x*V.x + tri.N.y*V.y + tri.N.z*V.z);

    if(lambda > 1.0f) return false;
    Coord I;  I.x = P.x + lambda*V.x;  I.y = P.y + lambda*V.y;  I.z = P.z + lambda*V.z;


    Float tri_orientation = (tri.vertices[0].x - tri.vertices[2].x) * (tri.vertices[1].z - tri.vertices[2].z)
                          - (tri.vertices[0].z - tri.vertices[2].z) * (tri.vertices[1].x - tri.vertices[2].x);
    Float ABIor = (tri.vertices[0].x - I.x) * (tri.vertices[1].z - I.z) - (tri.vertices[0].z - I.z) * (tri.vertices[1].x - I.x);
    Float BCIor = (tri.vertices[1].x - I.x) * (tri.vertices[2].z - I.z) - (tri.vertices[1].z - I.z) * (tri.vertices[2].x - I.x);
    Float CAIor = (tri.vertices[2].x - I.x) * (tri.vertices[0].z - I.z) - (tri.vertices[2].z - I.z) * (tri.vertices[0].x - I.x);
    if( tri_orientation >= 0.0f && (ABIor < 0.0f  || BCIor < 0.0f || CAIor < 0.0f) )   return false;
    if( tri_orientation < 0.0f && (ABIor >= 0.0f  || BCIor >= 0.0f || CAIor >= 0.0f) ) return false;


    Float factor = sqrt( (radius*radius)/(tri.N.x*tri.N.x + tri.N.y*tri.N.y + tri.N.z*tri.N.z) );
    center.x = I.x + tri.N.x*factor;
    center.y = I.y + tri.N.y*factor;
    center.z = I.z + tri.N.z*factor;

    return true;
}



