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
    const RMeshInput &rMeshInput = Session::getInstance().getModel(this->modelID).getMeshInput();

    this->setWindowTitle(tr("Genenerate 3D mesh"));

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    this->surfaceIntegrityCheck = new QCheckBox(tr("Check surface integrity"));
    mainLayout->addWidget(this->surfaceIntegrityCheck);
    this->surfaceIntegrityCheck->setChecked(Session::getInstance().getModel(this->modelID).getNVolumes() == 0);

    this->qualityMeshGroupBox = new QGroupBox(tr("Quality mesh"));
    mainLayout->addWidget(this->qualityMeshGroupBox);
    this->qualityMeshGroupBox->setCheckable(true);
    this->qualityMeshGroupBox->setChecked(rMeshInput.getQualityMesh());

    QGridLayout *qualityMeshLayout = new QGridLayout;
    this->qualityMeshGroupBox->setLayout(qualityMeshLayout);

    int qualityMeshLayoutRow = 0;

    QLabel *label = new QLabel(tr("Volume constraint:"));
    qualityMeshLayout->addWidget(label, qualityMeshLayoutRow, 0, 1, 1);

    this->volumeConstraintEdit = new ValueLineEdit(0.0,1e99);
    qualityMeshLayout->addWidget(this->volumeConstraintEdit, qualityMeshLayoutRow++, 1, 1, 1);
    this->volumeConstraintEdit->setValue(rMeshInput.getVolumeConstraint());

    this->keepResultsCheck = new QCheckBox(tr("Keep node results"));
    mainLayout->addWidget(this->keepResultsCheck);
    this->keepResultsCheck->setChecked(rMeshInput.getKeepResults());

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MeshGeneratorDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MeshGeneratorDialog::accept);
}

int MeshGeneratorDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        RMeshInput &rMeshInput = Session::getInstance().getModel(this->modelID).getMeshInput();

        rMeshInput.setSurfaceIntegrityCheck(this->surfaceIntegrityCheck->isChecked());
        rMeshInput.setQualityMesh(this->qualityMeshGroupBox->isChecked());
        rMeshInput.setVolumeConstraint(this->volumeConstraintEdit->getValue());
        rMeshInput.setKeepResults(this->keepResultsCheck->isChecked());

        MeshGenerator *meshGenerator = new MeshGenerator(this->modelID);
        JobManager::getInstance().submit(meshGenerator);
    }
    return retVal;
}
