/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_gl_display_properties.h                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-nd February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL display properties class declaration         *
 *********************************************************************/

#ifndef RML_GL_DISPLAY_PROPERTIES_H
#define RML_GL_DISPLAY_PROPERTIES_H

#include <QVector>
#include <QColor>

#include <rblib.h>

class RGLDisplayProperties
{

    protected:

        //! Draw global axis.
        bool drawGlobalAxis;
        //! Draw local axis.
        bool drawLocalAxis;
        //! Show model edges.
        bool showModelEdges;
        //! Show model edges.
        bool showModelDimensions;
        //! Show errors.
        bool showErrors;
        //! Background color.
        QColor bgColor;
        //! Background gradient.
        bool bgGradient;
        //! OpenGL lights.
        QVector<RGLLight> lights;

    private:

        //! Internal initialization function.
        void _init(const RGLDisplayProperties *pDisplayProperties = nullptr);

    public:

        //! Constructor.
        RGLDisplayProperties();

        //! Copy constructor.
        RGLDisplayProperties(const RGLDisplayProperties &displayProperties);

        //! Destructor.
        ~RGLDisplayProperties();

        //! Assignment operator.
        RGLDisplayProperties & operator =(const RGLDisplayProperties &displayProperties);

        //! Return whether to draw global axis.
        bool getDrawGlobalAxis(void) const;

        //! Set whether to draw global axis.
        void setDrawGlobalAxis(bool drawGlobalAxis);

        //! Return whether to draw local axis.
        bool getDrawLocalAxis(void) const;

        //! Set whether to draw local axis.
        void setDrawLocalAxis(bool drawLocalAxis);

        //! Return whether to show model edges.
        bool getShowModelEdges(void) const;

        //! Set whether to show model edges.
        void setShowModelEdges(bool showModelEdges);

        //! Return whether to show model dimensions.
        bool getShowModelDimensions(void) const;

        //! Set whether to show model dimensions.
        void setShowModelDimensions(bool showModelDimensions);

        //! return whether to show errors.
        bool getShowErrors(void) const;

        //! Set whether to show errors.
        void setShowErrors(bool showErrors);

        //! Return background color.
        const QColor & getBgColor(void) const;

        //! Set background color.
        void setBgColor(const QColor &bgColor);

        //! Return background gradient.
        bool getBgGradient(void) const;

        //! Set background gradient.
        void setBgGradient(bool bgGradient);

        //! Return number of lights.
        uint getNLights(void) const;

        //! Set number of lights.
        void setNLights(uint nLights);

        //! Return const reference to a light at given position.
        const RGLLight & getLight(uint lightID) const;

        //! Return reference to a light at given position.
        RGLLight & getLight(uint lightID);

        //! Set light at given position.
        void setLight(uint lightID, const RGLLight &glLight);

        //! Read from file.
        void read(const QString &fileName);

        //! Write to file.
        void write(const QString &fileName) const;

        //! Allow RFileIO to access private members.
        friend class RFileIO;


        /*************************************************************
         * Static methods                                            *
         *************************************************************/

        //! Return default file extension.
        static QString getDefaultFileExtension(bool binary = true);

    protected:

        //! Read from the ASCII file.
        //! If file is a link target filename is returned.
        QString readAscii(const QString &fileName);

        //! Read from the binary file.
        //! If file is a link target filename is returned.
        QString readBinary(const QString &fileName);

        //! Write to the ASCII file.
        void writeAscii(const QString &fileName) const;

        //! Write to the binary file.
        void writeBinary(const QString &fileName) const;

};

#endif /* RML_GL_DISPLAY_PROPERTIES_H */
