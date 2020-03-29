/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_tree.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th July 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Problem tree class definition                       *
 *********************************************************************/

#include "problem_tree.h"
#include "session.h"
#include "mesh_setup_widget.h"
#include "modal_setup_widget.h"
#include "radiation_setup_widget.h"
#include "time_solver_setup_widget.h"

typedef enum _ProblemTreeColumn
{
    PROBLEM_TREE_COLUMN_1 = 0,
    PROBLEM_TREE_N_COLUMNS
} ProblemTreeColumn;

ProblemTree::ProblemTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(PROBLEM_TREE_N_COLUMNS);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setRootIsDecorated(false);
    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(PROBLEM_TREE_COLUMN_1,QString(tr("Problem setup")));
    this->setHeaderItem(headerItem);
    this->setHeaderHidden(true);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    this->populate();

    QObject::connect(&Session::getInstance(),
                     &Session::problemSelectionChanged,
                     this,
                     &ProblemTree::onProblemChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelChanged,
                     this,
                     &ProblemTree::onModelChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelSelectionChanged,
                     this,
                     &ProblemTree::onModelSelectionChanged);


}

void ProblemTree::populate(void)
{
    this->clear();

    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    if (modelIDs.size() == 0)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(PROBLEM_TREE_COLUMN_1,tr("No model is selected!"));
        item->setForeground(PROBLEM_TREE_COLUMN_1,QColor(Qt::red));
        return;
    }
    else if (modelIDs.size() > 1)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(PROBLEM_TREE_COLUMN_1,tr("Multiple models are selected!"));
        item->setForeground(PROBLEM_TREE_COLUMN_1,QColor(Qt::red));
        return;
    }

    const Model &rModel = Session::getInstance().getModel(modelIDs[0]);

    if (RProblem::getTimeSolverEnabled(rModel.getProblemTaskTree().getProblemTypeMask()))
    {
        QTreeWidgetItem *itemTimeSolver = new QTreeWidgetItem(this);
        TimeSolverSetupWidget *timeSolverWidget = new TimeSolverSetupWidget(rModel.getTimeSolver());
        this->setItemWidget(itemTimeSolver,PROBLEM_TREE_COLUMN_1,timeSolverWidget);
        QObject::connect(timeSolverWidget,&TimeSolverSetupWidget::changed,this,&ProblemTree::onTimeSolverChanged);
    }

    if (rModel.getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_STRESS_MODAL)
    {
        QTreeWidgetItem *modalSetup = new QTreeWidgetItem(this);
        ModalSetupWidget *modalSetupWidget = new ModalSetupWidget(rModel.getProblemSetup().getModalSetup());
        this->setItemWidget(modalSetup,PROBLEM_TREE_COLUMN_1,modalSetupWidget);
        QObject::connect(modalSetupWidget,&ModalSetupWidget::changed,this,&ProblemTree::onModalSetupChanged);
    }

    if (rModel.getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_RADIATIVE_HEAT)
    {
        QString vfFileName = rModel.buildDataFileName(RViewFactorMatrix::getDefaultFileExtension(true),rModel.getTimeSolver().getEnabled());

        QTreeWidgetItem *itemRadiationSetup = new QTreeWidgetItem(this);
        RadiationSetupWidget *radiationSetupWidget = new RadiationSetupWidget(rModel.getProblemSetup().getRadiationSetup(),vfFileName);
        this->setItemWidget(itemRadiationSetup,PROBLEM_TREE_COLUMN_1,radiationSetupWidget);
        QObject::connect(radiationSetupWidget,&RadiationSetupWidget::changed,this,&ProblemTree::onRadiationSetupChanged);
    }

    if (rModel.getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_MESH)
    {
        QTreeWidgetItem *meshSetup = new QTreeWidgetItem(this);
        MeshSetupWidget *meshSetupWidget = new MeshSetupWidget(rModel.getProblemSetup().getMeshSetup(),
                                                               RProblem::getVariableTypes(rModel.getProblemTaskTree().getProblemTypeMask()));
        this->setItemWidget(meshSetup,PROBLEM_TREE_COLUMN_1,meshSetupWidget);
        QObject::connect(meshSetupWidget,&MeshSetupWidget::changed,this,&ProblemTree::onMeshSetupChanged);
    }
}

void ProblemTree::onModelChanged(uint)
{
    this->populate();
}

void ProblemTree::onProblemChanged(uint)
{
    this->populate();
}

void ProblemTree::onModelSelectionChanged(uint)
{
    this->populate();
}

void ProblemTree::onTimeSolverChanged(const RTimeSolver &timeSolver)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        Session::getInstance().getModel(modelIDs[i]).setTimeSolver(timeSolver);
        Session::getInstance().setProblemChanged(modelIDs[i]);
    }
}

void ProblemTree::onModalSetupChanged(const RModalSetup &modalSetup)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        Session::getInstance().getModel(modelIDs[i]).getProblemSetup().setModalSetup(modalSetup);
        Session::getInstance().setProblemChanged(modelIDs[i]);
    }
}

void ProblemTree::onRadiationSetupChanged(const RRadiationSetup &radiationSetup)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        Session::getInstance().getModel(modelIDs[i]).getProblemSetup().setRadiationSetup(radiationSetup);
        Session::getInstance().setProblemChanged(modelIDs[i]);
    }
}

void ProblemTree::onMeshSetupChanged(const RMeshSetup &meshSetup)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        Session::getInstance().getModel(modelIDs[i]).getProblemSetup().setMeshSetup(meshSetup);
        Session::getInstance().setProblemChanged(modelIDs[i]);
    }
}
