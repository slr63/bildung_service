/* soapAPIBildungsPortalSoapService.cpp
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

#include "soapAPIBildungsPortalSoapService.h"

APIBildungsPortalSoapService::APIBildungsPortalSoapService()
{	this->soap = soap_new();
	this->soap_own = true;
	APIBildungsPortalSoapService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

APIBildungsPortalSoapService::APIBildungsPortalSoapService(const APIBildungsPortalSoapService& rhs)
{	this->soap = rhs.soap;
	this->soap_own = false;
}

APIBildungsPortalSoapService::APIBildungsPortalSoapService(struct soap *_soap)
{	this->soap = _soap;
	this->soap_own = false;
	APIBildungsPortalSoapService_init(_soap->imode, _soap->omode);
}

APIBildungsPortalSoapService::APIBildungsPortalSoapService(soap_mode iomode)
{	this->soap = soap_new();
	this->soap_own = true;
	APIBildungsPortalSoapService_init(iomode, iomode);
}

APIBildungsPortalSoapService::APIBildungsPortalSoapService(soap_mode imode, soap_mode omode)
{	this->soap = soap_new();
	this->soap_own = true;
	APIBildungsPortalSoapService_init(imode, omode);
}

APIBildungsPortalSoapService::~APIBildungsPortalSoapService()
{	if (this->soap_own)
	{	this->destroy();
		soap_free(this->soap);
	}
}

void APIBildungsPortalSoapService::APIBildungsPortalSoapService_init(soap_mode imode, soap_mode omode)
{	soap_imode(this->soap, imode);
	soap_omode(this->soap, omode);
	static const struct Namespace namespaces[] = {
        { "SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL },
        { "SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL },
        { "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL },
        { "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL },
        { "ns1", "http://www.lsb-niedersachsen.de/bildungsportal/api/", NULL, NULL },
        { NULL, NULL, NULL, NULL}
    };
	soap_set_namespaces(this->soap, namespaces);
}

void APIBildungsPortalSoapService::destroy()
{	soap_destroy(this->soap);
	soap_end(this->soap);
}

void APIBildungsPortalSoapService::reset()
{	this->destroy();
	soap_done(this->soap);
	soap_initialize(this->soap);
	APIBildungsPortalSoapService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

#ifndef WITH_PURE_VIRTUAL
APIBildungsPortalSoapService *APIBildungsPortalSoapService::copy()
{	APIBildungsPortalSoapService *dup = SOAP_NEW_UNMANAGED(APIBildungsPortalSoapService);
	if (dup)
	{	soap_done(dup->soap);
		soap_copy_context(dup->soap, this->soap);
	}
	return dup;
}
#endif

APIBildungsPortalSoapService& APIBildungsPortalSoapService::operator=(const APIBildungsPortalSoapService& rhs)
{	if (this->soap != rhs.soap)
	{	if (this->soap_own)
			soap_free(this->soap);
		this->soap = rhs.soap;
		this->soap_own = false;
	}
	return *this;
}

int APIBildungsPortalSoapService::soap_close_socket()
{	return soap_closesock(this->soap);
}

int APIBildungsPortalSoapService::soap_force_close_socket()
{	return soap_force_closesock(this->soap);
}

int APIBildungsPortalSoapService::soap_senderfault(const char *string, const char *detailXML)
{	return ::soap_sender_fault(this->soap, string, detailXML);
}

int APIBildungsPortalSoapService::soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_sender_fault_subcode(this->soap, subcodeQName, string, detailXML);
}

int APIBildungsPortalSoapService::soap_receiverfault(const char *string, const char *detailXML)
{	return ::soap_receiver_fault(this->soap, string, detailXML);
}

int APIBildungsPortalSoapService::soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_receiver_fault_subcode(this->soap, subcodeQName, string, detailXML);
}

void APIBildungsPortalSoapService::soap_print_fault(FILE *fd)
{	::soap_print_fault(this->soap, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void APIBildungsPortalSoapService::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this->soap, os);
}
#endif

char *APIBildungsPortalSoapService::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this->soap, buf, len);
}
#endif

void APIBildungsPortalSoapService::soap_noheader()
{	this->soap->header = NULL;
}

::SOAP_ENV__Header *APIBildungsPortalSoapService::soap_header()
{	return this->soap->header;
}

#ifndef WITH_NOIO
int APIBildungsPortalSoapService::run(int port, int backlog)
{	if (!soap_valid_socket(this->soap->master) && !soap_valid_socket(this->bind(NULL, port, backlog)))
		return this->soap->error;
	for (;;)
	{	if (!soap_valid_socket(this->accept()))
		{	if (this->soap->errnum == 0) // timeout?
				this->soap->error = SOAP_OK;
			break;
		}
		if (this->serve())
			break;
		this->destroy();
	}
	return this->soap->error;
}

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
int APIBildungsPortalSoapService::ssl_run(int port, int backlog)
{	if (!soap_valid_socket(this->soap->master) && !soap_valid_socket(this->bind(NULL, port, backlog)))
		return this->soap->error;
	for (;;)
	{	if (!soap_valid_socket(this->accept()))
		{	if (this->soap->errnum == 0) // timeout?
				this->soap->error = SOAP_OK;
			break;
		}
		if (this->ssl_accept() || this->serve())
			break;
		this->destroy();
	}
	return this->soap->error;
}
#endif

SOAP_SOCKET APIBildungsPortalSoapService::bind(const char *host, int port, int backlog)
{	return soap_bind(this->soap, host, port, backlog);
}

SOAP_SOCKET APIBildungsPortalSoapService::accept()
{	return soap_accept(this->soap);
}

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
int APIBildungsPortalSoapService::ssl_accept()
{	return soap_ssl_accept(this->soap);
}
#endif
#endif

int APIBildungsPortalSoapService::serve()
{
#ifndef WITH_FASTCGI
	this->soap->keep_alive = this->soap->max_keep_alive + 1;
#endif
	do
	{
#ifndef WITH_FASTCGI
		if (this->soap->keep_alive > 0 && this->soap->max_keep_alive > 0)
			this->soap->keep_alive--;
#endif
		if (soap_begin_serve(this->soap))
		{	if (this->soap->error >= SOAP_STOP)
				continue;
			return this->soap->error;
		}
		if ((dispatch() || (this->soap->fserveloop && this->soap->fserveloop(this->soap))) && this->soap->error && this->soap->error < SOAP_STOP)
		{
#ifdef WITH_FASTCGI
			soap_send_fault(this->soap);
#else
			return soap_send_fault(this->soap);
#endif
		}
#ifdef WITH_FASTCGI
		soap_destroy(this->soap);
		soap_end(this->soap);
	} while (1);
#else
	} while (this->soap->keep_alive);
#endif
	return SOAP_OK;
}

static int serve___ns1__dumpTableFields(struct soap*, APIBildungsPortalSoapService*);
static int serve___ns1__getAllEvents(struct soap*, APIBildungsPortalSoapService*);
static int serve___ns1__getSingleEvent(struct soap*, APIBildungsPortalSoapService*);
static int serve___ns1__register(struct soap*, APIBildungsPortalSoapService*);
static int serve___ns1__dumpTableFields_(struct soap*, APIBildungsPortalSoapService*);
static int serve___ns1__getAllEvents_(struct soap*, APIBildungsPortalSoapService*);
static int serve___ns1__getSingleEvent_(struct soap*, APIBildungsPortalSoapService*);
static int serve___ns1__register_(struct soap*, APIBildungsPortalSoapService*);

int APIBildungsPortalSoapService::dispatch()
{	return dispatch(this->soap);
}

int APIBildungsPortalSoapService::dispatch(struct soap* soap)
{
	APIBildungsPortalSoapService_init(soap->imode, soap->omode);
	(void)soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "ns1:dumpTableFields"))
		return serve___ns1__dumpTableFields(soap, this);
	if (!soap_match_tag(soap, soap->tag, "ns1:getAllEvents"))
		return serve___ns1__getAllEvents(soap, this);
	if (!soap_match_tag(soap, soap->tag, "ns1:getSingleEvent"))
		return serve___ns1__getSingleEvent(soap, this);
	if (!soap_match_tag(soap, soap->tag, "ns1:register"))
		return serve___ns1__register(soap, this);
	if (!soap_match_tag(soap, soap->tag, "ns1:dumpTableFields"))
		return serve___ns1__dumpTableFields_(soap, this);
	if (!soap_match_tag(soap, soap->tag, "ns1:getAllEvents"))
		return serve___ns1__getAllEvents_(soap, this);
	if (!soap_match_tag(soap, soap->tag, "ns1:getSingleEvent"))
		return serve___ns1__getSingleEvent_(soap, this);
	if (!soap_match_tag(soap, soap->tag, "ns1:register"))
		return serve___ns1__register_(soap, this);
	return soap->error = SOAP_NO_METHOD;
}

static int serve___ns1__dumpTableFields(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__dumpTableFields soap_tmp___ns1__dumpTableFields;
	_ns1__dumpTableFieldsResponse ns1__dumpTableFieldsResponse;
	ns1__dumpTableFieldsResponse.soap_default(soap);
	soap_default___ns1__dumpTableFields(soap, &soap_tmp___ns1__dumpTableFields);
	if (!soap_get___ns1__dumpTableFields(soap, &soap_tmp___ns1__dumpTableFields, "-ns1:dumpTableFields", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->dumpTableFields(soap_tmp___ns1__dumpTableFields.ns1__dumpTableFields, ns1__dumpTableFieldsResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__dumpTableFieldsResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__dumpTableFieldsResponse.soap_put(soap, "ns1:dumpTableFieldsResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__dumpTableFieldsResponse.soap_put(soap, "ns1:dumpTableFieldsResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns1__getAllEvents(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__getAllEvents soap_tmp___ns1__getAllEvents;
	_ns1__getAllEventsResponse ns1__getAllEventsResponse;
	ns1__getAllEventsResponse.soap_default(soap);
	soap_default___ns1__getAllEvents(soap, &soap_tmp___ns1__getAllEvents);
	if (!soap_get___ns1__getAllEvents(soap, &soap_tmp___ns1__getAllEvents, "-ns1:getAllEvents", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->getAllEvents(soap_tmp___ns1__getAllEvents.ns1__getAllEvents, ns1__getAllEventsResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__getAllEventsResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__getAllEventsResponse.soap_put(soap, "ns1:getAllEventsResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__getAllEventsResponse.soap_put(soap, "ns1:getAllEventsResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns1__getSingleEvent(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__getSingleEvent soap_tmp___ns1__getSingleEvent;
	_ns1__getSingleEventResponse ns1__getSingleEventResponse;
	ns1__getSingleEventResponse.soap_default(soap);
	soap_default___ns1__getSingleEvent(soap, &soap_tmp___ns1__getSingleEvent);
	if (!soap_get___ns1__getSingleEvent(soap, &soap_tmp___ns1__getSingleEvent, "-ns1:getSingleEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->getSingleEvent(soap_tmp___ns1__getSingleEvent.ns1__getSingleEvent, ns1__getSingleEventResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__getSingleEventResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__getSingleEventResponse.soap_put(soap, "ns1:getSingleEventResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__getSingleEventResponse.soap_put(soap, "ns1:getSingleEventResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns1__register(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__register soap_tmp___ns1__register;
	_ns1__registerResponse ns1__registerResponse;
	ns1__registerResponse.soap_default(soap);
	soap_default___ns1__register(soap, &soap_tmp___ns1__register);
	if (!soap_get___ns1__register(soap, &soap_tmp___ns1__register, "-ns1:register", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->register_(soap_tmp___ns1__register.ns1__register, ns1__registerResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__registerResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__registerResponse.soap_put(soap, "ns1:registerResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__registerResponse.soap_put(soap, "ns1:registerResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns1__dumpTableFields_(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__dumpTableFields_ soap_tmp___ns1__dumpTableFields_;
	_ns1__dumpTableFieldsResponse ns1__dumpTableFieldsResponse;
	ns1__dumpTableFieldsResponse.soap_default(soap);
	soap_default___ns1__dumpTableFields_(soap, &soap_tmp___ns1__dumpTableFields_);
	if (!soap_get___ns1__dumpTableFields_(soap, &soap_tmp___ns1__dumpTableFields_, "-ns1:dumpTableFields", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->dumpTableFields_(soap_tmp___ns1__dumpTableFields_.ns1__dumpTableFields, ns1__dumpTableFieldsResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__dumpTableFieldsResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__dumpTableFieldsResponse.soap_put(soap, "ns1:dumpTableFieldsResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__dumpTableFieldsResponse.soap_put(soap, "ns1:dumpTableFieldsResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns1__getAllEvents_(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__getAllEvents_ soap_tmp___ns1__getAllEvents_;
	_ns1__getAllEventsResponse ns1__getAllEventsResponse;
	ns1__getAllEventsResponse.soap_default(soap);
	soap_default___ns1__getAllEvents_(soap, &soap_tmp___ns1__getAllEvents_);
	if (!soap_get___ns1__getAllEvents_(soap, &soap_tmp___ns1__getAllEvents_, "-ns1:getAllEvents", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->getAllEvents_(soap_tmp___ns1__getAllEvents_.ns1__getAllEvents, ns1__getAllEventsResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__getAllEventsResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__getAllEventsResponse.soap_put(soap, "ns1:getAllEventsResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__getAllEventsResponse.soap_put(soap, "ns1:getAllEventsResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns1__getSingleEvent_(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__getSingleEvent_ soap_tmp___ns1__getSingleEvent_;
	_ns1__getSingleEventResponse ns1__getSingleEventResponse;
	ns1__getSingleEventResponse.soap_default(soap);
	soap_default___ns1__getSingleEvent_(soap, &soap_tmp___ns1__getSingleEvent_);
	if (!soap_get___ns1__getSingleEvent_(soap, &soap_tmp___ns1__getSingleEvent_, "-ns1:getSingleEvent", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->getSingleEvent_(soap_tmp___ns1__getSingleEvent_.ns1__getSingleEvent, ns1__getSingleEventResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__getSingleEventResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__getSingleEventResponse.soap_put(soap, "ns1:getSingleEventResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__getSingleEventResponse.soap_put(soap, "ns1:getSingleEventResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns1__register_(struct soap *soap, APIBildungsPortalSoapService *service)
{	struct __ns1__register_ soap_tmp___ns1__register_;
	_ns1__registerResponse ns1__registerResponse;
	ns1__registerResponse.soap_default(soap);
	soap_default___ns1__register_(soap, &soap_tmp___ns1__register_);
	if (!soap_get___ns1__register_(soap, &soap_tmp___ns1__register_, "-ns1:register", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = service->register_(soap_tmp___ns1__register_.ns1__register, ns1__registerResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	ns1__registerResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns1__registerResponse.soap_put(soap, "ns1:registerResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns1__registerResponse.soap_put(soap, "ns1:registerResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}
/* End of server object code */
