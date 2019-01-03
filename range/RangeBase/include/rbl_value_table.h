/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_value_table.h                                        *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Value table class declaration                       *
 *********************************************************************/

#ifndef RBL_VALUE_TABLE_H
#define RBL_VALUE_TABLE_H

#include <QString>
#include <map>

//! Value table class
class RValueTable
{

    private:

        //! Internal initialization function.
        void _init ( const RValueTable *pValueTable = nullptr );

    protected:

        //! Value table key name.
        QString keyName;
        //! Value table key units.
        QString keyUnits;
        //! Value table value name.
        QString valueName;
        //! Value table value units.
        QString valueUnits;
        //! Table.
        std::map<double,double> table;

    public:

        //! Constructor.
        RValueTable ();

        //! Copy constructor.
        RValueTable ( const RValueTable &valueTable );

        //! Destructor.
        ~RValueTable ();

        //! Return table key name.
        const QString & getKeyName ( void ) const;

        //! Set table key name.
        void setKeyName ( const QString &name );

        //! Return table key units.
        const QString & getKeyUnits ( void ) const;

        //! Set table key units.
        void setKeyUnits ( const QString &units );

        //! Return table value name.
        const QString & getValueName ( void ) const;

        //! Set table value name.
        void setValueName ( const QString &name );

        //! Return table value units.
        const QString & getValueUnits ( void ) const;

        //! Set table value units.
        void setValueUnits ( const QString &units );

        //! Return number of values in the vector.
        unsigned int size ( void ) const;

        //! Add new item to the value vector.
        //! If such key already exist it will be replaced by value.
        void add ( double key,
                   double value = 0.0 );

        //! Remove item specified by given key from the value vector.
        void remove ( double key );

        //! Remove item at given position from the value vector.
        void remove ( unsigned int position );

        //! Return value for a given key.
        //! If such key is not fund an interpolated value is returned.
        double get ( double key ) const;

        //! Return key at given position.
        double getKey ( unsigned int position ) const;

        //! Return value at given position.
        double getValue ( unsigned int position ) const;

        //! Drop all keys and values
        inline void clear ( void )
        {
            this->table.clear();
        }

        //! Test if table is empty.
        inline bool empty ( void ) const
        {
            return this->table.empty();
        }

        //! Assignment operator.
        RValueTable & operator = ( const RValueTable &valueTable );

        //! Equal operator.
        bool operator == ( const RValueTable &valueTable ) const;

        //! Not equal operator.
        bool operator != ( const RValueTable &valueTable ) const;

};

#endif /* RBL_VALUE_TABLE_H */
