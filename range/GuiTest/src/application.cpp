#include <QTimer>
#include <QPalette>
#include <QStyle>
#include <QStyleFactory>

#include "application.h"

Application::Application(int &argc, char **argv) :
    QApplication(argc,argv)
{
    this->setStyleSheet(QLatin1String("QDialogButtonBox {"
                                      "  button-layout: 0;"
                                      "  dialogbuttonbox-buttons-have-icons: 1;"
                                      "}"));

    QTimer::singleShot(0, this, SIGNAL(started()));
}

void Application::applyStyle(const QString &styleName)
{
    if (styleName == "dark")
    {
        QPalette palette;
    //    palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::Window, QColor(83,83,83));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(15,15,15));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, Qt::lightGray);

        palette.setColor(QPalette::Highlight, Qt::lightGray);
        palette.setColor(QPalette::HighlightedText, Qt::black);

        palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

        QApplication::setPalette(palette);

        QApplication::setStyle(QStyleFactory::create("Fusion"));
    }
    else
    {
        QStyle *style = QStyleFactory::create(styleName);
        QApplication::setPalette(style->standardPalette());
        QApplication::setStyle(style);
    }
}

void Application::onStyleChanged(const QString &styleName)
{
    this->applyStyle(styleName);
}
