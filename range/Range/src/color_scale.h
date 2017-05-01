/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   color_scale.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   3-rd May 2013                                            *
 *                                                                   *
 *  DESCRIPTION: Color scale class declaration                       *
 *********************************************************************/

#ifndef __COLOR_SCALE_H__
#define __COLOR_SCALE_H__

#include <QObject>
#include <QList>

class ColorScale : public QObject
{

    Q_OBJECT

    protected:

        //! Scale name.
        QString scaleName;

    public:

        //! Constructor.
        explicit ColorScale(const QString &scaleName = QString(), QObject *parent = 0);

        //! Get color scale name.
        const QString & getName(void) const;

        //! Set color scale name.
        void setName(const QString &scaleName);

        //! Validate color scale.
        static bool validate(const QString &scaleName);

        //! Return list of possible color scales.
        static QList<QString> getColorScales(void);

        //! Return default color scale.
        static QString getDefaultColorScale(void);
        
    signals:

        //! Scale has changed.
        void changed(const QString &scaleName);
        
    public slots:
        
};

#endif // __COLOR_SCALE_H__
