#include "APIBildungsPortalSoap.nsmap"
#include "soapH.h" /* generated with soapcpp2 */
#include "soapAPIBildungsPortalSoapService.h"
#include <sstream>
#include "utility.h"
#include "utf_conv.h"
#include "Poco/Util/Application.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/String.h"
#include "MySQLRecordSet.h"
#include "Common.h"

using Poco::Util::Application;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;

/*int main(int argc, char **argv)
{
  APIBildungsPortalSoapService bp;
  if (argc < 2)
    bp.serve();	// serve as CGI application 
  else
  {
    int port = atoi(argv[1]);
    if (!port)
    {
      fprintf(stderr, "Usage: bildung_server  <port>\n");
      exit(0);
    }
    // run iterative server on port until fatal error 
    if (bp.run(port))
    {
      bp.soap_stream_fault(std::cerr);
      exit(1);
    }
  }
  return 0;
} */

void APIBildungsPortalSoapService::createDataset(odb4cpp::MySQLDatabase *db)
{
    ds_.reset(new odb4cpp::MySQLDataset(db));
}

void APIBildungsPortalSoapService::setDebug(bool d)
{
    debug_ = d;
}

void APIBildungsPortalSoapService::log(const std::string &method,const std::string &msg)
{
    std::string lmsg_;

	Poco::LocalDateTime ex_now;
	std::string ex_dt(DateTimeFormatter::format(ex_now, DateTimeFormat::SORTABLE_FORMAT));
	lmsg_ += "[ "+ex_dt+" ] "+method+": "+msg;
    Application::instance().logger().information(lmsg_);
}

int APIBildungsPortalSoapService::dumpTableFields(_ns1__dumpTableFields *in, _ns1__dumpTableFieldsResponse &out)
{
    if (debug_)
        log("dumpTableFields","Anfang");
    try {
        if (in->tableName != NULL)
        {
            std::ostringstream qry,desc;
            qry << "describe "<<*in->tableName<<"\n";
            ds_->query(qry.str());
            while(!ds_->eof())
            {
                desc <<ds_->fv("Field")<<": "<<ds_->fv("Type")<<"\n";
                ds_->next();
            }
            out.dumpTableFieldsResult = new std::string(desc.str());
        }
    } catch (odb4cpp::DbErrors &e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        std::ostringstream m;
        m<<e.getMsg();
        int sz = m.str().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, m.str().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (odb4cpp::Error &e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        int sz = e.what().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.what().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (Poco::Exception & e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        int sz = e.displayText().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.displayText().c_str());
        return this->soap_senderfault(msg, NULL);        
    }
    if (debug_)
        log("dumpTableFields","Ende");
    return SOAP_OK;
}


void APIBildungsPortalSoapService::fillSubEvents(ns1__EventMini * tmp_res,odb4cpp::MySQLDataset *ds,bool simpleDate)
{
    if (debug_)
        log("fillSubEvents","Anfang");
    
    std::unique_ptr<odb4cpp::MySQLDataset> subds(dynamic_cast<odb4cpp::MySQLDataset *>(ds->getDatabase()->CreateDataset()));
    std::ostringstream qry;
    qry << "SELECT \n";
    qry << "CAST(dts AS UNSIGNED) AS id,\n";
    qry << "versteckterLG AS hidden,\n";
    qry << "OB2LSB AS number,\n";
    qry << "KSBNr AS ksbNumber,\n";
    qry << "LSBNr AS lsbNumber,\n";
    qry << "AB_DTS AS sportRegion,\n";
    qry << "DAUERVON AS dateFrom,\n";
    qry << "DAUERBIS AS dateTo,\n";
    qry << "ZEITVON AS timeFrom,\n";
    qry << "ZEITBIS AS timeTo\n";
    qry << "FROM Lehrgang_Grund\n";
    qry << "WHERE Ursprung="<<tmp_res->Id<<"\n";
    qry << "ORDER BY T1\n";
    subds->query(qry.str());
    while (!subds->eof())
    {
        ns1__EventBase * sub_res = new ns1__EventBase();
        fillEventBase(sub_res,subds.get(),false);
        tmp_res->SubEvents.push_back(sub_res);
        subds->next();
    }
    if (debug_)
        log("fillSubEvents","Ende");
}

void APIBildungsPortalSoapService::fillEventBase(ns1__EventBase * tmp_res,odb4cpp::MySQLDataset *ds,bool simpleDate)
{
    if (debug_)
        log("fillEventBase","Anfang");
    
        tmp_res->Id = ds->fv("id").get_asLong();
        tmp_res->Number = new std::string(ds->fv("number").get_asString());
        tmp_res->KSBNumber = new std::string(ds->fv("ksbNumber").get_asString());
        tmp_res->LSBNumber = new std::string(ds->fv("lsbNumber").get_asString());
        tmp_res->SportRegion = new unsigned int(ds->fv("sportRegion").get_asLong());

        std::string dateFrom = ds->fv("dateFrom").get_asString();
        std::string dateTo = ds->fv("dateTo").get_asString();
        if (simpleDate) {
            if (dateFrom == dateTo)
                tmp_res->Date = new std::string(dateFrom);
            else
            {
                dateFrom.append(" bis ");
                dateFrom.append(dateTo);
                tmp_res->Date = new std::string(dateFrom);
            }
        } else {
            if (dateFrom.size() > 0) {
                std::string sb = dateFrom;
                std::string timeFrom = ds->fv("timeFrom").get_asString();
                if (timeFrom.size() > 0) {
                    sb.append(", ");
                    sb.append(timeFrom);
                }
                std::string timeTo = ds->fv("timeTo").get_asString();
                if (dateTo == dateFrom) {
                    if (timeTo.size() > 0 && timeTo != timeFrom) {
                        sb.append(" bis ");
                        sb.append(timeTo);
                    }
                } else {
                    sb.append(" bis ");
                    sb.append(dateTo);
                    if (timeTo.size() > 0) {
                        sb.append(", ");
                        sb.append(timeTo);
                    }
                }
                tmp_res->Date = new std::string(sb);
            } 
        }

    if (debug_)
        log("fillEventBase","Ende");
}

void APIBildungsPortalSoapService::fillEventMini(ns1__EventMini * tmp_res,odb4cpp::MySQLDataset *ds,bool simpleDate)
{
    if (debug_)
        log("fillEventMini","Anfang");

        //ns1__EventBase
        fillEventBase(tmp_res,ds_.get(),true);
        
        //ns1__EventMini
        tmp_res->Title = new std::string(ds->fv("title").get_asString());
        tmp_res->TitleAddition = new std::string(ds->fv("titleAddition").get_asString());
        tmp_res->Subtitle = new std::string(ds->fv("subtitle").get_asString());
        tmp_res->SubtitleAddition = new std::string(ds->fv("subtitleAddition").get_asString());
        tmp_res->Category = new std::string(ds->fv("category").get_asString());
        tmp_res->SubCategory = new std::string(ds->fv("subCategory").get_asString());
        tmp_res->Type = new std::string(ds->fv("type").get_asString());
        tmp_res->VenueCity = new std::string(ds->fv("venueCity").get_asString());
        tmp_res->OccupancyPercentage = ds->fv("occupancyPercentage").get_asLong();
        tmp_res->EducationalLeave = ds->fv("educationalLeave").get_asString() == "X";
        int nSubEvents = ds->fv("numberOfSubEvents").get_asLong();
        if (nSubEvents > 1) {
            fillSubEvents(tmp_res,ds,false);
        }
    if (debug_)
        log("fillEventMini","Ende");
        
}

void APIBildungsPortalSoapService::fillEvent(ns1__Event * tmp_res,odb4cpp::MySQLDataset *ds)
{
    if (debug_)
        log("fillEvent","Anfang");

        //ns1__EventBase
        fillEventMini(tmp_res,ds,true);        
        
        //ns1__Event
        //std::vector<ns1__EventFee *> EventFees;
        if (ds->fv("feeAttendance").get_asString().size())
        {
            ns1__EventFee * Fee = new ns1__EventFee();
            Fee->name = new std::string(fromUtf2ISO("Teilnahmegebühr"));
            Fee->fee = ds->fv("feeAttendance").get_asString();
            tmp_res->EventFees.push_back(Fee);
        }

        if (ds->fv("feeDiscountJuleica").get_asString().size())
        {
            ns1__EventFee * Fee = new ns1__EventFee();
            Fee->name = new std::string(fromUtf2ISO("Ermäßigung für Juleica-Inhaber"));
            Fee->fee = ds->fv("feeDiscountJuleica").get_asString();
            tmp_res->EventFees.push_back(Fee);
        }

        if (ds->fv("feeWithoutAssociation").get_asString().size())
        {
            ns1__EventFee * Fee = new ns1__EventFee();
            Fee->name = new std::string("Ohne Vereinsbindung");
            Fee->fee = ds->fv("feeWithoutAssociation").get_asString();
            tmp_res->EventFees.push_back(Fee);
        }

        if (ds->fv("feeAssociationsOutsideOfLowerSaxony").get_asString().size())
        {
            ns1__EventFee * Fee = new ns1__EventFee();
            Fee->name = new std::string(fromUtf2ISO("Vereine außerhalb Niedersachen"));
            Fee->fee = ds->fv("feeAssociationsOutsideOfLowerSaxony").get_asString();
            tmp_res->EventFees.push_back(Fee);
        }

        if (ds->fv("feeVariable").get_asString().size())
        {
            ns1__EventFee * Fee = new ns1__EventFee();
            Fee->name = new std::string(ds->fv("feeVariableName").get_asString());
            Fee->fee = ds->fv("feeVariable").get_asString();
            tmp_res->EventFees.push_back(Fee);
        }

        tmp_res->Profile = new std::string(ds->fv("profile").get_asString());
        tmp_res->Acknowledgement = new std::string(ds->fv("acknowledgement").get_asString());
        tmp_res->TargetGroup = new std::string(ds->fv("targetGroup").get_asString());
        tmp_res->NumberOfLearnUnits = ds->fv("numberOfLearnUnits").get_asLong();
        
        tmp_res->Venue = new ns1__Address();
        tmp_res->Venue->Name = new std::string(ds->fv("venueName").get_asString());
        tmp_res->Venue->Zip = new std::string(ds->fv("venueZIP").get_asString());
        tmp_res->Venue->Street = new std::string(ds->fv("venueStreet").get_asString());
        tmp_res->Venue->City = new std::string(ds->fv("venueCity").get_asString());
        tmp_res->Venue->Contact = new std::string(ds->fv("venueContact").get_asString());
        tmp_res->Venue->Phone = new std::string(ds->fv("venuePhone").get_asString());
        tmp_res->Venue->Fax = new std::string(ds->fv("venueFax").get_asString());
        tmp_res->Venue->MobilePhone = new std::string();
        tmp_res->Venue->EMail = new std::string(ds->fv("venueEMail").get_asString());
        
        tmp_res->VenueIsHandicappedAccessible = ds->fv("venueIsHandicappedAccessible").get_asBool();

        tmp_res->Organizer = new ns1__Address();
        tmp_res->Organizer->Name = new std::string(ds->fv("organizerName").get_asString());
        tmp_res->Organizer->Street = new std::string(ds->fv("organizerStreet").get_asString());
        tmp_res->Organizer->Zip = new std::string(ds->fv("organizerZIP").get_asString());
        tmp_res->Organizer->City = new std::string(ds->fv("organizerCity").get_asString());
        tmp_res->Organizer->Contact = new std::string(ds->fv("organizerContact").get_asString());
        tmp_res->Organizer->Phone = new std::string(ds->fv("organizerContactPhone").get_asString());
        tmp_res->Organizer->Fax = new std::string(ds->fv("organizerFax").get_asString());
        tmp_res->Organizer->MobilePhone = new std::string(ds->fv("organizerMobilePhone").get_asString());
        tmp_res->Organizer->EMail = new std::string(ds->fv("organizerContactEMail").get_asString());

        tmp_res->InfoText = new std::string(ds->fv("infoText").get_asString());
        tmp_res->InfoText2 = new std::string(ds->fv("infoText2").get_asString());
        tmp_res->InfoText3 = new std::string(ds->fv("infoText3").get_asString());
        tmp_res->InfoText4 = new std::string(ds->fv("infoText4").get_asString());
        tmp_res->InfoURL = new std::string(ds->fv("infoURL").get_asString());
        
        if (ds->fv("maxNumberOfWorkshopRegistrations").get_asLong())
        {
            tmp_res->MaxNumberOfWorkshopRegistrations = new unsigned int(ds->fv("maxNumberOfWorkshopRegistrations").get_asLong());
        }
        
        fillWorkshops(tmp_res,ds);
    if (debug_)
        log("fillEvent","Ende");
}

void APIBildungsPortalSoapService::fillWorkshops(ns1__Event * tmp_res,odb4cpp::MySQLDataset *ds)
{
    if (debug_)
        log("fillWorkshops","Anfang");
    
    std::unique_ptr<odb4cpp::MySQLDataset> subds(dynamic_cast<odb4cpp::MySQLDataset *>(ds->getDatabase()->CreateDataset()));
    std::ostringstream qry;
    qry << "SELECT\n";
    qry << "CAST(dts AS UNSIGNED) AS id,\n";
    qry << "workshopNr AS number,\n";
    qry << "shopname AS title,\n";
    qry << "Raum AS additionalInfo,\n";
    qry << "datum AS date,\n";
    qry << "von AS timeFrom,\n";
    qry << "bis AS timeTo,\n";
    qry << "CASE\n";
    qry << "    WHEN maxtln>0\n";
    qry << "    THEN CAST(\n";
    qry << "        (\n";
    qry << "            (\n";
    qry << "                SELECT\n";
    qry << "                    CAST(IFNULL(COUNT(*),0) AS DECIMAL)\n";
    qry << "                FROM\n";
    qry << "                    workshop_tln\n";
    qry << "                WHERE\n";
    qry << "                    shopdts=workshop.dts\n";
    qry << "            )\n";
    qry << "            /\n";
    qry << "            CAST(IFNULL(maxtln,0) AS DECIMAL)\n";
    qry << "            *\n";
    qry << "            100\n";
    qry << "            ) AS UNSIGNED\n";
    qry << "        )\n";
    qry << "    ELSE 100\n";
    qry << "END AS occupancyPercentage\n";
    qry << "FROM workshop\n";
    qry << "WHERE\n";
    qry << "    lehrdts="<<single_id_<<"\n";
    qry << "    AND datum IS NOT NULL\n";
    qry << "    AND von IS NOT NULL\n";
    qry << "    AND bis IS NOT NULL\n";
    qry << "ORDER BY datum,von,bis\n";
    
    if (debug_)
        log("fillWorkshops",qry.str());
        
    subds->query(qry.str());
    std::map<std::string,ns1__WorkshopTimeslot *> slots;
    while (!subds->eof())
    {
        ns1__Workshop * w_res = new ns1__Workshop();
        fillWorkshop(w_res,subds.get());
        std::string sbTimeslot;
        sbTimeslot.append(*w_res->Date);
        sbTimeslot.append(" ");
        sbTimeslot.append(*w_res->TimeFrom);
        sbTimeslot.append("-");
        sbTimeslot.append(*w_res->TimeTo);
        if (slots.find(sbTimeslot) == slots.end()) {
            ns1__WorkshopTimeslot * s = new ns1__WorkshopTimeslot();
            s->Timeslot = new std::string(sbTimeslot);
            slots[sbTimeslot] = s;
        } 
        slots[sbTimeslot]->Workshops.push_back(w_res);
        subds->next();
    }
    for (auto i = slots.begin(); i != slots.end(); ++i)
    {
        tmp_res->WorkshopTimeslots.push_back(i->second);
    }
    if (debug_)
        log("fillWorkshops","Ende");
}

void APIBildungsPortalSoapService::fillWorkshop(ns1__Workshop * tmp_res,odb4cpp::MySQLDataset *ds)
{
    if (debug_)
        log("fillWorkshop","Anfang");
    
    tmp_res->Id = ds->fv("id").get_asLong();
    tmp_res->Number = new std::string(ds->fv("number").get_asString());
    tmp_res->Title = new std::string(ds->fv("title").get_asString());
    tmp_res->AdditionalInfo = new std::string(ds->fv("additionalInfo").get_asString());
    std::string date = ds->fv("date").get_asString();
    std::string sbDate = date.substr(0,6);
    sbDate.append("20");
    sbDate.append(date.substr(6,2));
    tmp_res->Date = new std::string(sbDate);
    tmp_res->TimeFrom = new std::string(ds->fv("timeFrom").get_asString());
    tmp_res->TimeTo = new std::string(ds->fv("timeTo").get_asString());
    tmp_res->OccupancyPercentage = ds->fv("occupancyPercentage").get_asShort();
    
    if (debug_)
        log("fillWorkshop","Ende");

}


int APIBildungsPortalSoapService::getAllEvents(_ns1__getAllEvents* in, _ns1__getAllEventsResponse& out)
{
    if (debug_)
        log("getAllEvents","Anfang");
    try {    
        ns1__EventFilter * filter = in->filter;
        std::string q = Poco::trim(fromUtf2ISO(getSqlStatementForListOfEvents(filter)));
        if (debug_)
            log("getAllEvents",q);
    
        ds_->query(q);
        while(!ds_->eof())
        {
            ns1__EventMini * tmp_res = new ns1__EventMini();
            
            //ns1__EventMini
            fillEventMini(tmp_res,ds_.get(),true);
    
            out.getAllEventsResult.push_back(tmp_res);
            ds_->next();
        }
    } catch (odb4cpp::DbErrors &e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        std::ostringstream m;
        m<<e.getMsg();
        int sz = m.str().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, m.str().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (odb4cpp::Error &e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        int sz = e.what().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.what().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (Poco::Exception & e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        int sz = e.displayText().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.displayText().c_str());
        return this->soap_senderfault(msg, NULL);        
    }

    if (debug_)
        log("getAllEvents","Ende");
    
    return SOAP_OK;
}

int APIBildungsPortalSoapService::getSingleEvent(_ns1__getSingleEvent* in, _ns1__getSingleEventResponse& out)
{
    if (debug_)
        log("getSingleEvent","Anfang");
    try { 
        single_id_ = in->id;
        std::string q = Poco::trim(fromUtf2ISO(getSqlStatementForSingleEvent()));
        if (debug_)
            log("getSingleEvent",q);
    
        ds_->query(q);
        if (!ds_->eof())
        {
            ns1__Event *tmp_res = new ns1__Event();
            fillEvent(tmp_res,ds_.get());
            out.getSingleEventResult = tmp_res;
        } else
        {
                std::ostringstream m;
                m << "Event record with ID "<<single_id_<<" not found"; 
                int sz = m.str().size()+1;
                char *msg = (char*)soap_malloc(this->soap, sz);
                snprintf(msg, sz, m.str().c_str());
                return this->soap_senderfault(msg, NULL);        
        }
    } catch (odb4cpp::DbErrors &e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        std::ostringstream m;
        m<<e.getMsg();
        int sz = m.str().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, m.str().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (odb4cpp::Error &e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        int sz = e.what().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.what().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (Poco::Exception & e)
    {
        if (ds_->getDatabase()->in_transaction())
            ds_->getDatabase()->rollback_transaction();
        int sz = e.displayText().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.displayText().c_str());
        return this->soap_senderfault(msg, NULL);        
    }

    
    
    if (debug_)
        log("getSingleEvent","Ende");
    
    return SOAP_OK;
}

int APIBildungsPortalSoapService::register_(_ns1__register* in, _ns1__registerResponse& out)
{
    if (debug_)
        log("register_","Anfang");
    
    ns1__EventRegistration * regData = in->registrationData;
    try {
        if (in->registrationData->Events.size() > 0)
        {   
            odb4cpp::Database * db = ds_->getDatabase();
            db->start_transaction();
            for (auto pos = in->registrationData->Events.begin(); pos != in->registrationData->Events.end(); ++pos)
                registerForOneEvent( *pos, regData, out.registerResult,ds_.get());
            db->commit_transaction();
        } else
        {
            char *msg = (char*)soap_malloc(this->soap, 1024);
            snprintf(msg, 1024, "Registration data without any events submitted!");
            return this->soap_senderfault(msg, NULL);        
        }
    } catch (odb4cpp::DbErrors &e)
    {
        
        ds_->getDatabase()->rollback_transaction();
        std::ostringstream m;
        m<<e.getMsg();
        int sz = m.str().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, m.str().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (odb4cpp::Error &e)
    {
        ds_->getDatabase()->rollback_transaction();
        int sz = e.what().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.what().c_str());
        return this->soap_senderfault(msg, NULL);        
        
    } catch (Poco::Exception & e)
    {
        ds_->getDatabase()->rollback_transaction();
        int sz = e.displayText().size()+1;
        char *msg = (char*)soap_malloc(this->soap, sz);
        snprintf(msg, sz, e.displayText().c_str());
        return this->soap_senderfault(msg, NULL);        
    }
    
    if (debug_)
        log("register_","Ende");
    return SOAP_OK;
}

void APIBildungsPortalSoapService::registerForOneEvent(ns1__EventRegistrationEvent * registrationEvent, ns1__EventRegistration * registrationData, std::vector<ns1__EventRegistrationWarning *>  &eventRegistrationWarnings,odb4cpp::MySQLDataset *ds)
{
    if (debug_)
        log("registerForOneEvent","Anfang");
    
    std::vector<unsigned int> workshopIds;

    if (registrationEvent->WorkshopIds.size() == 0)  {
        workshopIds.push_back(0);
    } else {
        for (auto v: registrationEvent->WorkshopIds)
            workshopIds.push_back(v);
            //workshopIds = registrationEvent->WorkshopIds;
    }
    
    for (auto &workshopId :  workshopIds)
    {
        std::ostringstream qry;
        unsigned int eventId = registrationEvent->Id;
        qry << "SELECT KSBNr,OB2,OB2LSB,dts FROM Lehrgang_Grund WHERE LSBNr='03' AND NichtPort IS NULL AND CAST(dts AS UNSIGNED)="<<eventId;
        ds->query(qry.str());
        if (!ds->eof())
        {
            odb4cpp::Database * db = ds->getDatabase();

            std::string conn_str= "host="+std::string(db->getHostName())+";user="+std::string(db->getLogin())+";password="+std::string(db->getPasswd())+";db="+std::string(db->getDatabase())+";protocol=tcp";
            MySQLRecordSet rs(conn_str,"Lehrgang_Vormerkung");

            rs.setStrAttr("Lehr_dts", ds->fv("dts").get_asString());
            rs.setStrAttr("ksbnr", ds->fv("KSBNr").get_asString());
            rs.setStrAttr("lsbnr", "03");
            rs.setStrAttr("ob2", ds->fv("ob2").get_asString());
            rs.setStrAttr("ob2lsb", ds->fv("ob2lsb").get_asString());
            rs.setStrAttr("Anrede", stringMaxLength(registrationData->Salutation, 10, registrationEvent->Id, "Salutation", eventRegistrationWarnings));
            rs.setStrAttr("Titel", stringMaxLength(registrationData->Title, 10, registrationEvent->Id, "Title", eventRegistrationWarnings));
            rs.setStrAttr("Vorname", stringMaxLength(registrationData->Firstname, 50, registrationEvent->Id, "Firstname", eventRegistrationWarnings));
            rs.setStrAttr("Name", stringMaxLength(registrationData->Lastname, 50, registrationEvent->Id, "Lastname", eventRegistrationWarnings));
            rs.setStrAttr("Strasse", stringMaxLength(registrationData->Street, 50, registrationEvent->Id, "Street", eventRegistrationWarnings));
            rs.setStrAttr("PLZ", stringMaxLength(registrationData->Zip, 5, registrationEvent->Id, "Zip", eventRegistrationWarnings));
            rs.setStrAttr("Ort", stringMaxLength(registrationData->City, 50, registrationEvent->Id, "City", eventRegistrationWarnings));
            rs.setStrAttr("Gebdat", stringMaxLength(registrationData->DateOfBirth, 10, registrationEvent->Id, "DateOfBirth", eventRegistrationWarnings));
            Poco::LocalDateTime d;
            std::string datum = Poco::DateTimeFormatter::format(d,"%d.%m.%Y %H:%M:%S");
            rs.setStrAttr("anmeldung", datum);
            rs.setStrAttr("kontoinhaber", stringMaxLength(registrationData->AccountHolder, 50, registrationEvent->Id, "AccountHolder", eventRegistrationWarnings));
            rs.setStrAttr("IBAN", stringMaxLength(registrationData->IBAN, 50, registrationEvent->Id, "IBAN", eventRegistrationWarnings));
            rs.setStrAttr("BIC", stringMaxLength(registrationData->BIC, 30, registrationEvent->Id, "BIC", eventRegistrationWarnings));
            rs.setStrAttr("email", stringMaxLength(registrationData->EMail, 100, registrationEvent->Id, "EMail", eventRegistrationWarnings));
            rs.setStrAttr("Telefon1", stringMaxLength(registrationData->Phone, 25, registrationEvent->Id, "Phone", eventRegistrationWarnings));
            rs.setStrAttr("handy", stringMaxLength(registrationData->MobilePhone, 25, registrationEvent->Id, "MobilePhone", eventRegistrationWarnings));
            rs.setStrAttr("Vereinsnummer", stringMaxLength(registrationData->AssociationNumber, 30, registrationEvent->Id, "AssociationNumber", eventRegistrationWarnings));
            rs.setStrAttr("Vereinsname", stringMaxLength(registrationData->Association, 50, registrationEvent->Id, "Association", eventRegistrationWarnings));
            rs.setStrAttr("Juleica", stringMaxLength(registrationData->YouthLeaderCard, 50, registrationEvent->Id, "YouthLeaderCard", eventRegistrationWarnings));
            rs.setStrAttr("JulDat", stringMaxLength(registrationData->YouthLeaderCardValidity, 10, registrationEvent->Id, "YouthLeaderCardValidity", eventRegistrationWarnings));
            rs.setStrAttr("Bemerkung", stringMaxLength(registrationData->Comment, 500, registrationEvent->Id, "Comment", eventRegistrationWarnings));

            if (workshopId > 0)
            {
                std::ostringstream v;
                v << workshopId;
                rs.setStrAttr("shopdts",v.str());
            }
            rs.setStrAttr("kommentarWS", stringMaxLength(registrationEvent->WorkshopComment, 500, registrationEvent->Id, "WorkshopComment", eventRegistrationWarnings));
            if (registrationData->Overnight) {
                    rs.setStrAttr("uebernacht","X");
                }
            if (registrationData->SingleRoom) {
                    rs.setStrAttr("EinzelZ","X");
                }
            if (registrationData->Vegetarian) {
                    rs.setStrAttr("Vege","X");
                }
            if (registrationData->AddressDisclosure) {
                    rs.setStrAttr("KeineFreigabe","X");
                }
            
            std::string insert_sql = rs.parseSave();
            
            if (debug_)
            {
                log("registerForOneEvent",conn_str);
                log("registerForOneEvent",insert_sql);
            }
            rs.save();
            //ds->exec(insert_sql);
        } else
        {
            //Fehler ausgeben - muss noch gemacht werden
            std::ostringstream m;
            m << "Event record with ID "<<registrationEvent->Id<<" not found"; 
            throw odb4cpp::Error(m.str());
        }
    }
    if (debug_)
        log("registerForOneEvent","Ende");
}


std::string APIBildungsPortalSoapService::stringMaxLength(std::string *inStr, int maxLength, unsigned int eventId, const std::string & fieldName, std::vector<ns1__EventRegistrationWarning *>  &eventRegistrationWarnings)
{
    if (debug_)
        log("stringMaxLength","Anfang");

    std::string res;
    if (inStr != NULL)
    {
        res = fromISO2Utf(*inStr);
        if (res.size() > maxLength) {
            
                ns1__EventRegistrationWarning * warn = new ns1__EventRegistrationWarning();
                warn->eventId = eventId;
                warn->code = new std::string("StringMaxLength");
                std::ostringstream m;
                m << "The string in field \""<<fieldName<<"\" has been cut off after "<<maxLength<<" characters.";
                warn->warningMessage = new std::string(m.str());
                ns1__KeyValue * a1 = new ns1__KeyValue();
                a1->key = new std::string("fieldName");
                a1->value = new std::string(fieldName);
                warn->additionalData.push_back(a1);
                ns1__KeyValue * a2 = new ns1__KeyValue();
                a2->key = new std::string("maxLength");
                m.str("");
                m << maxLength;
                a2->value = new std::string(m.str());
                warn->additionalData.push_back(a2);
                eventRegistrationWarnings.push_back(warn);
                res = res.substr(0, maxLength);
        }
    }
    if (debug_)
        log("stringMaxLength","Ende");
    return res;
}

int APIBildungsPortalSoapService::dumpTableFields_(_ns1__dumpTableFields *in, _ns1__dumpTableFieldsResponse &out)
{

    return dumpTableFields(in,out);
}

int APIBildungsPortalSoapService::getAllEvents_(_ns1__getAllEvents *in, _ns1__getAllEventsResponse &out)
{
    return getAllEvents(in,out);
}

int APIBildungsPortalSoapService::getSingleEvent_(_ns1__getSingleEvent *in, _ns1__getSingleEventResponse &out)
{
    return getSingleEvent(in,out);
}


std::string APIBildungsPortalSoapService::getSqlStatementForListOfEvents(ns1__EventFilter *filter)
{
    if (debug_)
        log("getSqlStatementForListOfEvents","Anfang");
    
    std::string res = getSqlStatement(false);
    std::ostringstream app;
    if (filter != NULL)
    {
        if (filter->OrganizerId != NULL) {
                app << " AND g.veranstalter="<<*filter->OrganizerId;
        }
        if (filter->VenueId != NULL) {
                app << " AND g.lehrort="<<*filter->VenueId;
        }
        if (filter->KSBNumber != NULL) {
            if (filter->KSBNumber->size())
            {
                app << " AND g.KSBNr='"<<*filter->KSBNumber<<"'";
            }
        }
        if (filter->LSBNumber != NULL) {
            if (filter->LSBNumber->size())
            {
                app << " AND g.LSBNr='"<<*filter->LSBNumber<<"'";
            }
        }
        if (filter->SportRegion != NULL) {
            if (*filter->SportRegion>0)
                app << " AND g.AB_DTS="<<*filter->SportRegion;
        }
        if (filter->OnlyVisible != NULL) {
                if (*filter->OnlyVisible)
                    app << " AND g.versteckterLG IS NULL";
        }
        if (filter->OnlyWithEducationalLeave != NULL) {
                if (*filter->OnlyWithEducationalLeave)
                    app << " AND g.bildkz IS NOT NULL";
        }
        

        if (app.str().size())
            res.append(app.str());
        addWhereExpressionToSqlStatementForTextSearch(res, "g.Tit_Bez",  filter->Title, filter->TitleSearchType);
        addWhereExpressionToSqlStatementForTextSearch(res, "g.Art_Bez",  filter->TitleAddition, filter->TitleAdditionSearchType);
        addWhereExpressionToSqlStatementForTextSearch(res, "g.The_Bez",  filter->Subtitle, filter->SubtitleSearchType);
        addWhereExpressionToSqlStatementForTextSearch(res, "g.Unt_Bez",  filter->SubtitleAddition, filter->SubtitleAdditionSearchType);
        addWhereExpressionToSqlStatementForTextSearch(res, "g.Kat_Bez",  filter->Category, filter->CategorySearchType);
        addWhereExpressionToSqlStatementForTextSearch(res, "g.Stu_Bez",  filter->SubCategory, filter->SubCategorySearchType);
    }
    if (debug_)
        log("getSqlStatementForListOfEvents","Ende");
        
    return res;
}

void APIBildungsPortalSoapService::addWhereExpressionToSqlStatementForTextSearch(std::string & stmt, const std::string &fieldName, const std::string *searchString,ns1__SearchType *searchType)
{
    if (debug_)
        log("addWhereExpressionToSqlStatementForTextSearch","Anfang");
    
            if (searchString != NULL && searchType != NULL)
            {
                if (searchString->size() ) {
                    std::ostringstream app;
                    app << " AND ";
                    app << fieldName;
                    if (*searchType == ns1__SearchType__Exact) {
                        app << "='";
                    } else {
                        app<<" LIKE ";
                        if ((*searchType == ns1__SearchType__Middle) || (*searchType == ns1__SearchType__End)) {
                            app<<"'%";
                        } else if (*searchType == ns1__SearchType__Start)
                            app <<"'";
                    }
                    app <<SqlEscape(*searchString);
                    if ((*searchType == ns1__SearchType__Start) || (*searchType == ns1__SearchType__Middle)) {
                        app<<"%'";
                    } else
                        app<<"'";
                    if (app.str().size())
                        stmt.append(app.str());
                }
            }
    if (debug_)
        log("addWhereExpressionToSqlStatementForTextSearch","Ende");
            
}


std::string APIBildungsPortalSoapService::getSqlStatementForSingleEvent()
{
    std::string res;
    res = getSqlStatement(true);
    return res;
}

std::string APIBildungsPortalSoapService::getSqlStatement(bool detail)
{
    if (debug_)
        log("getSqlStatement","Anfang");
    
    std::string res;
    std::ostringstream qry;
    qry << "        SELECT\n";
    qry << "        CAST(g.dts AS UNSIGNED) AS id,\n";
    qry << "        g.versteckterLG AS hidden,\n";
    qry << "        g.OB2LSB AS number,\n";
    qry << "        g.KSBNr AS ksbNumber,\n";
    qry << "        g.LSBNr AS lsbNumber,\n";
    qry << "        g.AB_DTS AS sportRegion,\n";
    qry << "        g.bildkz AS educationalLeave,\n";
    qry << "        g.Tit_Bez AS title,\n";
    qry << "        g.Art_Bez AS titleAddition,\n";
    qry << "        g.The_Bez AS subtitle,\n";
    qry << "        g.Unt_Bez AS subtitleAddition,\n";
    qry << "        g.Kat_Bez AS category,\n";
    qry << "        g.Stu_Bez AS subCategory,\n";
    qry << "        g.lehrtyp AS type,\n";
    qry << "        g.DAUERVON AS dateFrom,\n";
    qry << "        g.DAUERBIS AS dateTo,\n";
    qry << "        v.ORT AS venueCity,\n";
    qry << "        CASE\n";
    qry << "            WHEN g.MAXTLN>0\n";
    qry << "            THEN CAST(\n";
    qry << "                (\n";
    qry << "                    (\n";
    qry << "                       CAST(IFNULL(g.VorTN,0) AS DECIMAL)\n";
    qry << "                        +\n";
    qry << "                        CAST(IFNULL(g.LGVorm,0) AS DECIMAL)\n";
    qry << "                        +\n";
    qry << "                        CAST(IFNULL(g.IstTN,0) AS DECIMAL)\n";
    qry << "                    )\n";
    qry << "                    /\n";
    qry << "                    CAST(IFNULL(g.MAXTLN,0) AS DECIMAL)\n";
    qry << "                    *\n";
    qry << "                    100\n";
    qry << "                ) AS UNSIGNED\n";
    qry << "            )\n";
    qry << "            ELSE 100\n";
    qry << "        END AS occupancyPercentage,\n";
    qry << "        CAST(g.T2 AS UNSIGNED) AS numberOfSubEvents\n";
    if (detail)
    {
        qry << "       ,\n";
        qry << "        v.ANSCH1 AS venueName,\n";
        qry << "        v.strasse1 AS venueStreet,\n";
        qry << "        v.PLZ AS venueZIP,\n";
        qry << "        v.ORT AS venueCity,\n";
        qry << "        v.ansch AS venueContact,\n";
        qry << "        v.TELEFON AS venuePhone,\n";
        qry << "        v.FAX AS venueFax,\n";
        qry << "        v.email AS venueEMail,\n";
        qry << "        CAST(CASE WHEN v.behindert IS NOT NULL THEN 1 ELSE 0 END AS UNSIGNED) AS venueIsHandicappedAccessible,\n";
        qry << "        o.Bezeichnung AS organizerName,\n";
        qry << "        o.strasse1 AS organizerStreet,\n";
        qry << "        o.plz AS organizerZIP,\n";
        qry << "        o.ort AS organizerCity,\n";
        qry << "        o.telefon AS organizerPhone,\n";
        qry << "        o.telefax AS organizerFax,\n";
        qry << "        o.handy AS organizerMobilePhone,\n";
        qry << "        o.email AS organizerEMail,\n";
        qry << "        oc.Name AS organizerContact,\n";
        qry << "        oc.Telefon AS organizerContactPhone,\n";
        qry << "        oc.eMail AS organizerContactEMail,\n";
        qry << "        g.ZEITVON AS timeFrom,\n";
        qry << "        g.ZEITBIS AS timeTo,\n";
        qry << "        geb.Geb1 AS feeAttendance,\n";
        qry << "        geb.Geb2 AS feeDiscountJuleica,\n";
        qry << "        geb.Geb8 AS feeWithoutAssociation,\n";
        qry << "        geb.Geb9 AS feeAssociationsOutsideOfLowerSaxony,\n";
        qry << "        geb.Geb3 AS feeVariable,\n";
        qry << "        geb.Gebname3 AS feeVariableName,\n";
        qry << "        p.Profil AS profile,\n";
        qry << "        a.Bezeichnung AS acknowledgement,\n";
        qry << "        z.Bezeichnung AS targetGroup,\n";
        qry << "        CAST(IFNULL(g.LEVerl,0) AS UNSIGNED) AS numberOfLearnUnits,\n";
        qry << "        i.Infotext AS infoText,\n";
        qry << "        i2.Infotext AS infoText2,\n";
        qry << "        i3.Infotext AS infoText3,\n";
        qry << "        i4.Infotext AS infoText4,\n";
        qry << "        v.URLlink AS infoURL,\n";
        qry << "        g.AnzWs AS maxNumberOfWorkshopRegistrations\n";
                
    }
    qry << "        FROM Lehrgang_Grund AS g INNER JOIN Lehrgang_ort v ON (CAST(v.dts AS UNSIGNED)=g.lehrort)\n";
    if (detail) {
        qry << "        LEFT JOIN Lehrgang_Veranstalter o ON (CAST(o.dts AS UNSIGNED)=g.veranstalter)\n";
        qry << "        LEFT JOIN Lehrgang_InfoName oc ON (CAST(oc.dts AS UNSIGNED)=g.InfoName)\n";
        qry << "        LEFT JOIN Lehrgang_Gebuehr geb ON (geb.LGDTS=g.dts)\n";
        qry << "        LEFT JOIN lehrgang_Profil p ON (CAST(p.DTS AS UNSIGNED)=g.Profildts)\n";
        qry << "        LEFT JOIN Lehrgang_Anerkennung a ON (CAST(a.DTS AS UNSIGNED)=g.An_DTS AND a.Status='A')\n";
        qry << "        LEFT JOIN Lehrgang_Anerkennung z ON (CAST(z.DTS AS UNSIGNED)=g.Ziel_DTS AND z.Status='Z')\n";
        qry << "        LEFT JOIN lehrgang_Infotexte i ON (CAST(i.DTS AS UNSIGNED)=g.InfoArtNr)\n";
        qry << "        LEFT JOIN lehrgang_Infotexte i2 ON (CAST(i2.DTS AS UNSIGNED)=g.InfoTitNr)\n";
        qry << "        LEFT JOIN lehrgang_Infotexte i3 ON (CAST(i3.DTS AS UNSIGNED)=g.InfoTheNr)\n";
        qry << "        LEFT JOIN lehrgang_Infotexte i4 ON (CAST(i4.DTS AS UNSIGNED)=g.Infountnr)\n";
    }
    qry << "        WHERE g.LSBNr='03' AND g.NichtPort IS NULL\n";
    qry << "        AND (CAST(g.T2 AS UNSIGNED)=1 OR CAST(g.dts AS UNSIGNED)=g.Ursprung)\n";
    if (detail && single_id_) {
            qry << "        AND CAST(g.dts AS UNSIGNED)="<<single_id_<<"\n";
        }    
    res = qry.str();
    if (debug_)
        log("getSqlStatement","Ende");
    
    return res;
}
