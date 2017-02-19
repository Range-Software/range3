/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_tree.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th July 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Problem tree class declaration                      *
 *********************************************************************/

#ifndef __PROBLEM_TREE_H__
#define __PROBLEM_TREE_H__

#include <QTreeWidget>

#include <rmlib.h>

#include "file_chooser_button.h"

class ProblemTree : public QTreeWidget
{

    Q_OBJECT

    protected:

        FileChooserButton *radiationVfFileButton;

    public:

        //! Constructor.
        explicit ProblemTree(QWidget *parent = 0);

    protected slots:

        //! Populate.
        void populate(void);

        //! Model has changed.
        void onModelChanged(uint modelID);

        //! Problem has changed.
        void onProblemChanged(uint modelID);

        //! Model selection has changed.
        void onModelSelectionChanged(uint modelID);

        //! Time solver has changed.
        void onTimeSolverChanged(const RTimeSolver &timeSolver);

        //! Modal setup has changed.
        void onModalSetupChanged(const RModalSetup &modalSetup);

        //! Radiation setup has changed.
        void onRadiationSetupChanged(const RRadiationSetup &radiationSetup);

};

#endif // __PROBLEM_TREE_H__
