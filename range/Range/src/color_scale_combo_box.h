/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   color_scale_combo_box.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   3-rd May 2013                                            *
 *                                                                   *
 *  DESCRIPTION: Color scale combo box class declaration             *
 *********************************************************************/

#ifndef __COLOR_SCALE_COMBO_BOX_H__
#define __COLOR_SCALE_COMBO_BOX_H__

#include <QComboBox>

class ColorScaleComboBox : public QComboBox
{

    Q_OBJECT

    protected:

        //! Width to height factor.
        int whFactor;

    public:

        //! Constructor.
        explicit ColorScaleComboBox(QWidget *parent = 0);

        //! Set color scale by name.
        void setColorScaleName(const QString &colorScaleName);

    protected:

        //! Create icon from color scale name.
        QIcon createIcon(const QString &colorScaleName);
        
};

#endif // __COLOR_SCALE_COMBO_BOX_H__
