#include <GpcPolygon.h>

GpcPolygon::GpcPolygon()
{
    m_PolygonPtr = new gpc_polygon;
    m_PolygonPtr->num_contours = 0;
    m_PolygonPtr->contour = NULL;
    m_PolygonPtr->hole = NULL;
}

GpcPolygon::GpcPolygon(int NumOfVertices, double* Vertices)
{
    m_PolygonPtr = new gpc_polygon;
    m_PolygonPtr->num_contours = 1;
    m_PolygonPtr->contour = (gpc_vertex_list*)malloc(sizeof(gpc_vertex_list));
    m_PolygonPtr->contour->num_vertices = NumOfVertices;
    m_PolygonPtr->contour->vertex = (gpc_vertex*)malloc(sizeof(gpc_vertex) * NumOfVertices);
    int i, j;
    for(i = 0, j= 0;  i < NumOfVertices;  ++i, ++j)
    {
        m_PolygonPtr->contour->vertex[i].x = Vertices[j++];
        m_PolygonPtr->contour->vertex[i].y = Vertices[j];
    }
    m_PolygonPtr->hole = (int*)malloc(sizeof(int));
    m_PolygonPtr->hole[0] = 0;
}

GpcPolygon::GpcPolygon(const GpcPolygon& Polygon)
{
    m_PolygonPtr = new gpc_polygon;
    m_PolygonPtr->num_contours = 0;
    m_PolygonPtr->contour = NULL;
    m_PolygonPtr->hole = NULL;
    GpcPolygon EmptyPolygon;
    gpc_polygon_clip(GPC_UNION, EmptyPolygon.m_PolygonPtr, const_cast < gpc_polygon* > (Polygon.m_PolygonPtr), m_PolygonPtr);
}

GpcPolygon::GpcPolygon(const gpc_polygon& Polygon)
{
    m_PolygonPtr = new gpc_polygon;
    m_PolygonPtr->num_contours = 0;
    m_PolygonPtr->contour = NULL;
    m_PolygonPtr->hole = NULL;
    GpcPolygon EmptyPolygon;
    gpc_polygon_clip(GPC_UNION, EmptyPolygon.m_PolygonPtr, const_cast < gpc_polygon* > (&Polygon), m_PolygonPtr);
}

GpcPolygon::~GpcPolygon()
{
    gpc_free_polygon(m_PolygonPtr);
    delete m_PolygonPtr;
}


GpcPolygon& GpcPolygon::operator=(const GpcPolygon& Polygon)
{
    GpcPolygon EmptyPolygon;
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_UNION, EmptyPolygon.m_PolygonPtr, const_cast < gpc_polygon* > (Polygon.m_PolygonPtr),
                     TmpPolygon.m_PolygonPtr);
    gpc_free_polygon(m_PolygonPtr);
    gpc_polygon_clip(GPC_UNION, EmptyPolygon.m_PolygonPtr, TmpPolygon.m_PolygonPtr, m_PolygonPtr);
    return *this;
}

GpcPolygon& GpcPolygon::operator+=(const GpcPolygon& Polygon)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_UNION, m_PolygonPtr, const_cast < gpc_polygon* > (Polygon.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    gpc_free_polygon(m_PolygonPtr);
    *this = TmpPolygon;
    return *this;
}

GpcPolygon& GpcPolygon::operator-=(const GpcPolygon& Polygon)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_DIFF, m_PolygonPtr, const_cast < gpc_polygon* > (Polygon.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    gpc_free_polygon(m_PolygonPtr);
    *this = TmpPolygon;
    return *this;
}

GpcPolygon& GpcPolygon::operator&=(const GpcPolygon& Polygon)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_INT, m_PolygonPtr, const_cast < gpc_polygon* > (Polygon.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    gpc_free_polygon(m_PolygonPtr);
    *this = TmpPolygon;
    return *this;
}

GpcPolygon& GpcPolygon::operator^=(const GpcPolygon& Polygon)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_XOR, m_PolygonPtr, const_cast < gpc_polygon* > (Polygon.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    gpc_free_polygon(m_PolygonPtr);
    *this = TmpPolygon;
    return *this;
}

GpcPolygon operator+(const GpcPolygon& a, const GpcPolygon& b)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_UNION, a.m_PolygonPtr, const_cast < gpc_polygon* > (b.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    return TmpPolygon;
}

GpcPolygon operator-(const GpcPolygon& a, const GpcPolygon& b)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_DIFF, a.m_PolygonPtr, const_cast < gpc_polygon* > (b.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    return TmpPolygon;
}

GpcPolygon operator&(const GpcPolygon& a, const GpcPolygon& b)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_INT, a.m_PolygonPtr, const_cast < gpc_polygon* > (b.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    return TmpPolygon;
}

GpcPolygon operator^(const GpcPolygon& a, const GpcPolygon& b)
{
    GpcPolygon TmpPolygon;
    gpc_polygon_clip(GPC_XOR, a.m_PolygonPtr, const_cast < gpc_polygon* > (b.m_PolygonPtr), TmpPolygon.m_PolygonPtr);
    return TmpPolygon;
}

ostream& operator << (ostream& ostr, GpcPolygon& Poly)
{
    ostr << "-------------------------------" << endl << "Polygon" << endl;
    ostr << "    Contours : " << Poly.getPolyPtr()->num_contours << endl;
    for(int i = 0;  i < Poly.getPolyPtr()->num_contours;  ++i)
    {
        ostr << (Poly.getPolyPtr()->hole[i] ? "        Hole " : "        Contour ") << i << endl;
        for(int j = 0;  j < Poly.getPolyPtr()->contour[i].num_vertices;  ++j)
        {
            ostr << "            Vertex" << j << " : x = " << Poly.getPolyPtr()->contour[i].vertex[j].x << "; y : "
            << Poly.getPolyPtr()->contour[i].vertex[j].y << endl;
        }
    }
    ostr << "-------------------------------" << endl;

    return ostr;
}

GpcTrianglesStrips::GpcTrianglesStrips()
{
    m_TristripPtr = new gpc_tristrip;
}

GpcTrianglesStrips::GpcTrianglesStrips(const GpcPolygon& Polygon)
{
    m_TristripPtr = new gpc_tristrip;
    gpc_polygon_to_tristrip(const_cast < gpc_polygon* > (Polygon.getPolyPtr()), m_TristripPtr);
}

GpcTrianglesStrips::~GpcTrianglesStrips()
{
    gpc_free_tristrip(m_TristripPtr);
    delete m_TristripPtr;
}

GpcTrianglesStrips& GpcTrianglesStrips::operator=(const GpcPolygon& Polygon)
{
    gpc_free_tristrip(m_TristripPtr);
    gpc_polygon_to_tristrip(const_cast < gpc_polygon* > (Polygon.getPolyPtr()), m_TristripPtr);
    return *this;
}

int GpcTrianglesStrips::getVertex(int StripNum, int VertexNum, gpc_vertex& Vertex)
{
    if(0 <= StripNum && StripNum < m_TristripPtr->num_strips && 0 <= VertexNum && VertexNum < m_TristripPtr->strip[StripNum].num_vertices)
        Vertex = m_TristripPtr->strip[StripNum].vertex[VertexNum];
    return -1;
}
