/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   mesh_generator_dialog.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th September 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Mesh generator dialog class declaration             *
 *********************************************************************/

#ifndef __MESH_GENERATOR_DIALOG_H__
#define __MESH_GENERATOR_DIALOG_H__

#include <QDialog>
#include <QCheckBox>
#include <QGroupBox>

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
        //! Keep results check box.
        QCheckBox *keepResultsCheck;
        //! TetGen parameters group box.
        QGroupBox *tetgenParamsGroupBox;
        //! TetGen parameters line edit.
        QLineEdit *tetgenParamsEdit;

        RMeshInput meshInput;

    public:

        //! Constructor.
        explicit MeshGeneratorDialog(uint modelID, QWidget *parent = 0);

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

        //! Keep results checkbox state changed.
        void onKeepResultsStateChanged(int);
    
};

#endif // __MESH_GENERATOR_DIALOG_H__
