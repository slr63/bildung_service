
include $(POCO_BASE)/build/rules/global

CFLAGS    += -I../../../odb4cpp/include -I/usr/include/mysql  -I/usr/local/include/ctpp2
CXXFLAGS  += -I../../../odb4cpp/include -I/usr/include/mysql -I/usr/local/include -I/usr/local -I../../../shttpd/include  -I../../../libdrizzle5.1/include -std=c++11
SYSLIBS += -L$(ACE_ROOT)/lib  -lboost_serialization -lctpp2 -lmysqlclient -lcurl -lgsoap++



objects = bildung_service bildung_service soapAPIBildungsPortalSoapService soapC soapAPIBildungsPortalSoapProxy PortalApp main

#soapAPIBildungsPortalSoapProxy 
 
#JConvertor
#objects = taskwork

target         = bildung_service
target_version = 1
target_libs    = PocoFoundation PocoXML PocoNet PocoUtil odb4cpp PocoData PocoDataODBC PocoDataMySQL

include $(POCO_BASE)/build/rules/exec
