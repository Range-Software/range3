/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   mesh_generator_dialog.cpp                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th September 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Mesh generator dialog class definition              *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "mesh_generator_dialog.h"
#include "session.h"
#include "mesh_generator.h"
#include "job_manager.h"

MeshGeneratorDialog::MeshGeneratorDialog(uint modelID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID)
{
    const Model &rModel = Session::getInstance().getModel(this->modelID);

    this->meshInput = rModel.getMeshInput();
    this->meshInput.setReconstruct(rModel.getNVolumes() > 0);

    this->setWindowTitle(tr("Genenerate 3D mesh"));

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    this->surfaceIntegrityCheck = new QCheckBox(tr("Check surface integrity"));
    mainLayout->addWidget(this->surfaceIntegrityCheck);
    this->surfaceIntegrityCheck->setChecked(rModel.getNVolumes() == 0);

    this->keepResultsCheck = new QCheckBox(tr("Keep node results"));
    mainLayout->addWidget(this->keepResultsCheck);
    this->keepResultsCheck->setChecked(this->meshInput.getKeepResults());

    this->qualityMeshGroupBox = new QGroupBox(tr("Quality mesh"));
    mainLayout->addWidget(this->qualityMeshGroupBox);
    this->qualityMeshGroupBox->setCheckable(true);
    this->qualityMeshGroupBox->setChecked(this->meshInput.getQualityMesh());

    QVBoxLayout *qualityMeshLayout = new QVBoxLayout;
    this->qualityMeshGroupBox->setLayout(qualityMeshLayout);

    QLabel *label = new QLabel(tr("Volume constraint:"));
    qualityMeshLayout->addWidget(label);

    this->volumeConstraintEdit = new ValueLineEdit(0.0,1e99);
    qualityMeshLayout->addWidget(this->volumeConstraintEdit);
    this->volumeConstraintEdit->setValue(this->meshInput.getVolumeConstraint());

    this->meshSizeFunctionGroupBox = new QGroupBox(tr("Generate mesh size function"));
    qualityMeshLayout->addWidget(this->meshSizeFunctionGroupBox);
    this->meshSizeFunctionGroupBox->setCheckable(true);
    this->meshSizeFunctionGroupBox->setChecked(false);

    QGridLayout *meshSizeFunctionLayout = new QGridLayout;
    this->meshSizeFunctionGroupBox->setLayout(meshSizeFunctionLayout);

    QLabel *meshSizeFunctionLabel = new QLabel(tr("Source variable"));
    meshSizeFunctionLayout->addWidget(meshSizeFunctionLabel,0,0,1,1);

    this->meshSizeFunctionSourceComboBox = new QComboBox;
    meshSizeFunctionLayout->addWidget(this->meshSizeFunctionSourceComboBox,0,1,1,1);

    for (uint i=0;i<rModel.getNVariables();i++)
    {
        this->meshSizeFunctionSourceComboBox->addItem(rModel.getVariable(i).getName(),
                                                      QVariant(int(rModel.getVariable(i).getType())));
    }

    QLabel *meshSizeFunctionMinValueLabel = new QLabel(tr("Minimum size"));
    meshSizeFunctionLayout->addWidget(meshSizeFunctionMinValueLabel,1,0,1,1);

    this->meshSizeFunctionMinValueEdit = new ValueLineEdit(this->volumeConstraintEdit->getMinimum(),
                                                       this->volumeConstraintEdit->getMaximum());
    this->meshSizeFunctionMinValueEdit->setValue(this->volumeConstraintEdit->getValue()*0.1);
    meshSizeFunctionLayout->addWidget(this->meshSizeFunctionMinValueEdit,1,1,1,1);

    QLabel *meshSizeFunctionMaxValueLabel = new QLabel(tr("Maximum size"));
    meshSizeFunctionLayout->addWidget(meshSizeFunctionMaxValueLabel,2,0,1,1);

    this->meshSizeFunctionMaxValueEdit = new ValueLineEdit(this->volumeConstraintEdit->getMinimum(),
                                                       this->volumeConstraintEdit->getMaximum());
    this->meshSizeFunctionMaxValueEdit->setValue(this->volumeConstraintEdit->getValue());
    meshSizeFunctionLayout->addWidget(this->meshSizeFunctionMaxValueEdit,2,1,1,1);

    this->tetgenParamsGroupBox = new QGroupBox(tr("TetGen parameters"));
    this->tetgenParamsGroupBox->setCheckable(true);
    this->tetgenParamsGroupBox->setChecked(false);
    mainLayout->addWidget(this->tetgenParamsGroupBox);

    QVBoxLayout *tetgenParamsLayout = new QVBoxLayout;
    this->tetgenParamsGroupBox->setLayout(tetgenParamsLayout);

    this->tetgenParamsEdit = new QLineEdit(this->meshInput.generateTetGenInputParams());
    tetgenParamsLayout->addWidget(this->tetgenParamsEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(this->surfaceIntegrityCheck,&QCheckBox::stateChanged,this,&MeshGeneratorDialog::onSurfaceIntegrityStateChanged);
    QObject::connect(this->qualityMeshGroupBox,&QGroupBox::clicked,this,&MeshGeneratorDialog::onQualityMeshGroupBoxClicked);
    QObject::connect(this->volumeConstraintEdit,&ValueLineEdit::valueChanged,this,&MeshGeneratorDialog::onVolumeConstraintValueChanged);
    QObject::connect(this->meshSizeFunctionMinValueEdit,&ValueLineEdit::valueChanged,this,&MeshGeneratorDialog::onVolumeConstraintValueChanged);
    QObject::connect(this->meshSizeFunctionMaxValueEdit,&ValueLineEdit::valueChanged,this,&MeshGeneratorDialog::onVolumeConstraintValueChanged);
    QObject::connect(this->keepResultsCheck,&QCheckBox::stateChanged,this,&MeshGeneratorDialog::onKeepResultsStateChanged);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MeshGeneratorDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MeshGeneratorDialog::accept);
}

int MeshGeneratorDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        if (this->tetgenParamsGroupBox->isChecked())
        {
            this->meshInput.setUseTetGenInputParams(true);
            this->meshInput.setTetGenInputParams(this->tetgenParamsEdit->text());
        }
        else
        {

            if (this->qualityMeshGroupBox->isChecked() && this->meshSizeFunctionGroupBox->isChecked())
            {
                const Model &rModel = Session::getInstance().getModel(this->modelID);

                RVariableType meshSizeFunctionVariableType = RVariableType(this->meshSizeFunctionSourceComboBox->currentData().toInt());
                double meshSizeFunctionMinValue = this->meshSizeFunctionMinValueEdit->getValue();
                double meshSizeFunctionMaxValue = this->meshSizeFunctionMaxValueEdit->getValue();

                RRVector meshSizeFunction = rModel.generateMeshSizeFunction(meshSizeFunctionVariableType,
                                                                            meshSizeFunctionMinValue,
                                                                            meshSizeFunctionMaxValue);

                this->meshInput.setUseSizeFunction(true);
                this->meshInput.setSizeFunctionValues(meshSizeFunction);
            }

            this->meshInput.setUseTetGenInputParams(false);
            this->meshInput.setTetGenInputParams(this->meshInput.generateTetGenInputParams());
        }

        Session::getInstance().getModel(this->modelID).setMeshInput(this->meshInput);

        MeshGenerator *meshGenerator = new MeshGenerator(this->modelID);
        JobManager::getInstance().submit(meshGenerator);
    }
    return retVal;
}

void MeshGeneratorDialog::updateMeshInput(void)
{
    this->meshInput.setSurfaceIntegrityCheck(this->surfaceIntegrityCheck->isChecked());
    this->meshInput.setQualityMesh(this->qualityMeshGroupBox->isChecked());
    this->meshInput.setVolumeConstraint(this->volumeConstraintEdit->getValue());
    this->meshInput.setKeepResults(this->keepResultsCheck->isChecked());
    this->meshInput.setReconstruct(Session::getInstance().getModel(this->modelID).getNVolumes() > 0);

    this->meshSizeFunctionMaxValueEdit->setRange(this->volumeConstraintEdit->getMinimum(),
                                                 this->volumeConstraintEdit->getValue());
    if (this->meshSizeFunctionMaxValueEdit->getValue() > this->meshSizeFunctionMaxValueEdit->getMaximum())
    {
        this->meshSizeFunctionMaxValueEdit->setValue(this->meshSizeFunctionMaxValueEdit->getMaximum());
    }

    this->meshSizeFunctionMinValueEdit->setRange(this->meshSizeFunctionMaxValueEdit->getMinimum(),
                                                 this->meshSizeFunctionMaxValueEdit->getValue());
    if (this->meshSizeFunctionMinValueEdit->getValue() > this->meshSizeFunctionMaxValueEdit->getValue())
    {
        this->meshSizeFunctionMinValueEdit->setValue(this->meshSizeFunctionMinValueEdit->getMaximum());
    }

    // Update TetGen parameters line edit.
    this->tetgenParamsEdit->setText(this->meshInput.generateTetGenInputParams());
}

void MeshGeneratorDialog::onSurfaceIntegrityStateChanged(int)
{
    this->updateMeshInput();
}

void MeshGeneratorDialog::onQualityMeshGroupBoxClicked(bool)
{
    this->updateMeshInput();
}

void MeshGeneratorDialog::onVolumeConstraintValueChanged(double)
{
    this->updateMeshInput();
}

void MeshGeneratorDialog::onMeshSizeFunctionMinValueChanged(double)
{
    this->updateMeshInput();
}

void MeshGeneratorDialog::onMeshSizeFunctionMaxValueChanged(double)
{
    this->updateMeshInput();
}

void MeshGeneratorDialog::onKeepResultsStateChanged(int)
{
    this->updateMeshInput();
}
