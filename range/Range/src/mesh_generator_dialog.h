/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   mesh_generator_dialog.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th September 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Mesh generator dialog class declaration             *
 *********************************************************************/

#ifndef MESH_GENERATOR_DIALOG_H
#define MESH_GENERATOR_DIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>

#include <rmlib.h>

#include "value_line_edit.h"

class MeshGeneratorDialog : public QDialog
{
    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Keep results check box.
        QCheckBox *surfaceIntegrityCheck;
        //! Quality mesh group box.
        QGroupBox *qualityMeshGroupBox;
        //! Volume constraint.
        ValueLineEdit *volumeConstraintEdit;
        //! Mesh size function group box.
        QGroupBox *meshSizeFunctionGroupBox;
        //! Mesh size source variable.
        QComboBox *meshSizeFunctionSourceComboBox;
        //! Mesh size function minimum value.
        ValueLineEdit *meshSizeFunctionMinValueEdit;
        //! Mesh size function maximum value.
        ValueLineEdit *meshSizeFunctionMaxValueEdit;
        //! Reconstruct check box.
        QCheckBox *reconstructCheck;
        //! Keep results check box.
        QCheckBox *keepResultsCheck;
        //! TetGen parameters group box.
        QGroupBox *tetgenParamsGroupBox;
        //! TetGen parameters line edit.
        QLineEdit *tetgenParamsEdit;

        RMeshInput meshInput;

    public:

        //! Constructor.
        explicit MeshGeneratorDialog(uint modelID, QWidget *parent = nullptr);

        //! Overloaded exec function.
        int exec(void);

    private:

        //! Update mesh generator input.
        void updateMeshInput(void);

    private slots:

        //! Surface integrity checkbox state changed.
        void onSurfaceIntegrityStateChanged(int);

        //! Quality mesh group box clicked.
        void onQualityMeshGroupBoxClicked(bool);

        //! Volume constraint value changed.
        void onVolumeConstraintValueChanged(double);

        //! Mesh size function minimum value changed.
        void onMeshSizeFunctionMinValueChanged(double);

        //! Mesh size function maximum value changed.
        void onMeshSizeFunctionMaxValueChanged(double);

        //! Reconstruct checkbox state changed.
        void onReconstructStateChanged(int);

        //! Keep results checkbox state changed.
        void onKeepResultsStateChanged(int);

        //! Tetgen parameters group box clicked.
        void onTetgenParamsGroupBoxClicked(bool);
    
};

#endif // MESH_GENERATOR_DIALOG_H
