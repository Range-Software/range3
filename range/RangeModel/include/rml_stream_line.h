/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_stream_line.h                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Stream line class declaration                       *
 *********************************************************************/

#ifndef __RML_STREAM_LINE_H__
#define __RML_STREAM_LINE_H__

#include <vector>

#include "rml_variable.h"
#include "rml_interpolated_entity.h"

class RStreamLine : public RInterpolatedEntity
{

    public:

        //! Default entity name.
        const static QString defaultName;

    protected:

        //! Variable type.
        RVariableType variableType;
        //! Start position.
        RR3Vector position;

    private:

        //! Internal initialization function.
        void _init ( const RStreamLine *pStreamLine = 0 );

    public:

        //! Constructor.
        RStreamLine();

        //! Copy constructor.
        RStreamLine (const RStreamLine &streamLine);

        //! Destructor.
        ~RStreamLine ();

        //! Assignment operator.
        RStreamLine & operator = ( const RStreamLine &streamLine );

        //! Return variable type.
        RVariableType getVariableType ( void ) const;

        //! Set variable type.
        void setVariableType ( RVariableType variableType );

        //! Return const reference to position vector.
        const RR3Vector & getPosition ( void ) const;

        //! Set position vector.
        void setPosition ( const RR3Vector &position );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* __RML_STREAM_LINE_H__ */
