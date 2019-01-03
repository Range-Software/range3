/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_functions.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   3-rd March 2016                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL functions declarations                       *
 *********************************************************************/

#ifndef GL_FUNCTIONS_H
#define GL_FUNCTIONS_H

#include <rblib.h>

#include <qglfunctions.h>

#define GL_SAFE_CALL(__safe_call) { __safe_call; GLFunctions::printError(#__safe_call,__FILE__,__LINE__); }

namespace GLFunctions
{

    //! Wrapper for glBegin().
    void begin(GLenum  mode);

    //! Wrapper for glEnd().
    void end();

    void printError(const char *command, const char *file, unsigned int line);

}

#endif // GL_FUNCTIONS_H
