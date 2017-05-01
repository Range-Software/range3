/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   session_node_id.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th February 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Session Node ID class declaration                   *
 *********************************************************************/

#ifndef __SESSION_NODE_ID_H__
#define __SESSION_NODE_ID_H__

#include <QtGlobal>

class SessionNodeID
{

    protected:
        uint mid;
        uint nid;

    private:

        //! Internal initialization function.
        void _init(const SessionNodeID *pSessionNodeID = 0);

    public:

        //! Constructor.
        SessionNodeID();

        //! Constructor.
        SessionNodeID(uint mid, uint nid);

        //! Copy constructor.
        SessionNodeID(const SessionNodeID &sessionNodeID);

        //! Destructor.
        ~SessionNodeID();

        //! Assignment operator.
        SessionNodeID &operator =(const SessionNodeID &sessionNodeID);

        //! Compare operator.
        bool operator ==(const SessionNodeID &sessionNodeID) const;

        //! Compare less operator.
        bool operator <(const SessionNodeID &sessionNodeID) const;

        //! Compare greater operator.
        bool operator >(const SessionNodeID &sessionNodeID) const;

        //! Return model ID.
        uint getMid(void) const;

        //! Set model ID.
        void setMid(uint mid);

        //! Return node ID.
        uint getNid(void) const;

        //! Set node ID.
        void setNid(uint nid);

};

#endif // __SESSION_NODE_ID_H__
