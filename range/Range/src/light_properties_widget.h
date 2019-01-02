/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   light_properties_widget.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   29-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Light properties widget class declaration           *
 *********************************************************************/

#ifndef LIGHT_PROPERTIES_WIDGET_H
#define LIGHT_PROPERTIES_WIDGET_H

#include <QGroupBox>

#include <rblib.h>

class LightPropertiesWidget : public QGroupBox
{

    Q_OBJECT

    protected:

        //! OpenGL light.
        RGLLight glLight;
        //! Widget ID.
        uint id;

    public:

        //! Constructor.
        explicit LightPropertiesWidget(const QString &title, const RGLLight &glLight, QWidget *parent = nullptr);

        //! Return const reference to OpenGL light.
        const RGLLight &getLight(void) const;

        //! Return ID.
        inline uint getID(void) const
        {
            return this->id;
        }

        //! Set ID.
        inline void setID(uint id)
        {
            this->id = id;
        }
        
    signals:

        //! Light changed cignal.
        void lightChanged(const RGLLight &light);

        //! Light changed cignal.
        void lightChanged(const RGLLight &light, uint id);
        
    private slots:

        //! Light toggle button was toggled.
        void lightToggled(bool enabled);

        //! Ambient color has changed.
        void ambientColorChanged(const QColor &color);

        //! Diffuse color has changed.
        void diffuseColorChanged(const QColor &color);

        //! Specular color has changed.
        void specularColorChanged(const QColor &color);

        //! Light position has changed.
        void positionChanged(const RR3Vector &position);

        //! Light direction has changed.
        void directionChanged(const RR3Vector &direction);
        
};

#endif // LIGHT_PROPERTIES_WIDGET_H
