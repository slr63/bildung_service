        private:
            std::string getSqlStatementForSingleEvent();
            std::string getSqlStatement(bool detail);
            void addWhereExpressionToSqlStatementForTextSearch(std::string & stmt, const std::string &fieldName,  const std::string *searchString,ns1__SearchType *searchType);
            std::string getSqlStatementForListOfEvents(ns1__EventFilter *filter);
            void fillEventBase(ns1__EventBase * tmp_res,odb4cpp::MySQLDataset *ds,bool simpleDate=true);
            void fillEventMini(ns1__EventMini * tmp_res,odb4cpp::MySQLDataset *ds,bool simpleDate=true);
            void fillEvent(ns1__Event * tmp_res,odb4cpp::MySQLDataset *ds);
            void fillWorkshops(ns1__Event * tmp_res,odb4cpp::MySQLDataset *ds);
            void fillWorkshop(ns1__Workshop * tmp_res,odb4cpp::MySQLDataset *ds);
            void fillSubEvents(ns1__EventMini * tmp_res,odb4cpp::MySQLDataset *ds,bool simpleDate=true);
            void registerForOneEvent(/*SqlTransaction transaction,*/ ns1__EventRegistrationEvent * registrationEvent, ns1__EventRegistration * registrationData, std::vector<ns1__EventRegistrationWarning *>  &eventRegistrationWarnings,odb4cpp::MySQLDataset *ds);
            std::string stringMaxLength(std::string *inStr, int maxLength, unsigned int eventId, const std::string & fieldName, std::vector<ns1__EventRegistrationWarning *>  &eventRegistrationWarnings);
            std::unique_ptr<odb4cpp::MySQLDataset> ds_;
            unsigned int single_id_ = 0;
            bool debug_ = false;
            
            void log(const std::string &method,const std::string &msg);
        public:
            void createDataset(odb4cpp::MySQLDatabase *db);
            void setDebug(bool d);