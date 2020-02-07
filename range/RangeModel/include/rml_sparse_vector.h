/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_sparse_vector.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Range sparse vector class declaration               *
 *********************************************************************/

#ifndef RML_SPARSE_VECTOR_H
#define RML_SPARSE_VECTOR_H

#include <QtGlobal>
#include <vector>

/*
 * Following vector:
 *
 * [ 0.0 , 1.0 , 0.0 , 2.0 , 0.0 , 3.0 ]
 *
 * will be stored as:
 *
 *            | RSparseVectorItem |
 * +----------++-------+---------++
 * | Position || Index | Value   ||
 * +----------++-------+---------++
 * | 0        || 1     | 1.0     ||
 * | 1        || 3     | 2.0     ||
 * | 2        || 5     | 3.0     ||
 * +----------++-------+---------++
 */

template <class T>
class RSparseVectorItem
{

    public:

        //! Value index position.
        uint index;
        //! Value.
        T value;

        //! Constructor.
        RSparseVectorItem(uint index, T value)
            : index(index)
            , value(value)
        {
        }

        //! Less operator.
        bool operator <(const RSparseVectorItem<T> &sparseVectorItem) const
        {
            return (this->index < sparseVectorItem.index);
        }

        //! Equals operator.
        bool operator ==(const RSparseVectorItem<T> &sparseVectorItem) const
        {
            return (this->index == sparseVectorItem.index);
        }
};

template <class T>
class RSparseVector
{

    protected:

        //! Vector of matrix rows.
        std::vector< RSparseVectorItem<T> > data;

    private:

        //! Internal initialization function.
        void _init(const RSparseVector<T> *pVector = nullptr)
        {
            if (pVector)
            {
                this->data = pVector->data;
            }
        }

    public:

        //! Constructor.
        RSparseVector()
        {
            this->_init();
        }

        //! Copy constructor.
        RSparseVector(const RSparseVector<T> &vector)
        {
            this->_init(&vector);
        }

        //! Destructor.
        ~RSparseVector()
        {
        }

        //! Assignment operator.
        RSparseVector<T> & operator =(const RSparseVector<T> &vector)
        {
            this->_init(&vector);
            return (*this);
        }

        //! Return size of the vector.
        uint size(void) const
        {
            return uint(this->data.size());
        }

        //! Return value at given position.
        const T &getValue(uint position) const
        {
            return this->data[position].value;
        }

        //! Return index at given position.
        uint getIndex(uint position) const
        {
            return this->data[position].index;
        }

        //! Return vector of position indexes.
        std::vector<uint> getIndexes(void) const
        {
            std::vector<uint> idxList;

            idxList.resize(this->data.size());

            for (uint i=0;i<this->data.size();i++)
            {
                idxList[i] = this->data[i].index;
            }

            return idxList;
        }

        //! Add value.
        //! If value with given index already exist value will be added to its current value.
        void addValue(uint index, T value)
        {
            typename std::vector< RSparseVectorItem<T> >::iterator iter;
            RSparseVectorItem<T> match(index,value);

            iter = std::find(this->data.begin(),this->data.end(),match);
            if (iter == this->data.end())
            {
                this->data.push_back(RSparseVectorItem<T>(index,value));
                std::sort(this->data.begin(),this->data.end());
            }
            else
            {
                iter->value += value;
            }
        }

        //! Return real sized vector of values.
        //! nElements difines minimum size of the vector.
        std::vector<T> getValues(uint nElements) const
        {
            std::vector<T> values;
            values.resize(nElements,0.0);

            for (uint i=0;i<this->data.size();i++)
            {
                if (this->data[i].index >= nElements)
                {
                    nElements = this->data[i].index + 1;
                    values.resize(nElements,0.0);
                }
                values[this->data[i].index] = this->data[i].value;
            }
            return values;
        }

        //! Vector add operation.
        void addVector(const RSparseVector<T> &v)
        {
            for (uint i=0;i<v.size();i++)
            {
                this->addValue(v.getIndex(i),v.getValue(i));
            }
        }

        //! Reserve vector size.
        void reserve(uint nElements)
        {
            this->data.reserve(nElements);
        }

        //! Clear vector.
        void clear(void)
        {
            this->data.clear();
        }

};

#endif // RML_SPARSE_VECTOR_H
