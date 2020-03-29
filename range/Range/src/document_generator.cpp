/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   document_generator.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   3-rd May 2016                                            *
 *                                                                   *
 *  DESCRIPTION: Document generator class definition                 *
 *********************************************************************/

#include <QTextDocument>
#include <QTextCursor>
#include <QtPrintSupport/QPrinter>
#include <QTextDocumentWriter>
#include <QTextTable>
#include <QHostInfo>
#include <QPainter>

#include <rblib.h>

#include "document_generator.h"

DocumentGenerator::DocumentGenerator(QObject *parent)
    : Job(parent)
    , enableHeaderCounters(true)
    , fontSizeScale(1.0)
{
    this->docTitle = new QTextDocument(this);
    this->docHeader = new QTextDocument(this);
    this->docFooter = new QTextDocument(this);
    this->docBody = new QTextDocument(this);
    this->printer = new QPrinter(QPrinter::ScreenResolution);
    this->printer->setPageSize(QPrinter::A4);
    this->printer->setPageMargins(20.0,15.0,20.0,10.0,QPrinter::Millimeter);
}

void DocumentGenerator::setEnableHeaderCounters(bool enableHeaderCounters)
{
    this->enableHeaderCounters = enableHeaderCounters;
}

void DocumentGenerator::setFontSizeScale(double fontSizeScale)
{
    this->fontSizeScale = fontSizeScale;
}

void DocumentGenerator::addExportType(DocumentGenerator::Type type, const QString &fileName)
{
    this->exportTypes[type] = fileName;
}

void DocumentGenerator::run(void)
{
    RLogger::info("Generating document\n");
    RLogger::indent();

    try
    {
        this->generate();

        QMap<DocumentGenerator::Type,QString>::iterator iter;
        for (iter = this->exportTypes.begin(); iter != this->exportTypes.end(); ++iter)
        {
            RLogger::info("Exporting document to '%s'\n",iter.value().toUtf8().constData());
            switch (iter.key())
            {
                case DocumentGenerator::PlainText:
                {
                    this->exportToPlainText(iter.value());
                    break;
                }
                case DocumentGenerator::HTML:
                {
                    this->exportToHTML(iter.value());
                    break;
                }
                case DocumentGenerator::ODF:
                {
                    this->exportToODF(iter.value());
                    break;
                }
                case DocumentGenerator::PDF:
                {
                    this->exportToPDF(iter.value());
                    break;
                }
            }
        }
        RLogger::notice("Documents have been generated.");
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to generate document: %s\n",error.getMessage().toUtf8().constData());
        RLogger::unindent();
    }

    RLogger::unindent();
}

void DocumentGenerator::generate(void)
{
    RLogger::info("Generating document.\n");
    RLogger::indent();

    this->docHeader->setPageSize(this->printer->pageRect().size());

    this->generateHeader();

    this->docFooter->setPageSize(this->printer->pageRect().size());

    this->generateFooter();

    this->docTitle->setPageSize(this->printer->pageRect().size());
    // Calculating the main document size for one page
    QSizeF centerSize(this->printer->pageRect().width(),
                      this->printer->pageRect().height() - this->docHeader->size().toSize().height() - this->docFooter->size().toSize().height());
    this->docBody->setPageSize(centerSize);
    this->docBody->setIndentWidth(10.0);

    this->generateTitle();
    this->generateBody();

    RLogger::unindent();
}

void DocumentGenerator::generateHeader(void)
{
    QTextImageFormat imageFormat;
    imageFormat.setName(":/icons/logos/pixmaps/range-logo-128.png");
    imageFormat.setHeight(64.0);
    imageFormat.setWidth(64.0);

    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(5);
    tableFormat.setBorder(0.0);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    QTextCursor cursor(this->docHeader);
    cursor.insertTable(1,2,tableFormat);

    cursor.insertImage(imageFormat,QTextFrameFormat::FloatLeft);
    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,RVendor::name,true);
    cursor.insertBlock(QTextBlockFormat(),QTextCharFormat());
    this->insertText(cursor,RVendor::title,true);
    cursor.insertBlock(QTextBlockFormat(),QTextCharFormat());
    this->insertText(cursor,RVendor::www,true);
    cursor.movePosition(QTextCursor::End);
    cursor.insertHtml("<hr noshade/>");
}

void DocumentGenerator::generateFooter(void)
{
    QTextCursor cursor(this->docFooter);
    cursor.insertHtml(QString("<center><b>") + RVendor::name + QString("</b> - ") + RVendor::author.toHtmlEscaped() + QString(" &copy; ") + QString::number(RVendor::year) + "</center>");
    cursor.insertBlock(QTextBlockFormat(),QTextCharFormat());
}

void DocumentGenerator::exportToPlainText(const QString &fileName) const
{
    QTextDocumentWriter writer(fileName);

    writer.setFormat("plaintext");
    if (!writer.write(this->docBody))
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF, "Failed to write to file \'%s\'.",fileName.toUtf8().constData());
    }
}

void DocumentGenerator::exportToHTML(const QString &fileName) const
{
    QTextDocumentWriter writer(fileName);

    writer.setFormat("HTML");
    if (!writer.write(this->docBody))
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF, "Failed to write to file \'%s\'.",fileName.toUtf8().constData());
    }
}

void DocumentGenerator::exportToODF(const QString &fileName) const
{
    QTextDocumentWriter writer(fileName);

    writer.setFormat("ODF");
    if (!writer.write(this->docBody))
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF, "Failed to write to file \'%s\'.",fileName.toUtf8().constData());
    }
}

void DocumentGenerator::exportToPDF(const QString &fileName) const
{
    this->printer->setOutputFormat(QPrinter::PdfFormat);
    this->printer->setOutputFileName(fileName);

    // Setting up the rectangles for each section.
    QRect titleRect = QRect(QPoint(0,0), this->docTitle->size().toSize());
    QRect headerRect = QRect(QPoint(0,0), this->docHeader->size().toSize());
    QRect footerRect = QRect(QPoint(0,0), this->docFooter->size().toSize());
    QRect contentRect = QRect(QPoint(0,0), this->docBody->size().toSize());    // Main content rectangle.
    QRect currentRect = QRect(QPoint(0,0), this->docBody->pageSize().toSize());        // Current main content rectangle.

    QPainter painter(this->printer);

    // Drawing the title
    painter.resetTransform();
    painter.translate(0, this->docTitle->pageSize().height()/2.0 - titleRect.height());
    this->docTitle->drawContents(&painter,titleRect);
    painter.resetTransform();
    // Drawing the header on the top of the page
    this->docHeader->drawContents(&painter, headerRect);

    this->printer->newPage();

    // Loop if the current content rectangle intersects with the main content rectangle.
    uint nPages = 0;
    while (currentRect.intersects(contentRect))
    {
        // Resetting the painter matrix co ordinate system.
        painter.resetTransform();
        // Applying negative translation of painter co-ordinate system by current main content rectangle top y coordinate.
        painter.translate(0, -currentRect.y());
        // Applying positive translation of painter co-ordinate system by header hight.
        painter.translate(0, headerRect.height());
        // Drawing the center content for current page.
        this->docBody->drawContents(&painter, currentRect);
        // Resetting the painter matrix co ordinate system.
        painter.resetTransform();
        // Drawing the header on the top of the page
        this->docHeader->drawContents(&painter, headerRect);
        // Applying positive translation of painter co-ordinate system to draw the footer
        painter.translate(0, headerRect.height());
        painter.translate(0, this->docBody->pageSize().height());
        this->docFooter->drawContents(&painter, footerRect);

        // Translating the current rectangle to the area to be printed for the next page
        currentRect.translate(0, currentRect.height());

        painter.drawText(footerRect, Qt::AlignBottom | Qt::AlignCenter, QObject::tr("Page %1").arg(nPages+1));

        // Inserting a new page if there is till area left to be printed
        if (currentRect.intersects(contentRect))
        {
            this->printer->newPage();
        }
        nPages++;
    }
}

void DocumentGenerator::insertH1(QTextCursor &cursor, const QString &text, bool useCurrentBlockFormat)
{
    if (!useCurrentBlockFormat)
    {
        QTextBlockFormat blockFormat;
        blockFormat.setIndent(0);
        blockFormat.setAlignment(Qt::AlignLeft);
        blockFormat.setTopMargin(10.0);
        blockFormat.setBottomMargin(5.0);
        cursor.insertBlock(blockFormat,QTextCharFormat());
    }
    QTextCharFormat textFormat = DocumentGenerator::getH1Format();
    textFormat.setFontPointSize(textFormat.fontPointSize() * this->fontSizeScale);
    if (this->enableHeaderCounters)
    {
        cursor.insertText(QString::number(++this->headerCounters.nH1) + " " + text, textFormat);
    }
    else
    {
        cursor.insertText(text, textFormat);
    }
}

void DocumentGenerator::insertH2(QTextCursor &cursor, const QString &text, bool useCurrentBlockFormat)
{
    if (!useCurrentBlockFormat)
    {
        QTextBlockFormat blockFormat;
        blockFormat.setIndent(0);
        blockFormat.setAlignment(Qt::AlignLeft);
        blockFormat.setTopMargin(10.0);
        blockFormat.setBottomMargin(5.0);
        cursor.insertBlock(blockFormat,QTextCharFormat());
    }
    QTextCharFormat textFormat = DocumentGenerator::getH2Format();
    textFormat.setFontPointSize(textFormat.fontPointSize() * this->fontSizeScale);
    if (this->enableHeaderCounters)
    {
        cursor.insertText(QString::number(this->headerCounters.nH1) +
                          "." +
                          QString::number(++this->headerCounters.nH2) +
                          " " + text,
                          textFormat);
    }
    else
    {
        cursor.insertText(text, textFormat);
    }
}

void DocumentGenerator::insertH3(QTextCursor &cursor, const QString &text, bool useCurrentBlockFormat)
{
    if (!useCurrentBlockFormat)
    {
        QTextBlockFormat blockFormat;
        blockFormat.setIndent(1);
        blockFormat.setAlignment(Qt::AlignLeft);
        blockFormat.setTopMargin(5.0);
        blockFormat.setBottomMargin(2.0);
        cursor.insertBlock(blockFormat,QTextCharFormat());
    }
    QTextCharFormat textFormat = DocumentGenerator::getH3Format();
    textFormat.setFontPointSize(textFormat.fontPointSize() * this->fontSizeScale);
    if (this->enableHeaderCounters)
    {
        cursor.insertText(QString::number(this->headerCounters.nH1) +
                          "." +
                          QString::number(this->headerCounters.nH2) +
                          "." +
                          QString::number(++this->headerCounters.nH3) +
                          " " +
                          text,
                          textFormat);
    }
    else
    {
        cursor.insertText(text, textFormat);
    }
}

void DocumentGenerator::insertH4(QTextCursor &cursor, const QString &text, bool useCurrentBlockFormat)
{
    if (!useCurrentBlockFormat)
    {
        QTextBlockFormat blockFormat;
        blockFormat.setIndent(2);
        blockFormat.setAlignment(Qt::AlignLeft);
        cursor.insertBlock(blockFormat,QTextCharFormat());
    }
    QTextCharFormat textFormat = DocumentGenerator::getH4Format();
    textFormat.setFontPointSize(textFormat.fontPointSize() * this->fontSizeScale);
    if (this->enableHeaderCounters)
    {
        cursor.insertText(QString::number(this->headerCounters.nH1) +
                          "." +
                          QString::number(this->headerCounters.nH2) +
                          "." +
                          QString::number(this->headerCounters.nH3) +
                          "." +
                          QString::number(++this->headerCounters.nH4) +
                          " " +
                          text,
                          textFormat);
    }
    else
    {
        cursor.insertText(text, textFormat);
    }
}

void DocumentGenerator::insertH5(QTextCursor &cursor, const QString &text, bool useCurrentBlockFormat)
{
    if (!useCurrentBlockFormat)
    {
        QTextBlockFormat blockFormat;
        blockFormat.setIndent(3);
        blockFormat.setAlignment(Qt::AlignLeft);
        cursor.insertBlock(blockFormat,QTextCharFormat());
    }
    QTextCharFormat textFormat = DocumentGenerator::getH5Format();
    textFormat.setFontPointSize(textFormat.fontPointSize() * this->fontSizeScale);
    if (this->enableHeaderCounters)
    {
        cursor.insertText(QString::number(this->headerCounters.nH1) +
                          "." +
                          QString::number(this->headerCounters.nH2) +
                          "." +
                          QString::number(this->headerCounters.nH3) +
                          "." +
                          QString::number(this->headerCounters.nH4) +
                          "." +
                          QString::number(++this->headerCounters.nH5) +
                          " " +
                          text,
                          textFormat);
    }
    else
    {
        cursor.insertText(text, textFormat);
    }
}

void DocumentGenerator::insertText(QTextCursor &cursor, const QString &text, bool useCurrentBlockFormat)
{
    if (!useCurrentBlockFormat)
    {
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignLeft);
        cursor.insertBlock(blockFormat,QTextCharFormat());
    }
    QTextCharFormat textFormat = DocumentGenerator::getTextFormat();
    textFormat.setFontPointSize(textFormat.fontPointSize() * this->fontSizeScale);
    cursor.insertText(text,textFormat);
}

void DocumentGenerator::insertBold(QTextCursor &cursor, const QString &text, bool useCurrentBlockFormat)
{
    if (!useCurrentBlockFormat)
    {
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignLeft);
        cursor.insertBlock(blockFormat,QTextCharFormat());
    }
    QTextCharFormat textFormat = DocumentGenerator::getBoldFormat();
    textFormat.setFontPointSize(textFormat.fontPointSize() * this->fontSizeScale);
    cursor.insertText(text,textFormat);
}

QTextTableFormat DocumentGenerator::getTableFormat(void)
{
    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(0.2);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setAlignment(Qt::AlignCenter);
    return tableFormat;
}

QTextBlockFormat DocumentGenerator::getBlockFormat(void)
{
    QTextBlockFormat blockFormat;
    blockFormat.setIndent(0);
    blockFormat.setAlignment(Qt::AlignLeft);
    return blockFormat;
}

QTextCharFormat DocumentGenerator::getH1Format(void)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(18.0);
    fmt.setFontCapitalization(QFont::AllUppercase);
    fmt.setFontWeight(QFont::Bold);
    return fmt;
}

QTextCharFormat DocumentGenerator::getH2Format(void)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(16.0);
    fmt.setFontWeight(QFont::Bold);
    return fmt;
}

QTextCharFormat DocumentGenerator::getH3Format(void)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(16.0);
    fmt.setFontWeight(QFont::Bold);
    return fmt;
}

QTextCharFormat DocumentGenerator::getH4Format(void)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(12.0);
    fmt.setFontWeight(QFont::Bold);
    return fmt;
}

QTextCharFormat DocumentGenerator::getH5Format(void)
{

    QTextCharFormat fmt;
    fmt.setFontPointSize(12.0);
    fmt.setFontUnderline(true);
    return fmt;
}

QTextCharFormat DocumentGenerator::getTextFormat(void)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(10.0);
    return fmt;
}

QTextCharFormat DocumentGenerator::getBoldFormat(void)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(10.0);
    fmt.setFontWeight(QFont::Bold);
    return fmt;
}
