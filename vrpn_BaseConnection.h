////////////////////////////////////////////////////////
//
// class vrpn_BaseConnection
//
// base class for classes that provide a single connection,
// like vrpn_NetConnection.
//
// this class used to be called vrpn_OneConnection.
// now, it is an abstract base class for different kinds
// of connections, including a NetConnection and 
// probably a FileConnection.


#ifndef VRPN_BASECONNECTION_INCLUDED
#define VRPN_BASECONNECTION_INCLUDED

#include "vrpn_ConnectionOldCommonStuff.h"


class vrpn_BaseConnection
{
public:  // c'tors and d'tors
    vrpn_BaseConnection();
    virtual ~vrpn_BaseConnection();

public:  // status
    
    // a connection was made
    virtual int connected() const;

    // no errors
    virtual int doing_okay() const;

public: // clock stuff
    virtual int time_since_connection_open( timeval * elapsed_time );

    // has a clock sync occured yet (slight prob of false negative, but
    // only for a brief period)
    virtual int clockSynched();

protected: // clock stuff
    timeval tvClockOffset;

public:  // sending and receiving


	// *** N.B. ***
	// i think that these should be implemented in the derived classes
	// so i added the virtual keword and made them abstract
	// 
	// stefan


    // functions for sending messages and receiving messages
    // the ConnectionController will call these functions

    virtual int handle_outgoing_message( /*...XXX...*/ ) = 0;

    virtual int handle_incoming_message( /*...XXX...*/ ) = 0;


public:  // public type_id and sender_id stuff

    // * register a new local {type,sender} that that controller
    //   has assigned a {type,sender}_id to.
    // * in addition, look to see if this {type,sender} has
    //   already been registered remotely (newRemoteType/Sender)
    // * if so, record the correspondence so that
    //   local_{type,sender}_id() can do its thing
    // * XXX proposed new name:
    //         register_local_{type,sender}
    //
    //Return 1 if this {type,sender} was already registered
    //by the other side, 0 if not.

    // was: newLocalSender
    virtual int register_local_sender(
        const char *sender_name,  // e.g. "tracker0"
        vrpn_int32 local_id );    // from controller
    
    // was: newLocalType
    virtual int register_local_type(
        const char *type_name,   // e.g. "tracker_pos"
        vrpn_int32 local_id );   // from controller


    // Adds a new remote type/sender and returns its index.
    // Returns -1 on error.
    // * called by the ConnectionController when the peer on the
    //   other side of this connection has sent notification
    //   that it has registered a new type/sender
    // * don't call this function if the type/sender has
    //   already been locally registered
    // * XXX proposed new name:
    //         register_remote_{type,sender}
    
    // Adds a new remote type/sender and returns its index
    // was: newRemoteSender
    virtual int register_remote_sender(
        const cName sender_name,  // e.g. "tracker0"
        vrpn_int32 local_id );    // from controller
        
    // Adds a new remote type/sender and returns its index
    // was: newRemoteType
    virtual int register_remote_type(
        const cName type_name,    // e.g. "tracker_pos"
        vrpn_int32 local_id );    // from controller


    // Give the local mapping for the remote type or sender.
    // Returns -1 if there isn't one.
    // Pre: have already registered this type/sender remotely
    //      and locally using register_local_{type,sender}
    //      and register_remote_{type_sender}
    // * XXX proposed new name:
    //         translate_remote_{type,sender}_to_local


    // Give the local mapping for the remote type
    // was: local_type_id
    virtual int translate_remote_type_to_local(
        vrpn_int32 remote_type );
    
    // Give the local mapping for the remote sender
    // was: local_sender_id
    virtual int translate_remote_sender_to_local(
        vrpn_int32 remote_sender );


    // XXX todo
    // * check into why one of the register functions
    //   uses char * while the other uses char[100]
    // * make name const for both register functions

protected: // protected type_id and sender_id stuff

    // Holds one entry for a mapping of remote strings to local IDs
    struct cRemoteMapping {
	// remote/local sender/type equivalence
	// is based on name string comparison
	char * name;
	
	// each side assigns an id to each name
	// the id's may differ on the two ends of a connection
        vrpn_int32 local_id;
    };
    
    
    // * the number of senders that have been registered by the
    //   other side of the connection
    // * was: num_other_senders
    vrpn_int32 num_registered_remote_senders;

    // * The senders we know about that have been described by
    //   the other end of the connection.
    // * indexed by the ID from the other side, and store
    //   the name and local ID that corresponds to each.
    // * was: other_senders
    cRemoteMapping registered_remote_senders[vrpn_CONNECTION_MAX_SENDERS];
        

    // * the number of types that have been registered by the
    //   other side of the connection
    // * was: num_other_types
    vrpn_int32 num_registered_remote_types;
    
    // * The types we know about that have been described by
    //   the other end of the connection.
    // * indexed by the ID from the other side, and store
    //   the name and local ID that corresponds to each.
    // * was: other_types
    cRemoteMapping registered_remote_types[vrpn_CONNECTION_MAX_TYPES];


public: // logging stuff

    // we will support three different kinds of logging
    // the interface to logging needs to be enriched
    // in order to facilitate this


public: // todo items

    // * some way to querry the name of a connection, for display
    //   purposes in the hiball control panel, etc.

};


#endif