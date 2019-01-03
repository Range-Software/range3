/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_limit_box.h                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Limit box class declaration                         *
 *********************************************************************/

#ifndef RBL_BOX_H
#define RBL_BOX_H

#include "rbl_r3vector.h"

class RLimitBox
{

    protected:

        //! X limits.
        double xl, xu;
        //! Y limits.
        double yl, yu;
        //! Z limits.
        double zl, zu;

    private:

        //! Internal initialization function.
        void _init(const RLimitBox *pBox = nullptr);

    public:

        //! Constructor.
        RLimitBox();

        //! Constructor.
        RLimitBox(double xl, double xu, double yl, double yu, double zl, double zu);

        //! Copy constructor.
        RLimitBox(const RLimitBox &box);

        //! Destructor.
        ~RLimitBox();

        //! Assignment operator.
        RLimitBox &operator =(const RLimitBox &box);

        //! Set limits.
        void setLimits(double xl, double xu, double yl, double yu, double zl, double zu);

        //! Scale limit box by factor.
        void scale(double scaleFactor);

        //! Print limit box.
        void print(void) const;

        //! Return true if two boxes intersect.
        static bool areIntersecting(const RLimitBox &b1, const RLimitBox &b2);

    protected:

        //! Fix/sort limits.
        void fixLimits(void);

};

#endif // RBL_BOX_H
