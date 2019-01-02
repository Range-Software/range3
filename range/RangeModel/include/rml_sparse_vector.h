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

#include <vector>

template <class T>
class RSparseVectorItem
{

    public:

        //! Value index position.
        unsigned int index;
        //! Value.
        T value;

        //! Constructor.
        RSparseVectorItem(unsigned int index, T value)
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
        unsigned int size(void) const
        {
            return (unsigned int)this->data.size();
        }

        //! Return value at given position.
        const T &getValue(unsigned int position) const
        {
            return this->data[position].value;
        }

        //! Return index at given position.
        unsigned int getIndex(unsigned int position) const
        {
            return this->data[position].index;
        }

        //! Return vector of position indexes.
        std::vector<unsigned int> getIndexes(void) const
        {
            std::vector<unsigned int> idxList;

            idxList.resize(this->data.size());

            for (unsigned int i=0;i<this->data.size();i++)
            {
                idxList[i] = this->data[i].index;
            }

            return idxList;
        }

        //! Add value.
        //! If value with given index already exist value will be added to its current value.
        void addValue(unsigned int index, T value)
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
        //! nElements difinex minimum size of the vector.
        std::vector<T> getValues(unsigned int nElements) const
        {
            std::vector<T> values;
            values.resize(nElements,0.0);

            for (unsigned int i=0;i<this->data.size();i++)
            {
                if (this->data[i].index > nElements)
                {
                    nElements = this->data[i].index + 1;
                    values.resize(nElements,0.0);
                }
                values[this->data[i].index] = this->data[i].value;
            }
            return values;
        }

        //! Reserve vector size.
        void reserve(unsigned int nElements)
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
