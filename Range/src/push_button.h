/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   push_button.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Custom push button class declaration                *
 *********************************************************************/

#ifndef __PUSH_BUTTON_H__
#define __PUSH_BUTTON_H__

#include <QPushButton>

class PushButton : public QPushButton
{

    Q_OBJECT

    private:

        //! Push button ID.
        uint id;

    public:

        //! Constructor.
        explicit PushButton(uint id, const QString &text, QWidget *parent = 0);
        
    signals:
        
        //! Push button was clicked.
        void clicked(int id);

    private slots:

        //! Capture clicked signal.
        void onClicked(void);
        
};

#endif /* __PUSH_BUTTON_H__ */
