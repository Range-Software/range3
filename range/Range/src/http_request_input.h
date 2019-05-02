/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   http_request_input.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: HTTP request input class declaration                *
 *********************************************************************/

#ifndef HTTP_REQUEST_INPUT_H
#define HTTP_REQUEST_INPUT_H

#include <QString>
#include <QMap>

class HttpRequestInput
{

    public:

        enum VariableLayout
        {
            NOT_SET = 0,
            ADDRESS,
            URL_ENCODED,
            MULTIPART
        };

        struct FileElement
        {
            QString variableName;
            QString localFilename;
            QString requestFilename;
            QString mimeType;
        };

    protected:

        QString urlStr;
        QString httpMethod;
        VariableLayout variableLayout;
        QMap<QString, QString> variables;
        QList<FileElement> files;

    private:

        //! Internal initialization function.
        void _init(const HttpRequestInput *pHttpRequestInput = nullptr);

    public:

        //! Constructor.
        explicit HttpRequestInput(QString urlStr, QString httpMethod);

        //! Copy constructor.
        HttpRequestInput(const HttpRequestInput &httpRequestInput);

        //! Destructor.
        ~HttpRequestInput();

        //! Assignment operator.
        HttpRequestInput &operator =(const HttpRequestInput &httpRequestInput);

        //! Return const reference to url string.
        const QString &getUrlStr(void) const;

        //! Return reference to url string.
        QString &getUrlStr(void);

        //! Set url string.
        void setUrlStr(const QString &urlStr);

        //! Return const reference to http method string.
        const QString &getHttpMethod(void) const;

        //! Return variable layout.
        VariableLayout getVariableLayout(void) const;

        //! Set variable layout.
        void setVariableLayout(VariableLayout variableLayout);

        //! Return const reference to variable map.
        const QMap<QString, QString> &getVariableMap(void) const;

        //! Return reference to variable map.
        QMap<QString, QString> &getVariableMap(void);

        //! Return const reference to file list.
        const QList<HttpRequestInput::FileElement> &getFileList(void) const;

        //! Return reference to file list.
        QList<HttpRequestInput::FileElement> &getFileList(void);

        void addVariable(QString key, QString value);

        void addFile(QString variable_name, QString local_filename, QString request_filename, QString mime_type);

};

#endif // HTTP_REQUEST_INPUT_H
