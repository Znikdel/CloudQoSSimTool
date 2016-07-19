/*
 * TCP_New.cc
 *
 *  Created on: Jul 13, 2016
 *      Author: nishant
 */

#include "TCP_New.h"
Define_Module(TCP_New);

TCP_New::TCP_New(){

}
TCP_New::TCP_New(string newLocalIP, cGate* toTCP, NetworkService *netService){
    localIP = newLocalIP;
    outGate_TCP = toTCP;
    networkService = netService;
}

TCP_New::TCP_New(string newLocalIP, cGate* toTCP, cGate* toicancloudAPI, NetworkService *netService){

        // Init...
        localIP = newLocalIP;
        outGate_TCP = toTCP;
        outGate_icancloudAPI = toicancloudAPI;
        networkService = netService;
}

TCP_New::~TCP_New(){
    socketMap.deleteSockets ();
    connections.clear();
}

void TCP_New::createConnection(icancloud_Message *sm){

    clientTCP_Connector newConnection;
    icancloud_App_NET_Message *sm_net;


        // Cast to icancloud_App_NET_Message
        sm_net = dynamic_cast<icancloud_App_NET_Message *>(sm);

        // Wrong message?
        if (sm_net == NULL)
            networkService->showErrorMessage ("[createConnection] Error while casting to icancloud_App_NET_Message!");

        // Attach the message to the corresponding connection
        newConnection.msg = sm;

        // Create the socket
        newConnection.socket = new TCPSocket();
        newConnection.socket->setDataTransferMode(TCP_TRANSFER_OBJECT);
        newConnection.socket->bind(*(localIP.c_str()) ? IPvXAddress(localIP.c_str()) : IPvXAddress(), -1);
        newConnection.socket->setCallbackObject(this);
        newConnection.socket->setOutputGate(outGate_TCP);
        newConnection.socket->renewSocket();

        // Add new socket to socketMap
        socketMap.addSocket (newConnection.socket);

        // Add new connection to vector
        connections.push_back (newConnection);

        // Debug...
        if (DEBUG_TCP_Service_Client)
            networkService->showDebugMessage ("[createConnection] local IP:%s ---> %s:%d. %s",
                                                localIP.c_str(),
                                                sm_net->getDestinationIP(),
                                                sm_net->getDestinationPort(),
                                                sm_net->contentsToString(DEBUG_TCP_Service_MSG_Client).c_str());


        // Establish Connection
        newConnection.socket->connect(IPvXAddressResolver().resolve(sm_net->getDestinationIP()), sm_net->getDestinationPort());
}


void TCP_New::sendPacketToServer(icancloud_Message *sm){

     int index;
        // Search for the connection...
        index = searchConnectionByConnId (sm->getConnectionId());

        // Connection not found?
        if (index == NOT_FOUND)
            networkService->showErrorMessage ("[sendPacketToServer] Socket not found!");

        else{

            if (DEBUG_TCP_Service_Client)
                networkService->showDebugMessage ("[sendPacketToServer] Sending message to %s:%d. %s",
                                                    connections[index].socket->getRemoteAddress().str().c_str(),
                                                    connections[index].socket->getRemotePort(),
                                                    sm->contentsToString(DEBUG_TCP_Service_MSG_Client).c_str());

            connections[index].socket->send(sm);
        }
}


void TCP_New::closeConnection(icancloud_Message *sm){

    TCPSocket *socket;

        // Search for the socket
        socket = socketMap.findSocketFor(sm);

             if (!socket)
                networkService->showErrorMessage ("[closeConnection] Socket not found to send the message: %s\n", sm->contentsToString(true).c_str());
             else{

                if (DEBUG_TCP_Service_Client)
                    networkService->showDebugMessage ("Closing connection %s:%d. %s",
                                                socket->getRemoteAddress().str().c_str(),
                                                socket->getRemotePort(),
                                                sm->contentsToString(DEBUG_TCP_Service_MSG_Client).c_str());
                socket->close();

                socket = socketMap.removeSocket(socket);

                delete(socket);
             }

             delete (sm);
}


int TCP_New::searchConnectionByConnId(int connId){

    int result;
    bool found;
    unsigned int i;

        // Init
        found = false;
        i = 0;

            // Search for the connection...
            while ((!found) && (i<connections.size())){

                if (connections[i].socket->getConnectionId() == connId)
                    found = true;
                else
                    i++;
            }

            // Set the result
            if (found)
                result = i;
            else
                result = NOT_FOUND;

    return result;
}


TCPSocket* TCP_New::getInvolvedSocket (cMessage *msg){
    return (socketMap.findSocketFor(msg));
}

void TCP_New::newListenConnection (icancloud_Message *sm){
//
//    TCPSocket *newSocket;
//    icancloud_App_NET_Message *sm_net;
//    serverTCP_Connector newConnection;
//
//
//        // Cast to icancloud_App_NET_Message
//        sm_net = dynamic_cast<icancloud_App_NET_Message *>(sm);
//
//        // Wrong message?
//        if (sm_net == NULL)
//            networkService->showErrorMessage ("[TCP_ServerSideService::newListenConnection] Error while casting to icancloud_App_NET_Message!");
//
//        // Exists a previous connection listening at the same port
//        if (existsConnection(sm_net->getLocalPort()))
//            networkService->showErrorMessage ("[TCP_ServerSideService::newListenConnection] This connection already exists. Port:%d!", sm_net->getLocalPort());
//
//        // Connection entry
//        newConnection.port = sm_net->getLocalPort();
//        newConnection.appIndex = sm_net->getNextModuleIndex();
//        connection.push_back (newConnection);
//
//        // Create a new socket
//        newSocket = new TCPSocket();
//
//        // Listen...
//
//        newSocket->setOutputGate(outGate_TCP);
//        newSocket->setDataTransferMode(TCP_TRANSFER_OBJECT);
//        newSocket->bind(*(localIP.c_str()) ? IPvXAddress(localIP.c_str()) : IPvXAddress(), sm_net->getLocalPort());
//        newSocket->setCallbackObject(this);
//        newSocket->listen();
//
//        // Debug...
//        if (DEBUG_TCP_Service_Server)
//            networkService->showDebugMessage ("[TCP_ServerSideService] New connection for App[%d] is listening on %s:%d",
//                                                sm_net->getNextModuleIndex(),
//                                                localIP.c_str(),
//                                                sm_net->getLocalPort());
//
//        // Delete msg
//        delete (sm);
}

void TCP_New::arrivesIncommingConnection (cMessage *msg){

    TCPSocket *socket;

        // Search an existing connection...
        socket = socketMap.findSocketFor(msg);

        // Connection does not exist. Create a new one!
        if (!socket){

            // Create a new socket
            socket = new TCPSocket(msg);
            socket->setOutputGate(outGate_TCP);
            socket->setDataTransferMode(TCP_TRANSFER_OBJECT);
            socket->setCallbackObject(this);
            socketMap.addSocket(socket);

            if (DEBUG_TCP_Service_Server)
            networkService->showDebugMessage ("[TCP_ServerSideService] Arrives an incoming connection from %s:%d to local connection %s:%d with connId = %d",
                                                    socket->getRemoteAddress().str().c_str(),
                                                    socket->getRemotePort(),
                                                    socket->getLocalAddress().str().c_str(),
                                                    socket->getLocalPort(),
                                                    socket->getConnectionId());

            // Process current operation!
            socket->processMessage(msg);
        }
        else
            networkService->showErrorMessage ("[TCP_ServerSideService::arrivesIncommingConnection] Connection already exists. ConnId =  %d", socket->getConnectionId());
}

void TCP_New::sendPacketToClient(icancloud_Message *sm){

    TCPSocket *socket;

        // Search for the socket
        socket = socketMap.findSocketFor(sm);

             if (!socket)
                networkService->showErrorMessage ("[sendPacketToClient] Socket not found to send the message: %s\n", sm->contentsToString(true).c_str());

             else{

                if (DEBUG_TCP_Service_Server)
                    networkService->showDebugMessage ("Sending message to client %s:%d. %s",
                                                socket->getRemoteAddress().str().c_str(),
                                                socket->getRemotePort(),
                                                sm->contentsToString(DEBUG_TCP_Service_MSG_Server).c_str());

                delete sm->removeControlInfo();
                TCPSendCommand *cmd = new TCPSendCommand();
                cmd->setConnId(socket->getConnectionId());
                sm->setControlInfo(cmd);
                sm->setKind (TCP_C_SEND);
                networkService->sendResponseMessage (sm);

             }
}



void TCP_New::socketEstablished(int connId, void *ptr){

}

void TCP_New::socketDataArrived(int connId, void *ptr, cPacket *msg, bool urgent){

}


void TCP_New::socketPeerClosed(int, void *){
    if (DEBUG_TCP_Service_Client)
        networkService->showDebugMessage ("Socket Peer closed");
}


void TCP_New::socketClosed(int connId, void *ptr){
    if (DEBUG_TCP_Service_Client)
        networkService->showDebugMessage ("Socket closed");
}


void TCP_New::socketFailure(int connId, void *ptr, int code){
    if (DEBUG_TCP_Service_Client)
        networkService->showDebugMessage ("Socket Failure");
}


