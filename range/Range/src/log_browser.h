/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   log_browser.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th August 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Log browser class declaration                       *
 *********************************************************************/

#ifndef LOG_BROWSER_H
#define LOG_BROWSER_H

#include <QTextBrowser>

class LogBrowser : public QTextBrowser
{

    Q_OBJECT

    protected:

        //! Log file name.
        const QString logFileName;
        //! End stream position.
        qint64 fileStreamEnd;

    public:

        //! Constructor.
        explicit LogBrowser(const QString &logFileName, QWidget *parent = nullptr);

        //! Clear log.
        void clearLog(void);
        
    signals:
        
    private slots:

        //! File changed signal.
        void onFileChanged(const QString &path);
        
};

#endif // LOG_BROWSER_H
