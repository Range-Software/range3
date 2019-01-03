/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_save_file.h                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   1-st January 2018                                        *
 *                                                                   *
 *  DESCRIPTION: Save file class declaration                         *
 *********************************************************************/

#ifndef RML_SAVE_FILE_H
#define RML_SAVE_FILE_H

#include <QSaveFile>
#include <QTextStream>

#include <rblib.h>

class RSaveFile : public QSaveFile
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
        void _init(const RSaveFile *pFile = nullptr);

    public:

        //! Constructor.
        explicit RSaveFile(const QString &fileName, Type type);

        //! Copy constructor.
        RSaveFile(const RSaveFile &file);

        //! Destructor.
        ~RSaveFile();

    private:

        //! Assignment operator.
        RSaveFile &operator =(const RSaveFile &file);

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

#endif // RML_SAVE_FILE_H
