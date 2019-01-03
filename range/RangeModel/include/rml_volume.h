/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_volume.h                                             *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Volume class declaration                            *
 *********************************************************************/

#ifndef RML_VOLUME_H
#define RML_VOLUME_H

#include <vector>

#include "rml_element.h"
#include "rml_element_group.h"

//! Volume element group class
class RVolume : public RElementGroup
{

    public:

        //! Default entity name.
        const static QString defaultName;

    private:

        //! Internal initialization function.
        void _init ( const RVolume *pVolume = nullptr );

    public:

        //! Constructor.
        RVolume ();

        //! Copy constructor.
        RVolume (const RVolume &volume);

        //! Destructor.
        ~RVolume ();

        //! Find volume volume.
        double findVolume(const std::vector<RNode> &nodes, const std::vector<RElement> &elements) const;

        //! Assignment operator.
        RVolume & operator = ( const RVolume &volume );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_VOLUME_H */
