/*
 * TCP_test.h
 *
 *  Created on: May 12, 2016
 *      Author: nishant
 */

#ifndef TCP_TEST_H_
#define TCP_TEST_H_

#include <omnetpp.h>
#include "HWEnergyInterface.h"
#include "TCP_ClientSideService.h"
#include "TCP_ServerSideService.h"
#include "icancloud_App_NET_Message.h"

#include "UserJob.h"
#define     NETWORK_ON  "network_on"
#define     NETWORK_OFF "network_off"

class TCP_ClientSideService;
class TCP_ServerSideService;

class TCP_test : public UserJob{
    /** Local IP address */
            string localIP;
            unsigned int startDelay;
            /** TCP Client-side Services */
            TCP_ClientSideService *clientTCP_Services;

            /** TCP Server-side Services */
            TCP_ServerSideService *serverTCP_Services;

            /** Input gate from Service Redirector */
            cGate* fromNetManagerGate;

            /** Input gate from Network (TCP) */
            cGate* fromNetTCPGate;

            /** Output gate to Service Redirector */
            cGate* toNetManagerGate;

            /** Output gate to Network (TCP) */
            cGate* toNetTCPGate;

            /** Node state */
        //    string nodeState;

            vector <icancloud_Message*> sm_vector;
            int lastOp;

            /**
            * Destructor.
            */
            ~TCP_test();

           /**
            *  Module initialization.
            */
            void initialize();

           /**
            * Module ending.
            */
            void finish();

            //void startExcecution();
            void startExecution (int pid);
            void changeState (string energyState,unsigned componentIndex = 0);

        private:


           /**
            * This method classifies an incoming message and invokes the corresponding method
            * to process it.
            *
            * For self-messages, it invokes processSelfMessage(sm);
            * For request-messages, it invokes processRequestMessage(sm);
            * For response-messages, it invokes processResponseMessage(sm);
            */
            void handleMessage (cMessage *msg);

           /**
            * Get the outGate ID to the module that sent <b>msg</b>
            * @param msg Arrived message.
            * @return. Gate Id (out) to module that sent <b>msg</b> or NOT_FOUND if gate not found.
            */
            cGate* getOutGate (cMessage *msg);

           /**
            * Process a self message.
            * @param msg Self message.
            */
            void processSelfMessage (cMessage *msg);

           /**
            * Process a request message.
            * @param sm Request message.
            */
            void processRequestMessage (icancloud_Message *sm);

           /**
            * Process a response message.
            * @param sm Request message.
            */
            void processResponseMessage (icancloud_Message *sm);

            /**
             * Receiving an established connection message.
             */
            void receivedEstablishedConnection (cMessage *msg);


            /*
             * Change the energy state of the memory given by node state
             */
            void changeDeviceState (string state, unsigned componentIndex = 0);

            /*
             *  Change the energy state of the memory
             */


};

#endif /* TCP_TEST_H_ */
