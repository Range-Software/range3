/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_line.h                                               *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Line class definition                               *
 *********************************************************************/

#ifndef RML_LINE_H
#define RML_LINE_H

#include <vector>

#include "rml_element.h"
#include "rml_element_group.h"

//! Line element group class.
class RLine : public RElementGroup
{

    public:

        //! Default entity name.
        const static QString defaultName;

    private:

        //! Internal initialization function.
        void _init ( const RLine *pLine = nullptr );

    protected:

        //! Line artificial cross area.
        double crossArea;

    public:

        //! Constructor.
        RLine ();

        //! Copy constructor.
        RLine (const RLine &line);

        //! Destructor.
        ~RLine ();

        //! Return line group artificial cross area.
        double getCrossArea ( void ) const;

        //! Set line group artificial cross area.
        void setCrossArea ( double cross_area );

        //! Find line length.
        double findLength(const std::vector<RNode> &nodes, const std::vector<RElement> &elements) const;

        //! Assignment operator.
        RLine & operator = ( const RLine &line );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_LINE_H */
