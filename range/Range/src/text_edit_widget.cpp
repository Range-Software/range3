/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_edit_widget.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   2-nd November 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Text edit widget class definition                   *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "text_edit_widget.h"

TextEditWidget::TextEditWidget(QWidget *parent)
    : QWidget(parent)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);

    this->textEdit = new QPlainTextEdit;
    layout->addWidget(this->textEdit);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    this->textEdit->setFont(font);

    QObject::connect(this->textEdit,&QPlainTextEdit::textChanged,this,&TextEditWidget::onTextEditChanged);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    layout->addLayout(buttonsLayout);

    this->cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(this->cancelButton);

    QObject::connect(this->cancelButton,&QPushButton::clicked,this,&TextEditWidget::onCancelButtonClicked);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
    this->okButton->setDisabled(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(this->okButton,&QPushButton::clicked,this,&TextEditWidget::onOkButtonClicked);
}

void TextEditWidget::setText(const QString &text)
{
    this->textEdit->setPlainText(text);
}

void TextEditWidget::onTextEditChanged()
{
    this->okButton->setDisabled(this->textEdit->toPlainText().isEmpty());
}

void TextEditWidget::onCancelButtonClicked()
{
    emit this->closed();
}

void TextEditWidget::onOkButtonClicked()
{
    emit this->changed(textEdit->toPlainText());
    emit this->closed();
}
