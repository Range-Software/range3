/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_file_manager.h                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: File manager class declarations                     *
 *********************************************************************/

#ifndef RML_FILE_MANAGER_H
#define RML_FILE_MANAGER_H

#include <QString>

#include <vector>

#include <rblib.h>

class RFileManager
{

    private:

        //! Private constructor.
        explicit RFileManager() {}

    public:

        //! Remove file.
        static void remove ( const QString &fileName );

        //! Rename file.
        static void rename ( const QString &srcFileName,
                             const QString &dstFileName );

        //! List directory content.
        //! Directory path must contain directory separator at the end.
        static unsigned int listDirectory ( const QString    &directory,
                                            QVector<QString> &directories,
                                            QVector<QString> &files);

        //! Return file extension.
        static QString getExtension ( const QString &fileName );

        //! Return file name without an extension.
        static QString removeExtension ( const QString &fileName );

        //! Build path.
        static QString buildPath ( const QString &dirName, const QString &fileName );

        //! Create file name containing time step.
        static QString getFileNameWithTimeStep ( const QString &fileName,
                                                 unsigned int   timeStepNumber,
                                                 unsigned int   numberWidth = 5 );

        //! Create file name containing time step.
        static QString getFileNameWithOutTimeStep ( const QString &fileName,
                                                    unsigned int   numberWidth = 5 );

        //! Create file name containing suffix (basename-suffix.extension).
        static QString getFileNameWithSuffix ( const QString &fileName,
                                               const QString &suffix );

        //! Return file suffix.
        static QString getSuffix ( const QString &fileName );

        //! Check if file exists.
        static bool fileExists ( const QString &fileName );

        //! Find last file
        static QString findLastFile ( const QString &filePattern );

        //! Find files
        static QStringList findFiles ( const QString &filePattern );

        //! Write link file.
        static void writeLink(const QString &linkFileName, const QString &targetFileName, const RVersion &version, bool binary = true);

        //! Find link target file name.
        static QString findLinkTargetFileName(const QString &linkFileName, const QString &targetFileName);

};

#endif /* RML_FILE_MANAGER_H */
