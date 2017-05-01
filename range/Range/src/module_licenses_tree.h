/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   module_licenses_tree.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Module licenses tree class declaration              *
 *********************************************************************/

#ifndef __MODULE_LICENSES_TREE_H__
#define __MODULE_LICENSES_TREE_H__

#include <QTreeWidget>

#include <ralib.h>

class ModuleLicensesTree : public QTreeWidget
{

    Q_OBJECT

    private:

        enum TreeColumn
        {
            Name = 0,
            ModuleID,
            ExpiryDate,
            NColumns
        };

    public:

        //! Constructor.
        explicit ModuleLicensesTree(const RLicense &rLicense, QWidget *parent = 0);

};

#endif // __MODULE_LICENSES_TREE_H__
