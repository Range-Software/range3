/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_local_diraction.h                                    *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   23-rd May 2019                                           *
 *                                                                   *
 *  DESCRIPTION: Local direction class declaration                   *
 *********************************************************************/

#ifndef RBL_LOCAL_DIRECTION_H
#define RBL_LOCAL_DIRECTION_H

#include "rbl_r3vector.h"

class RLocalDirection
{

    protected:

        //! Position.
        RR3Vector position;
        //! Direction.
        RR3Vector direction;

    private:

        //! Internal initialization function.
        void _init(const RLocalDirection *pLocalDirection = nullptr);

    public:

        //! Constructor.
        RLocalDirection();

        //! Constructor.
        RLocalDirection(const RR3Vector &position, const RR3Vector &direction);

        //! Copy constructor.
        RLocalDirection(const RLocalDirection &rLocalDirection);

        //! Destructor.
        ~RLocalDirection() = default;

        //! Assignment operator.
        RLocalDirection &operator =(const RLocalDirection &rLocalDirection);

        //! Return const reference position.
        const RR3Vector &getPosition() const;

        //! Return const reference direction.
        const RR3Vector &getDirection() const;

};

#endif // RBL_LOCAL_DIRECTION_H
