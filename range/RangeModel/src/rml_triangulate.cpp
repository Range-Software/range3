/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_triangulate.cpp                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th May 2015                                            *
 *                                                                   *
 *  DESCRIPTION: Triangulate functions                               *
 *********************************************************************/

#include <cmath>

#include "rml_triangulate.h"

/*********************************************************************
 * TYPE DEFINITIONS ================================================ *
 *********************************************************************/

typedef struct _TRIANGLE
{
    int p1;
    int p2;
    int p3;
} TRIANGLE;

typedef struct _EDGE
{
    int p1;
    int p2;
} EDGE;

typedef struct _XYZ
{
    uint id;
    double x;
    double y;
    double z;
} XYZ;

/*********************************************************************
 * STATIC FUNCTIONS DECLARATIONS =================================== *
 *********************************************************************/

static std::vector<TRIANGLE> triangulate(std::vector<XYZ> &pxyz, bool removeZeroSized, double zeroSize);

static bool CircumCircle( double  xp, double  yp,
                          double  x1, double  y1,
                          double  x2, double  y2,
                          double  x3, double  y3,
                          double *xc, double *yc, double *r );

struct less_than_key
{
    inline bool operator() (const XYZ& pxyz1, const XYZ& pxyz2)
    {
        return ((pxyz1.x < pxyz2.x) || (pxyz1.x == pxyz2.x && pxyz1.y < pxyz2.y));
    }
};


/*********************************************************************
 * PUBLIC FUNCTIONS DEFINITIONS ==================================== *
 *********************************************************************/


/*********************************************************************
 * NAME:        RTriangulateNodes                                    *
 * DESCRIPTION: Triangulate set of nodes                             *
 *********************************************************************/
std::vector<RElement> RTriangulateNodes(const std::vector<RNode> &nodes, bool removeZeroSized, double zeroSize)
{
    // Convert nodes to XYZ
    std::vector<XYZ> pxyz;
    pxyz.resize(nodes.size());

    for (uint i=0;i<nodes.size();i++)
    {
        pxyz[i].id = i;
        pxyz[i].x = nodes[i].getX();
        pxyz[i].y = nodes[i].getY();
        pxyz[i].z = nodes[i].getZ();
    }
    std::sort(pxyz.begin(), pxyz.end(), less_than_key());

    std::vector<TRIANGLE>v = triangulate(pxyz,removeZeroSized,zeroSize);

    std::vector<RElement> elements;
    elements.resize(v.size(),RElement(R_ELEMENT_TRI1));

    for (uint i=0;i<elements.size();i++)
    {
        elements[i].setNodeId(0,pxyz[v[i].p1].id);
        elements[i].setNodeId(1,pxyz[v[i].p2].id);
        elements[i].setNodeId(2,pxyz[v[i].p3].id);
    }

    return elements;
} /* RTriangulateNodes */


/*********************************************************************
 * STATIC FUNCTIONS DEFINITIONS ==================================== *
 *********************************************************************/


/*********************************************************************
 * NAME:        triangulate                                          *
 * DESCRIPTION: Triangulation subroutine                             *
 *              Takes as input NV vertices in array pxyz. Returned   *
 *              is a list of ntri triangular faces in the array v.   *
 *              These triangles are arranged in a consistent         *
 *              clockwise order. The triangle array 'v' should be    *
 *              malloced to 3 * nv. The vertex array pxyz must be    *
 *              big enough to hold 3 more points. The vertex array   *
 *              must be sorted in increasing x values say.           *
 *********************************************************************/
static std::vector<TRIANGLE> triangulate (std::vector<XYZ> &pxyz, bool removeZeroSized, double zeroSize)
{
    std::vector<int> complete;
    std::vector<EDGE> edges;
    std::vector<TRIANGLE> v;

    uint nv = uint(pxyz.size());
    uint ntri = 0;

    uint nedge = 0;
    uint emax = 1000;

    double xp=0.0,yp=0.0,x1=0.0,y1=0.0,x2=0.0,y2=0.0,x3=0.0,y3=0.0,xc=0.0,yc=0.0,r=0.0;
    double xmin=0.0,xmax=0.0,ymin=0.0,ymax=0.0;
    double dx=0.0,dy=0.0,dmax=0.0;

    /* Allocate memory for the completeness list, flag for each triangle */
    uint trimax = 4 * nv;
    complete.resize(trimax);
    v.resize(trimax);

    /* Allocate memory for the edge list */
    edges.resize(emax);

    /* Find the maximum and minimum vertex bounds.
       This is to allow calculation of the bounding triangle */

    xmin = pxyz[0].x;
    ymin = pxyz[0].y;
    xmax = xmin;
    ymax = ymin;
    for (uint i=1;i<nv;i++)
    {
        if (pxyz[i].x < xmin) xmin = pxyz[i].x;
        if (pxyz[i].x > xmax) xmax = pxyz[i].x;
        if (pxyz[i].y < ymin) ymin = pxyz[i].y;
        if (pxyz[i].y > ymax) ymax = pxyz[i].y;
    }
    dx = xmax - xmin;
    dy = ymax - ymin;
    dmax = (dx > dy) ? dx : dy;

    /* Set up the supertriangle
       This is a triangle which encompasses all the sample points.
       The supertriangle coordinates are added to the end of the
       vertex list. The supertriangle is the first triangle in
       the triangle list. */

    double dfactor = 20.0;

    pxyz.resize(pxyz.size()+4);
    pxyz[nv+0].x = xmin - dfactor*dmax;
    pxyz[nv+0].y = ymin - dfactor*dmax;
    pxyz[nv+0].z = 0.0;
    pxyz[nv+1].x = xmin - dfactor*dmax;
    pxyz[nv+1].y = ymax + dfactor*dmax;
    pxyz[nv+1].z = 0.0;
    pxyz[nv+2].x = xmax + dfactor*dmax;
    pxyz[nv+2].y = ymin - dfactor*dmax;
    pxyz[nv+2].z = 0.0;
    pxyz[nv+3].x = xmax + dfactor*dmax;
    pxyz[nv+3].y = ymax + dfactor*dmax;
    pxyz[nv+3].z = 0.0;
    v[0].p1 = nv;
    v[0].p2 = nv+1;
    v[0].p3 = nv+2;
    v[1].p1 = nv+2;
    v[1].p2 = nv+1;
    v[1].p3 = nv+3;
    complete[0] = 0;
    complete[1] = 0;
    ntri = 2;

    /* Include each point one at a time into the existing mesh */
    for (uint i=0;i<nv;i++)
    {
        xp = pxyz[i].x;
        yp = pxyz[i].y;
        nedge = 0;

        /* Set up the edge buffer.
           If the point (xp,yp) lies inside the circumcircle then the
           three edges of that triangle are added to the edge buffer
           and that triangle is removed. */
        for (uint j=0;j<ntri;j++)
        {
            if (complete[j] == 1)
            {
                continue;
            }
            x1 = pxyz[v[j].p1].x;
            y1 = pxyz[v[j].p1].y;
            x2 = pxyz[v[j].p2].x;
            y2 = pxyz[v[j].p2].y;
            x3 = pxyz[v[j].p3].x;
            y3 = pxyz[v[j].p3].y;
            bool inside = CircumCircle (xp, yp,
                                        x1, y1,
                                        x2, y2,
                                        x3, y3,
                                        &xc, &yc, &r);
            if (xc + r < xp)
            {
                complete[j] = 1;
            }
            if (inside)
            {
                /* Check that we haven't exceeded the edge list size */
                if (nedge+3 >= emax)
                {
                    emax += 100;
                    edges.resize(emax);
                }
                edges[nedge+0].p1 = v[j].p1;
                edges[nedge+0].p2 = v[j].p2;
                edges[nedge+1].p1 = v[j].p2;
                edges[nedge+1].p2 = v[j].p3;
                edges[nedge+2].p1 = v[j].p3;
                edges[nedge+2].p2 = v[j].p1;
                nedge += 3;
                v[j] = v[ntri-1];
                complete[j] = complete[ntri-1];
                ntri--;
                j--;
            }
        }

        if (nedge > 0)
        {
            /* Tag multiple edges
               Note: if all triangles are specified anticlockwise then all
                     interior edges are opposite pointing in direction. */
            for (uint j=0;j<nedge-1;j++)
            {
                for (uint k=j+1;k<nedge;k++)
                {
                    if ((edges[j].p1 == edges[k].p2) &&
                        (edges[j].p2 == edges[k].p1))
                    {
                        edges[j].p1 = -1;
                        edges[j].p2 = -1;
                        edges[k].p1 = -1;
                        edges[k].p2 = -1;
                    }
                    /* Shouldn't need the following, see note above */
                    if ((edges[j].p1 == edges[k].p1) &&
                        (edges[j].p2 == edges[k].p2))
                    {
                        edges[j].p1 = -1;
                        edges[j].p2 = -1;
                        edges[k].p1 = -1;
                        edges[k].p2 = -1;
                    }
                }
            }
        }

        /* Form new triangles for the current point
           Skipping over any tagged edges.
           All edges are arranged in clockwise order. */
        for (uint j=0;j<nedge;j++)
        {
            if (edges[j].p1 < 0 || edges[j].p2 < 0)
            {
                continue;
            }
            if (ntri >= trimax)
            {
                trimax += 100;
                complete.resize(trimax);
                v.resize(trimax);
            }
            v[ntri].p1 = edges[j].p1;
            v[ntri].p2 = edges[j].p2;
            v[ntri].p3 = i;
            complete[ntri] = 0;
            ntri++;
        }
    }

    /* Remove triangles with supertriangle vertices
       These are triangles which have a vertex number greater than nv */
    for (uint i=0;i<ntri;i++)
    {
        if (v[i].p1 >= int(nv) || v[i].p2 >= int(nv) || v[i].p3 >= int(nv))
        {
            v[i] = v[ntri-1];
            ntri--;
            i--;
        }
    }

    /* Remove zero-sized triangles */
    if (removeZeroSized)
    {
        uint n = 0;
        for (uint i=0;i<ntri;i++)
        {
            RTriangle t(RNode(pxyz[v[i].p1].x,pxyz[v[i].p1].y,pxyz[v[i].p1].z),
                        RNode(pxyz[v[i].p2].x,pxyz[v[i].p2].y,pxyz[v[i].p2].z),
                        RNode(pxyz[v[i].p3].x,pxyz[v[i].p3].y,pxyz[v[i].p3].z));
            if (t.findArea() > zeroSize)
            {
                v[n]=v[i];
                n++;
            }
        }
        ntri = n;
    }
    v.resize(ntri);

    for (uint i=0;i<ntri;i++)
    {
        std::swap(v[i].p2,v[i].p3);
    }

    return v;
} /* triangulate */


/*********************************************************************
 * NAME:        CircumCircle                                         *
 * DESCRIPTION: Return '1' if a point (xp,yp) is inside the          *
 *              circumcircle made up of the points (x1,y1), (x2,y2), *
 *              (x3,y3).                                             *
 *              The circumcircle centre is returned in (xc,yc) and   *
 *              the radius r.                                        *
 *              NOTE: A point on the edge is inside the circumcircle *
 *********************************************************************/
static bool CircumCircle(double  xp, double  yp,
                         double  x1, double  y1,
                         double  x2, double  y2,
                         double  x3, double  y3,
                         double *xc, double *yc, double *r)
{
    double m1,m2,mx1,mx2,my1,my2;
    double dx,dy,rsqr,drsqr;
    double tolerance = 0.0;

    tolerance = RConstants::eps;
    tolerance = 0.0;

   /* Check for coincident points */
    if (std::fabs(y1-y2) <= tolerance &&
        std::fabs(y2-y3) <= tolerance)
    {
        return (0);
    }

    if (std::fabs(y2-y1) <= tolerance)
    {
        m2 = - (x3-x2) / (y3-y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        *xc = (x2 + x1) / 2.0;
        *yc = m2 * (*xc - mx2) + my2;
    }
    else if (std::fabs(y3-y2) <= tolerance)
    {
        m1 = - (x2-x1) / (y2-y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        *xc = (x3 + x2) / 2.0;
        *yc = m1 * (*xc - mx1) + my1;
    }
    else
    {
        m1 = - (x2-x1) / (y2-y1);
        m2 = - (x3-x2) / (y3-y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        *xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        *yc = m1 * (*xc - mx1) + my1;
    }

    dx = x2 - *xc;
    dy = y2 - *yc;
    rsqr = dx*dx + dy*dy;
    *r = sqrt(rsqr);

    dx = xp - *xc;
    dy = yp - *yc;
    drsqr = dx*dx + dy*dy;

    return (drsqr <= rsqr);
} /* CircumCircle */
