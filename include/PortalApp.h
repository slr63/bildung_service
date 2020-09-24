#ifndef __PORTALAPP_H__
#define __PORTALAPP_H__

#include <string>
#include <memory>
#include <sstream>
#include <iostream>



#include "Poco/Exception.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;


#define __MAX_LIMIT__ 1000000000

class PortalApp: public Application {
public:
	PortalApp();

protected:
	void initialize(Application& self);

	void uninitialize();

	void reinitialize(Application& self);

	void defineOptions(OptionSet& options);

	void handleHelp(const std::string& name, const std::string& value);

	void handleDefine(const std::string& name, const std::string& value);

	void handleConfig(const std::string& name, const std::string& value);

	void displayHelp();

	void defineProperty(const std::string& def);

	int main(const std::vector<std::string>& args);

	void printProperties(const std::string& base);


private:
	bool _helpRequested;
	std::string my_host_;
	std::string my_db_;
	std::string my_user_;
	std::string my_pw_;
	int my_port_ = 3306;
	int svc_port_ = 0;
	int max_proc_ = 16;
	int accept_timeout_ = 24*60*60;//nach 24 Stunden Inaktivität - abbrechen
    int send_timeout_ = 1; /* 1 sec socket idle timeout */
    int transfer_timeout_ = 5; /* 5 sec message transfer timeout */
	std::string mongo_adr_;
    bool debug_ = false;
    
	//zero mq params
	int threads_num_;
	//time format
	std::string time_format_;

	int msg_count_;

	std::string config_file_;
};


#endif

