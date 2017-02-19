/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   update_dialog.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-nd August 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Update dialog class definition                      *
 *********************************************************************/

#include <QVBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>

#include "update_dialog.h"
#include "session.h"

UpdateDialog::UpdateDialog(const RVersion &softwareVersion, const QString &softwareLink, QWidget *parent)
    : QDialog(parent)
    , softwareVersion(softwareVersion)
    , softwareLink(softwareLink)
{
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon updateIcon(":/icons/file/pixmaps/range-download.svg");

    QString windowTitleStr = tr("Software update");
    this->setWindowTitle(tr(windowTitleStr.toUtf8().constData()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    QString message;
    if (RVendor::version < this->softwareVersion)
    {
        if (!this->softwareLink.isEmpty())
        {
            message += "<big><strong>" + tr("Download and install latest version?") + "</strong></big><br/><br/>";
        }
        message += tr("New version is available for download.") + "<br/>";
        message += tr("Installed version") + ": <span style=\"color:red\">" + RVendor::version.toString() + "</span><br/>";
        message += tr("Available version") + ": <span style=\"color:blue\">" + this->softwareVersion.toString() + "</span><br/>";
        message += tr("Visit") + " <a href=\"" + RVendor::www + "\">" + RVendor::www + "</a> " + tr("for more information.");
    }
    else
    {
        message += tr("Software is up to date.");
        this->softwareLink.clear();
    }

    QLabel *label = new QLabel(message);
    mainLayout->addWidget(label);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    if (this->softwareLink.isEmpty())
    {
        QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
        buttonsLayout->addWidget(closeButton);

        QObject::connect(closeButton,&QPushButton::clicked,this,&UpdateDialog::reject);
    }
    else
    {
        QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
        buttonsLayout->addWidget(cancelButton);

        QPushButton *updateButton = new QPushButton(updateIcon, tr("Update"));
        buttonsLayout->addWidget(updateButton);

        QObject::connect(cancelButton,&QPushButton::clicked,this,&UpdateDialog::reject);
        QObject::connect(updateButton,&QPushButton::clicked,this,&UpdateDialog::accept);
    }
}

int UpdateDialog::exec()
{
    int retVal = this->QDialog::exec();
    if (retVal == QDialog::Accepted)
    {
        Session::getInstance().getDownloadManager()->append(QUrl(this->softwareLink));
        QObject::connect(Session::getInstance().getDownloadManager(),&DownloadManager::fileSaved,&Session::getInstance(),&Session::onSoftwareUpdateFileSaved);
    }

    return retVal;
}
