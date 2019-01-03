/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_file.h                                               *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: File class declaration                              *
 *********************************************************************/

#ifndef RML_FILE_H
#define RML_FILE_H

#include <QFile>
#include <QTextStream>

#include <rblib.h>

class RFile : public QFile
{

    Q_OBJECT

    public:

        //! File type.
        enum Type
        {
            ASCII  = 0,
            BINARY = 1
        };

    protected:

        //! File name.
        QString fileName;
        //! File type.
        Type type;
        //! File version.
        RVersion fileVersion;

        //! Text stream.
        QTextStream textStream;

    private:

        //! Internal initialization function.
        void _init(const RFile *pFile = nullptr);

    public:

        //! Constructor.
        explicit RFile(const QString &fileName, Type type);

        //! Copy constructor.
        RFile(const RFile &file);

        //! Destructor.
        ~RFile();

    private:

        //! Assignment operator.
        RFile &operator =(const RFile &file);

    public:

        //! Return const reference file version.
        const RVersion &getVersion(void) const;

        //! Return const reference file version.
        void setVersion(const RVersion &fileVersion);

        //! Return reference to text stream.
        QTextStream &getTextStream(void);

        //! Return stream status text.
        static QString getTextStreamStatusStringt(QTextStream::Status status);

};

#endif // RML_FILE_H
