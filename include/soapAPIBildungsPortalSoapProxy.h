/* soapAPIBildungsPortalSoapProxy.h
   Generated by gSOAP 2.8.104 for bildung.h

gSOAP XML Web services tools
Copyright (C) 2000-2020, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapAPIBildungsPortalSoapProxy_H
#define soapAPIBildungsPortalSoapProxy_H
#include "soapH.h"

    class SOAP_CMAC APIBildungsPortalSoapProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'APIBildungsPortalSoapProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in bildung.h, if any
        /// Construct a proxy with new managing context
        APIBildungsPortalSoapProxy();
        /// Copy constructor
        APIBildungsPortalSoapProxy(const APIBildungsPortalSoapProxy& rhs);
        /// Construct proxy given a shared managing context
        APIBildungsPortalSoapProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        APIBildungsPortalSoapProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        APIBildungsPortalSoapProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        APIBildungsPortalSoapProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        APIBildungsPortalSoapProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        APIBildungsPortalSoapProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~APIBildungsPortalSoapProxy();
        /// Initializer used by constructors
        virtual void APIBildungsPortalSoapProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual APIBildungsPortalSoapProxy *copy();
        /// Copy assignment
        APIBildungsPortalSoapProxy& operator=(const APIBildungsPortalSoapProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault subcode QName string (NULL when absent)
        virtual const char *soap_fault_subcode();
        /// Get SOAP Fault string/reason (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail XML string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        //
        /// Web service synchronous operation 'dumpTableFields' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int dumpTableFields(_ns1__dumpTableFields *ns1__dumpTableFields, _ns1__dumpTableFieldsResponse &ns1__dumpTableFieldsResponse) { return this->dumpTableFields(NULL, NULL, ns1__dumpTableFields, ns1__dumpTableFieldsResponse); }
        /// Web service synchronous operation 'dumpTableFields' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int dumpTableFields(const char *soap_endpoint_url, const char *soap_action, _ns1__dumpTableFields *ns1__dumpTableFields, _ns1__dumpTableFieldsResponse &ns1__dumpTableFieldsResponse) { return this->send_dumpTableFields(soap_endpoint_url, soap_action, ns1__dumpTableFields) || this->recv_dumpTableFields(ns1__dumpTableFieldsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_dumpTableFields' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_dumpTableFields(const char *soap_endpoint_url, const char *soap_action, _ns1__dumpTableFields *ns1__dumpTableFields);
        /// Web service asynchronous operation 'recv_dumpTableFields' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_dumpTableFields(_ns1__dumpTableFieldsResponse &ns1__dumpTableFieldsResponse);
        //
        /// Web service synchronous operation 'getAllEvents' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int getAllEvents(_ns1__getAllEvents *ns1__getAllEvents, _ns1__getAllEventsResponse &ns1__getAllEventsResponse) { return this->getAllEvents(NULL, NULL, ns1__getAllEvents, ns1__getAllEventsResponse); }
        /// Web service synchronous operation 'getAllEvents' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int getAllEvents(const char *soap_endpoint_url, const char *soap_action, _ns1__getAllEvents *ns1__getAllEvents, _ns1__getAllEventsResponse &ns1__getAllEventsResponse) { return this->send_getAllEvents(soap_endpoint_url, soap_action, ns1__getAllEvents) || this->recv_getAllEvents(ns1__getAllEventsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_getAllEvents' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_getAllEvents(const char *soap_endpoint_url, const char *soap_action, _ns1__getAllEvents *ns1__getAllEvents);
        /// Web service asynchronous operation 'recv_getAllEvents' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_getAllEvents(_ns1__getAllEventsResponse &ns1__getAllEventsResponse);
        //
        /// Web service synchronous operation 'getSingleEvent' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int getSingleEvent(_ns1__getSingleEvent *ns1__getSingleEvent, _ns1__getSingleEventResponse &ns1__getSingleEventResponse) { return this->getSingleEvent(NULL, NULL, ns1__getSingleEvent, ns1__getSingleEventResponse); }
        /// Web service synchronous operation 'getSingleEvent' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int getSingleEvent(const char *soap_endpoint_url, const char *soap_action, _ns1__getSingleEvent *ns1__getSingleEvent, _ns1__getSingleEventResponse &ns1__getSingleEventResponse) { return this->send_getSingleEvent(soap_endpoint_url, soap_action, ns1__getSingleEvent) || this->recv_getSingleEvent(ns1__getSingleEventResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_getSingleEvent' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_getSingleEvent(const char *soap_endpoint_url, const char *soap_action, _ns1__getSingleEvent *ns1__getSingleEvent);
        /// Web service asynchronous operation 'recv_getSingleEvent' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_getSingleEvent(_ns1__getSingleEventResponse &ns1__getSingleEventResponse);
        //
        /// Web service synchronous operation 'register' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int register_(_ns1__register *ns1__register, _ns1__registerResponse &ns1__registerResponse) { return this->register_(NULL, NULL, ns1__register, ns1__registerResponse); }
        /// Web service synchronous operation 'register' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int register_(const char *soap_endpoint_url, const char *soap_action, _ns1__register *ns1__register, _ns1__registerResponse &ns1__registerResponse) { return this->send_register_(soap_endpoint_url, soap_action, ns1__register) || this->recv_register_(ns1__registerResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_register' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_register_(const char *soap_endpoint_url, const char *soap_action, _ns1__register *ns1__register);
        /// Web service asynchronous operation 'recv_register' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_register_(_ns1__registerResponse &ns1__registerResponse);
        //
        /// Web service synchronous operation 'dumpTableFields' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int dumpTableFields_(_ns1__dumpTableFields *ns1__dumpTableFields, _ns1__dumpTableFieldsResponse &ns1__dumpTableFieldsResponse) { return this->dumpTableFields_(NULL, NULL, ns1__dumpTableFields, ns1__dumpTableFieldsResponse); }
        /// Web service synchronous operation 'dumpTableFields' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int dumpTableFields_(const char *soap_endpoint_url, const char *soap_action, _ns1__dumpTableFields *ns1__dumpTableFields, _ns1__dumpTableFieldsResponse &ns1__dumpTableFieldsResponse) { return this->send_dumpTableFields_(soap_endpoint_url, soap_action, ns1__dumpTableFields) || this->recv_dumpTableFields_(ns1__dumpTableFieldsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_dumpTableFields' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_dumpTableFields_(const char *soap_endpoint_url, const char *soap_action, _ns1__dumpTableFields *ns1__dumpTableFields);
        /// Web service asynchronous operation 'recv_dumpTableFields' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_dumpTableFields_(_ns1__dumpTableFieldsResponse &ns1__dumpTableFieldsResponse);
        //
        /// Web service synchronous operation 'getAllEvents' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int getAllEvents_(_ns1__getAllEvents *ns1__getAllEvents, _ns1__getAllEventsResponse &ns1__getAllEventsResponse) { return this->getAllEvents_(NULL, NULL, ns1__getAllEvents, ns1__getAllEventsResponse); }
        /// Web service synchronous operation 'getAllEvents' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int getAllEvents_(const char *soap_endpoint_url, const char *soap_action, _ns1__getAllEvents *ns1__getAllEvents, _ns1__getAllEventsResponse &ns1__getAllEventsResponse) { return this->send_getAllEvents_(soap_endpoint_url, soap_action, ns1__getAllEvents) || this->recv_getAllEvents_(ns1__getAllEventsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_getAllEvents' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_getAllEvents_(const char *soap_endpoint_url, const char *soap_action, _ns1__getAllEvents *ns1__getAllEvents);
        /// Web service asynchronous operation 'recv_getAllEvents' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_getAllEvents_(_ns1__getAllEventsResponse &ns1__getAllEventsResponse);
        //
        /// Web service synchronous operation 'getSingleEvent' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int getSingleEvent_(_ns1__getSingleEvent *ns1__getSingleEvent, _ns1__getSingleEventResponse &ns1__getSingleEventResponse) { return this->getSingleEvent_(NULL, NULL, ns1__getSingleEvent, ns1__getSingleEventResponse); }
        /// Web service synchronous operation 'getSingleEvent' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int getSingleEvent_(const char *soap_endpoint_url, const char *soap_action, _ns1__getSingleEvent *ns1__getSingleEvent, _ns1__getSingleEventResponse &ns1__getSingleEventResponse) { return this->send_getSingleEvent_(soap_endpoint_url, soap_action, ns1__getSingleEvent) || this->recv_getSingleEvent_(ns1__getSingleEventResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_getSingleEvent' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_getSingleEvent_(const char *soap_endpoint_url, const char *soap_action, _ns1__getSingleEvent *ns1__getSingleEvent);
        /// Web service asynchronous operation 'recv_getSingleEvent' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_getSingleEvent_(_ns1__getSingleEventResponse &ns1__getSingleEventResponse);
        //
    };
#endif