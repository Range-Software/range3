/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rmatrixmanager.h                                         *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th October 2017                                       *
 *                                                                   *
 *  DESCRIPTION: Matrix manager class declaration                    *
 *********************************************************************/

#ifndef __RMATRIXMANAGER_H__
#define __RMATRIXMANAGER_H__

#include <rmlib.h>

template <class T>
class RMatrixManager
{

    protected:

        // List of matrix containers.
        std::vector<T> c;

    public:

        //! Constructor.
        RMatrixManager()
        {
            this->c.resize(R_ELEMENT_N_TYPES);
        }

        //! Return container for given element type.
        T &getMatricies(RElementType type)
        {
            if (!this->c[type].initialized)
            {
                this->c[type].resize(RElement::getNNodes(type));
            }
            return this->c[type];
        }

};

#endif // __RMATRIXMANAGER_H__
