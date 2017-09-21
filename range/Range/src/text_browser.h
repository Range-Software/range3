/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_browser.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Text browser class declaration                      *
 *********************************************************************/

#ifndef __TEXT_BROWSER_H__
#define __TEXT_BROWSER_H__

#include <QTextBrowser>
#include <QPushButton>

class TextBrowser : public QTextBrowser
{

    Q_OBJECT

    protected:

        //! Clear button enabled.
        bool hasClearButton;
        //! Clear button.
        QPushButton *clearButton;

    public:

        //! Constructor.
        explicit TextBrowser(bool hasClearButton, QWidget *parent = 0);

    protected:

        //! Resize event.
        virtual void resizeEvent(QResizeEvent *event);
        
};

#endif // __TEXT_BROWSER_H__
