/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   convergence_graph_object.h                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th March 2014                                          *
 *                                                                   *
 *  DESCRIPTION: Convergence graph object class declaration          *
 *********************************************************************/

#ifndef CONVERGENCE_GRAPH_OBJECT_H
#define CONVERGENCE_GRAPH_OBJECT_H

#include "graph_object.h"

class ConvergenceGraphObject : public GraphObject
{

    Q_OBJECT

    protected:

        //! Source file.
        QString sourceFileName;
        //! Data column.
        uint dataColumn;

    public:

        //! Constructor.
        explicit ConvergenceGraphObject(QObject *parent = nullptr);

        //! Return const reference to source file name.
        const QString & getSourceFileName(void) const;

        //! Set source file.
        void setSourceFileName(const QString &sourceFileName, bool loadData = true);

        //! Set data column.
        void setDataColumn(uint dataColumn, bool loadData = true);

        //! Return true if source was set.
        bool isSourceSet(void) const;

        //! Clear source.
        void clearSourceFile(void);

        //! Find data columns.
        static QStringList findDataColumns(const QString &sourceFileName);

    protected:

        //! Read data from source.
        void readSource(void);

};

#endif // CONVERGENCE_GRAPH_OBJECT_H
