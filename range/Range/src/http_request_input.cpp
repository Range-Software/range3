/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   http_request_worker.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: HTTP request worker class definition                *
 *********************************************************************/

#include <QDateTime>
#include <QUrl>
#include <QFileInfo>
#include <QBuffer>

#include "http_request_worker.h"

void HttpRequestInput::_init(const HttpRequestInput *pHttpRequestInput)
{
    if (pHttpRequestInput)
    {
        this->urlStr = pHttpRequestInput->urlStr;
        this->httpMethod = pHttpRequestInput->httpMethod;
        this->variableLayout = pHttpRequestInput->variableLayout;
        this->variables = pHttpRequestInput->variables;
        this->files = pHttpRequestInput->files;
    }
}

HttpRequestInput::HttpRequestInput(QString urlStr, QString httpMethod)
    : urlStr(urlStr)
    , httpMethod(httpMethod)
    , variableLayout(NOT_SET)
{
    this->_init();
}

HttpRequestInput::HttpRequestInput(const HttpRequestInput &httpRequestInput)
{
    this->_init(&httpRequestInput);
}

HttpRequestInput::~HttpRequestInput()
{

}

HttpRequestInput &HttpRequestInput::operator =(const HttpRequestInput &httpRequestInput)
{
    this->_init(&httpRequestInput);
    return (*this);
}

const QString &HttpRequestInput::getUrlStr(void) const
{
    return this->urlStr;
}

QString &HttpRequestInput::getUrlStr(void)
{
    return this->urlStr;
}

void HttpRequestInput::setUrlStr(const QString &urlStr)
{
    this->urlStr = urlStr;
}

const QString &HttpRequestInput::getHttpMethod(void) const
{
    return this->httpMethod;
}

HttpRequestInput::VariableLayout HttpRequestInput::getVariableLayout(void) const
{
    return this->variableLayout;
}

void HttpRequestInput::setVariableLayout(HttpRequestInput::VariableLayout varLayout)
{
    this->variableLayout = varLayout;
}

const QMap<QString, QString> &HttpRequestInput::getVariableMap(void) const
{
    return this->variables;
}

QMap<QString, QString> &HttpRequestInput::getVariableMap(void)
{
    return this->variables;
}

const QList<HttpRequestInput::FileElement> &HttpRequestInput::getFileList(void) const
{
    return this->files;
}

QList<HttpRequestInput::FileElement> &HttpRequestInput::getFileList(void)
{
    return this->files;
}

void HttpRequestInput::addVariable(QString key, QString value)
{
    variables[key] = value;
}

void HttpRequestInput::addFile(QString variable_name, QString local_filename, QString request_filename, QString mime_type)
{
    FileElement file;
    file.variableName = variable_name;
    file.localFilename = local_filename;
    file.requestFilename = request_filename;
    file.mimeType = mime_type;
    files.append(file);
}

