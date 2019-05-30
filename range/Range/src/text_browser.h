/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_browser.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Text browser class declaration                      *
 *********************************************************************/

#ifndef TEXT_BROWSER_H
#define TEXT_BROWSER_H

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
        explicit TextBrowser(bool hasClearButton, QWidget *parent = nullptr);

    protected:

        //! Resize event.
        virtual void resizeEvent(QResizeEvent *event) override;
        
};

#endif // TEXT_BROWSER_H
