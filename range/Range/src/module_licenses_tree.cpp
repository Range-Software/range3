/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   module_licenses_tree.cpp                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Module licenses tree class definition               *
 *********************************************************************/

#include <rmlib.h>

#include "main_settings.h"
#include "module_licenses_tree.h"

ModuleLicensesTree::ModuleLicensesTree(const RLicense &rLicense, QWidget *parent)
    : QTreeWidget(parent)
{
    this->setRootIsDecorated(true);
//    this->setSelectionMode(QAbstractItemView::NoSelection);

    this->setColumnCount(ModuleLicensesTree::NColumns);

    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(ModuleLicensesTree::Name,QString(tr("Name")));
    headerItem->setText(ModuleLicensesTree::ModuleID,QString(tr("ID")));
    headerItem->setText(ModuleLicensesTree::ExpiryDate,QString(tr("Expiration")));
    this->setHeaderItem(headerItem);

    this->resizeColumnToContents(ModuleLicensesTree::Name);

    std::vector<RProblemType> prbList(RProblem::getTypes(R_PROBLEM_ALL));

    for (uint i=0;i<prbList.size();i++)
    {
        QString expirationDate;

        if (rLicense.validateRecord(RProblem::getId(prbList[i]),
                                    MainSettings::getInstance().getApplicationSettings()->getRangeAccount(),
                                    MainSettings::getInstance().getApplicationSettings()->getRangePassword()))
        {
            expirationDate = rLicense.findRecordExpirationDate(RProblem::getId(prbList[i])).toString();
        }

        QTreeWidgetItem *itemModel = new QTreeWidgetItem(this);
        itemModel->setText(ModuleLicensesTree::Name, RProblem::getName(prbList[i]));
        itemModel->setText(ModuleLicensesTree::ModuleID, RProblem::getId(prbList[i]));
        itemModel->setText(ModuleLicensesTree::ExpiryDate, expirationDate);
    }
    for (int i=0;i<ModuleLicensesTree::NColumns;i++)
    {
        this->resizeColumnToContents(i);
    }


}
