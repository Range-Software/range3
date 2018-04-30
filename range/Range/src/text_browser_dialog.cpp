/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_browser_dialog.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-st September 2017                                     *
 *                                                                   *
 *  DESCRIPTION: Text browser dialog class definition                *
 *********************************************************************/

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>

#include "text_browser_dialog.h"

TextBrowserDialog::TextBrowserDialog(const QString &title,
                                     const QString &desc,
                                     const QString &text,
                                     QWidget *parent)
    : QDialog(parent)
{
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->resize(700,500);

    this->setWindowTitle(title);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel(desc);
    mainLayout->addWidget(titleLabel);

    QTextBrowser *textBrowser = new QTextBrowser(this);
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    textBrowser->setFont(font);
    textBrowser->insertPlainText(text);
    mainLayout->addWidget(textBrowser);

    QTextCursor textCursor = textBrowser->textCursor();
    textCursor.movePosition(QTextCursor::Start);
    textBrowser->setTextCursor(textCursor);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&TextBrowserDialog::close);
}
