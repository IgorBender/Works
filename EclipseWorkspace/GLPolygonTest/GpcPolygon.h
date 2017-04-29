#ifndef GPC_POLYGON_H
#define GPC_POLYGON_H


#ifdef __cplusplus
extern "C" {
#endif
#include <gpc.h>
#ifdef __cplusplus
}
#endif

#include <iostream>
using namespace std;


class GpcPolygon
{
public:
    GpcPolygon();
    GpcPolygon(int NumOfVertices, double* Vertices);
    GpcPolygon(const GpcPolygon&);
    GpcPolygon(const gpc_polygon&);
    virtual ~GpcPolygon();

    GpcPolygon& operator=(const GpcPolygon&);
    GpcPolygon& operator+=(const GpcPolygon&);
    GpcPolygon& operator-=(const GpcPolygon&);
    GpcPolygon& operator&=(const GpcPolygon&);
    GpcPolygon& operator^=(const GpcPolygon&);

    friend GpcPolygon operator+(const GpcPolygon&, const GpcPolygon&);
    friend GpcPolygon operator-(const GpcPolygon&, const GpcPolygon&);
    friend GpcPolygon operator&(const GpcPolygon&, const GpcPolygon&);
    friend GpcPolygon operator^(const GpcPolygon&, const GpcPolygon&);
    friend ostream& operator<<(ostream& ostr, GpcPolygon&);
    friend class GpcTrianglesStrips;
    
 private:
    gpc_polygon* m_PolygonPtr;

    gpc_polygon* getPolyPtr()
    {
        return m_PolygonPtr;
    }
    const gpc_polygon* getPolyPtr() const
    {
         return m_PolygonPtr;
    }
};

GpcPolygon operator+(const GpcPolygon&, const GpcPolygon&);
GpcPolygon operator-(const GpcPolygon&, const GpcPolygon&);
GpcPolygon operator&(const GpcPolygon&, const GpcPolygon&);
GpcPolygon operator^(const GpcPolygon&, const GpcPolygon&);

ostream& operator<<(ostream& ostr, GpcPolygon&);

class GpcTrianglesStrips
{
public:
    GpcTrianglesStrips();
    GpcTrianglesStrips(const GpcPolygon&);
    ~GpcTrianglesStrips();
    
    GpcTrianglesStrips& operator=(const GpcPolygon&);
    int getNumOfStrips()
    {
        return m_TristripPtr->num_strips;
    }
    int getNumOfStripVerteces(int StripNum)
    {
        if(0 <= StripNum && StripNum < m_TristripPtr->num_strips)
            return m_TristripPtr->strip[StripNum].num_vertices;
        return 0;
    }
    int getVertex(int StripNum, int VertexNum, gpc_vertex& Vertex);

private:
    gpc_tristrip* m_TristripPtr;
    
    GpcTrianglesStrips(const GpcTrianglesStrips&);
    GpcTrianglesStrips& operator=(const GpcTrianglesStrips&);
};

#endif // GPC_POLYGON_H
