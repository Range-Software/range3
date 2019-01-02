/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_distance_vector.h                                    *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Distance vector class declaration                   *
 *********************************************************************/

#ifndef RBL_DISTANCE_VECTOR_H
#define RBL_DISTANCE_VECTOR_H

#include <vector>
#include <set>
#include <algorithm>

template <class T>
class RDistanceVector : public std::vector<T>
{

    private:

        //! Internal initialization function.
        void _init(const RDistanceVector *pDistanceVector = nullptr)
        {
            if (pDistanceVector)
            {

            }
        }

    public:

        //! Constructor.
        RDistanceVector(unsigned int n = 0, const T &value = T())
            : std::vector<T>(n,value)
        {
            this->_init();
        }

        //! Copy constructor.
        RDistanceVector(const RDistanceVector &distanceVector)
            : std::vector<T>(distanceVector)
        {
            this->_init(&distanceVector);
        }

        //! Destructor.
        ~RDistanceVector()
        {

        }

        //! Assignment operator.
        RDistanceVector &operator =(const RDistanceVector &distanceVector)
        {
            this->std::vector<T>::operator=(distanceVector);
            this->_init(&distanceVector);
            return (*this);
        }

        //! Return set of distances.
        std::set<T> getDistances(void) const
        {
            std::set<T> distances;

            for (unsigned int i=0;i<this->size();i++)
            {
                distances.insert(this->at(i));
            }

            return distances;
        }

        //! Return vector of positions with given distance.
        std::vector<unsigned int> getDistancePositions(const T &distance) const
        {
            std::vector<unsigned int> positions;

            for (unsigned int i=0;i<this->size();i++)
            {
                if (distance == this->at(i))
                {
                    positions.push_back(i);
                }
            }

            return positions;
        }

        //! Find minimum distance.
        T findMinimumDistance(void) const
        {
            if (this->size() == 0)
            {
                return T();
            }

            unsigned int minPosition = 0;

            for (unsigned int i=1;i<this->size();i++)
            {
                if (this->at(minPosition) > this->at(i))
                {
                    minPosition = i;
                }
            }

            return this->at(minPosition);
        }

        //! Find maximum distance.
        T findMaximumDistance(void) const
        {
            if (this->size() == 0)
            {
                return T();
            }

            unsigned int maxPosition = 0;

            for (unsigned int i=1;i<this->size();i++)
            {
                if (this->at(maxPosition) < this->at(i))
                {
                    maxPosition = i;
                }
            }

            return this->at(maxPosition);
        }

        //! Fill with values.
        void fill(const T &value)
        {
            std::fill(this->begin(),this->end(),value);
        }

};

#endif // RBL_DISTANCE_VECTOR_H
