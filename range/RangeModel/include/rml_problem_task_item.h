/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_problem_task_item.h                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Problem task item class declaration                 *
 *********************************************************************/

#ifndef RML_PROBLEM_TASK_ITEM_H
#define RML_PROBLEM_TASK_ITEM_H

#include <vector>

#include "rml_problem_type.h"

class RProblemTaskItem
{

    protected:

        //! Problem type.
        RProblemType problemType;
        //! Number of children iterations.
        //! If problem type is not R_PROBLEM_NONE nIterations is ignored.
        unsigned int nIterations;
        //! Children items.
        //! If problem type is not R_PROBLEM_NONE children is ignored.
        std::vector<RProblemTaskItem> children;

    private:

        //! Internal initialization function.
        void _init(const RProblemTaskItem *pSolverTaskItem = nullptr);

    public:

        //! Constructor.
        RProblemTaskItem(RProblemType problemType = R_PROBLEM_NONE);

        //! Copy constructor.
        RProblemTaskItem(const RProblemTaskItem &solverTaskItem);

        //! Destructor.
        ~RProblemTaskItem();

        //! Assignment operator.
        RProblemTaskItem & operator =(const RProblemTaskItem &solverTaskItem);

        //! Return problem type mask.
        RProblemTypeMask getProblemTypeMask(void) const;

        //! Return problem type.
        RProblemType getProblemType(void) const;

        //! Return problem type.
        void setProblemType(RProblemType problemType);

        //! Return number of iterations.
        unsigned int getNIterations(void) const;

        //! Return number of iterations.
        void setNIterations(unsigned int nIterations);

        //! Return number of children.
        unsigned int getNChildren(void) const;

        //! Return const reference to child at given position.
        const RProblemTaskItem &getChild(unsigned int position) const;

        //! Return reference to child at given position.
        RProblemTaskItem &getChild(unsigned int position);

        //! Add child.
        void addChild(const RProblemTaskItem &solverTaskItem);

        //! Insert child.
        void insertChild(unsigned int position, const RProblemTaskItem &solverTaskItem);

        //! Remove child.
        void removeChild(unsigned int position);

        //! Print task tree.
        void print(bool printTitle = true) const;

        //! Generate default problem tree from given problem type mask.
        static RProblemTaskItem generateDefaultTree(RProblemTypeMask problemTypeMask);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_PROBLEM_TASK_ITEM_H
