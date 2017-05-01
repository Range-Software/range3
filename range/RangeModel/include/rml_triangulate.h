/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_triangulate.h                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th May 2015                                            *
 *                                                                   *
 *  DESCRIPTION: Triangulate functions                               *
 *********************************************************************/

#ifndef __RML_TRIANGULATE_H__
#define __RML_TRIANGULATE_H__

#include <vector>

#include "rml_node.h"
#include "rml_element.h"

std::vector<RElement> RTriangulateNodes(const std::vector<RNode> &nodes, bool removeZeroSized, double zeroSize = RConstants::eps);

#endif /* __RML_TRIANGULATE_H__ */
