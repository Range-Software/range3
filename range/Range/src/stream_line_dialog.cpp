/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   stream_line_dialog.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Stream line dialog class definition                 *
 *********************************************************************/

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

#include "session.h"
#include "stream_line_dialog.h"

StreamLineDialog::StreamLineDialog(uint modelID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    entityID(RConstants::eod)
{
    this->createDialog();
}

StreamLineDialog::StreamLineDialog(uint modelID, uint entityID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    entityID(entityID)
{
    this->createDialog();
}

int StreamLineDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        RVariableType varType = this->getVariableType();

        if (varType != R_VARIABLE_NONE)
        {
            Session::getInstance().storeCurentModelVersion(this->modelID,tr("Create stream line"));

            if (this->entityID != RConstants::eod)
            {
                RStreamLine &streamLine = Session::getInstance().getModel(this->modelID).getStreamLine(this->entityID);

                streamLine.setVariableType(varType);
                streamLine.setPosition(this->positionWidget->getPosition());

                RLogger::info("Modified stream line \'%s\'\n",streamLine.getName().toUtf8().constData());
            }
            else
            {
                RStreamLine streamLine;

                streamLine.setName(RVariable::getName(varType));
                streamLine.setVariableType(varType);
                streamLine.setPosition(this->positionWidget->getPosition());

                Session::getInstance().getModel(this->modelID).addStreamLine(streamLine);
                RLogger::info("Created new stream line \'%s\'\n",streamLine.getName().toUtf8().constData());
            }

            Session::getInstance().setModelChanged(this->modelID);
        }
    }

    Session::getInstance().setEndDrawStreamLinePosition();

    return retVal;
}

void StreamLineDialog::createDialog(void)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->resize(400,300);

    QString windowTitleStr = tr("Stream line editor");
    this->setWindowTitle(windowTitleStr);

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *titleLabel = new QLabel;
    if (this->entityID == RConstants::eod)
    {
        titleLabel->setText(tr("Create new stream line."));
    }
    else
    {
        titleLabel->setText(tr("Modify stream line:") + " <b>" + Session::getInstance().getModel(modelID).getStreamLine(entityID).getName() + "</b>");
    }
    mainLayout->addWidget(titleLabel, 0, 0, 1, 1);

    this->variableList = new QListWidget(this);
    this->variableList->setSelectionMode(QAbstractItemView::SingleSelection);

    Model &rModel = Session::getInstance().getModel(this->modelID);
    RR3Vector startPosition;

    for (uint i=0;i<rModel.getNVariables();i++)
    {
        RVariable &rVariable = rModel.getVariable(i);
        if (rVariable.getNVectors() > 1)
        {
            QListWidgetItem *itemVariable = new QListWidgetItem(this->variableList);
            itemVariable->setText(rVariable.getName());
            itemVariable->setData(Qt::UserRole,QVariant(rVariable.getType()));
            if (this->entityID != RConstants::eod)
            {
                RStreamLine &rStreamLine = Session::getInstance().getModel(this->modelID).getStreamLine(this->entityID);
                itemVariable->setSelected(rVariable.getType() == rStreamLine.getVariableType());
                startPosition = rStreamLine.getPosition();
            }
        }
    }

    mainLayout->addWidget(this->variableList, 1, 0, 1, 1);

    QObject::connect(this->variableList,
                     &QListWidget::itemSelectionChanged,
                     this,
                     &StreamLineDialog::onVariableListSelectionChanged);

    double xMin = 0.0;
    double xMax = 0.0;
    double yMin = 0.0;
    double yMax = 0.0;
    double zMin = 0.0;
    double zMax = 0.0;

    rModel.findNodeLimits(xMin,xMax,yMin,yMax,zMin,zMax);

    this->positionWidget = new PositionWidget("Stream line position",startPosition);
    this->positionWidget->hideButtons();
    this->positionWidget->setXRange(xMin,xMax);
    this->positionWidget->setYRange(yMin,yMax);
    this->positionWidget->setZRange(zMin,zMax);
    mainLayout->addWidget(this->positionWidget, 2, 0, 1, 1);

    QObject::connect(this->positionWidget,&PositionWidget::changed,this,&StreamLineDialog::onPositionChanged);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 3, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
    this->okButton->setEnabled(this->variableList->selectedItems().size());
    this->okButton->setDefault(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&StreamLineDialog::reject);
    QObject::connect(this->okButton,&QPushButton::clicked,this,&StreamLineDialog::accept);

    Session::getInstance().setBeginDrawStreamLinePosition(startPosition);
}

RVariableType StreamLineDialog::getVariableType(void) const
{
    QList<QListWidgetItem*> items = this->variableList->selectedItems();

    if (items.size() == 0)
    {
        return R_VARIABLE_NONE;
    }

    return RVariableType(items[0]->data(Qt::UserRole).toInt());
}

void StreamLineDialog::onPositionChanged(const RR3Vector &position)
{
    Session::getInstance().setBeginDrawStreamLinePosition(position);
}

void StreamLineDialog::onVariableListSelectionChanged(void)
{
    this->okButton->setEnabled(this->variableList->selectedItems().size());
}
