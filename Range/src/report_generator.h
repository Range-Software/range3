/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   report_generator.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th February 2014                                      *
 *                                                                   *
 *  DESCRIPTION: Report generator class declaration                  *
 *********************************************************************/

#ifndef __REPORT_GENERATOR_H__
#define __REPORT_GENERATOR_H__

#include <QObject>
#include <QMap>
#include <QTextDocument>
#include <QPrinter>
#include <QTextCharFormat>

#include "document_generator.h"

class ReportGenerator : public DocumentGenerator
{
    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;

    public:

        //! Constructor.
        explicit ReportGenerator(uint modelID, QObject *parent = 0);

    protected:

        //! Generate title.
        void generateTitle(void);

        //! Generate body.
        void generateBody(void);

        //! Generate model chapter.
        void generateModelChapter(void);

        //! Generate problem chapter.
        void generateProblemChapter(void);

        //! Generate results chapter.
        void generateResultsChapter(void);

        //! Take model screenshot.
        QImage takeModelScreenshot(void) const;

};

#endif // __REPORT_GENERATOR_H__
