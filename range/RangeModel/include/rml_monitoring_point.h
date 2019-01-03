/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_monitoring_point.h                                   *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th March 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Monitoring point class declaration                  *
 *********************************************************************/

#ifndef RML_MONITORING_POINT_H
#define RML_MONITORING_POINT_H

#include <rblib.h>

#include "rml_file.h"
#include "rml_variable.h"

class RMonitoringPoint
{

    protected:

        //! Variable type.
        RVariableType variableType;
        //! Monitoring point position.
        RR3Vector position;
        // FOLOWING MEMBERS ARE NOT SAVED TO FILE
        //! Output file name.
        QString outputFileName;

    private:

        //! Internal initialization function.
        void _init(const RMonitoringPoint *pMonitoringPoint = nullptr);

    public:

        //! Constructor.
        RMonitoringPoint();

        //! Constructor.
        RMonitoringPoint(RVariableType variableType, const RR3Vector &position);

        //! Copy constructor.
        RMonitoringPoint(const RMonitoringPoint &monitoringPoint);

        //! Destructor.
        ~RMonitoringPoint();

        //! Assignment operator.
        RMonitoringPoint & operator =(const RMonitoringPoint &monitoringPoint);

        //! Comparison operator.
        bool operator ==(const RMonitoringPoint &monitoringPoint) const;

        //! Less than operator.
        bool operator <(const RMonitoringPoint &monitoringPoint) const;

        //! Return variable type.
        RVariableType getVariableType(void) const;

        //! Set variable type.
        void setVariableType(RVariableType variableType);

        //! Return const reference to position.
        const RR3Vector & getPosition(void) const;

        //! Set position.
        void setPosition(const RR3Vector &position);

        //! Return output file name.
        const QString & getOutputFileName(void) const;

        //! Set output file name.
        void setOutputFileName(const QString &outputFileName);

        //! Write current values to file.
        void writeValueVectorToFile(unsigned int iteration, const RValueVector &valueVector) const;

        //! Read monitoring point record from file.
        void readRecordFromFile(RFile &in, unsigned int &iteration, RRVector &valueVector);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_MONITORING_POINT_H
