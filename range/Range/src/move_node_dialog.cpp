/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   move_node_dialog.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th February 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Move node dialog class definition                   *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

#include "move_node_dialog.h"
#include "position_widget.h"
#include "session.h"

MoveNodeDialog::MoveNodeDialog(uint modelID, const QList<uint> &nodeIDs, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    nodeIDs(nodeIDs)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Move node"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    QWidget *containerWidget = new QWidget;
    scrollArea->setWidget(containerWidget);

    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);

    const Model &rModel = Session::getInstance().getModel(this->modelID);

    for (int i=0;i<nodeIDs.size();i++)
    {
        QString labelStr = tr("Node") + QString(" # ") + QString::number(this->nodeIDs[i]);
        PositionWidget *positionWidget = new PositionWidget(labelStr,rModel.getNode(this->nodeIDs[i]).toVector(),true);
        positionWidget->hidePickButton();
        containerLayout->addWidget(positionWidget);

        QObject::connect(positionWidget,&PositionWidget::changed,this,&MoveNodeDialog::onPositionChanged);

        this->positionWidgets.append(positionWidget);
    }

    containerLayout->addStretch(1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
    this->okButton->setEnabled(false);
    this->okButton->setDefault(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    Session::getInstance().setBeginDrawMoveNodes(this->findNodeIdPositionMap());
}

int MoveNodeDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        Session::getInstance().storeCurentModelVersion(this->modelID,tr("Move node"));
        Model &rModel = Session::getInstance().getModel(this->modelID);

        for (int i=0;i<this->nodeIDs.size();i++)
        {
            RR3Vector position(this->positionWidgets[i]->getPosition());

            rModel.getNode(this->nodeIDs[i]).set(position[0],position[1],position[2]);
        }
    }

    Session::getInstance().setEndDrawMoveNodes();
    Session::getInstance().setModelChanged(this->modelID);

    return retVal;
}

QMap<SessionNodeID, RR3Vector> MoveNodeDialog::findNodeIdPositionMap(void) const
{
    QMap<SessionNodeID, RR3Vector> nodeIDMap;

    for (int i=0;i<this->nodeIDs.size();i++)
    {
        nodeIDMap.insert(SessionNodeID(this->modelID,this->nodeIDs[i]),this->positionWidgets[i]->getPosition());
    }

    return nodeIDMap;
}

void MoveNodeDialog::onPositionChanged(const RR3Vector &)
{
    this->okButton->setEnabled(true);
    Session::getInstance().setBeginDrawMoveNodes(this->findNodeIdPositionMap());
}
