#include "PortalApp.h"
#include "mysqldataset.h"
#include "SQLTableRightsManager.h"
#include "SQLRightsManager.h"
#include "Poco/String.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

#include <sys/param.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <grp.h>
//#include "plugin/threads.h"  /* use gsoap/plugin/threads.h portable threads */

#include "soapH.h" /* generated with soapcpp2 */
#include "soapAPIBildungsPortalSoapService.h"

int num_proc = 0;

void sigchld_handle(int x)
{
  for (;;)
  {
    int wstat;
    pid_t pid = waitpid(-1, &wstat, WNOHANG);
    if (pid == 0)
      return;
    if (pid == -1)
      return;
    /* figure out what happened, here we print the results for debugging only */
    if (WIFEXITED(wstat))
      fprintf(stderr, "PID %d returned %d\n", pid, WEXITSTATUS(wstat));
    else if (WIFSIGNALED(wstat))
      fprintf(stderr, "PID %d signaled %d%s\n", pid, WTERMSIG(wstat), WCOREDUMP(wstat) ? "dumped core" : "");
    else if (WIFSTOPPED(wstat))
      fprintf(stderr, "PID %d stopped with signal %d\n", pid, WSTOPSIG(wstat));
    --num_proc;
  }
}

void set_privileges()
{
  gid_t newgid = getgid(), oldgid = getegid();
  uid_t newuid = getuid(), olduid = geteuid();
  if (!olduid)
    setgroups(1, &newgid);
  if (newgid != oldgid)
  {
#if !defined(linux)
    setegid(newgid);
    if (setgid(newgid) == -1)
      abort();
#else
    if (setregid(newgid, newgid) == -1)
      abort();
#endif
  }
  if (newuid != olduid)
  {
#if !defined(linux)
    seteuid(newuid);
    if (setuid(newuid) == -1)
      abort();
#else
    if (setreuid(newuid, newuid) == -1)
      abort();
#endif
  }
}

void sigalrm_handle(int x)
{
  exit(EXIT_SUCCESS);
}

void set_timer()
{
  /* configure the timer to expire after 5 seconds */
  struct itimerval timer;
  timer.it_value.tv_sec = 5;
  timer.it_value.tv_usec = 0;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;
  signal(SIGALRM, sigalrm_handle);
  setitimer(ITIMER_REAL, &timer, NULL);
}


PortalApp::PortalApp()
{
	_helpRequested = false;
	config_file_ = "portal_app.properties";
}



void PortalApp::initialize(Application& self)
{
	loadConfiguration(config_file_);
	Application::initialize(self);

	my_db_ = config().getString("my.db","");
	my_host_ = config().getString("my.host","");
	my_user_ = config().getString("my.user","");
	my_pw_ = config().getString("my.password","");
	my_port_ = config().getInt("my.port",3306);

	mongo_adr_ = config().getString("mongo_adr","");

	svc_port_ = config().getInt("soap.port",0);;
	max_proc_ = config().getInt("soap.max_proc",16);
	debug_ = config().getBool("soap.debug",false);
	accept_timeout_ = config().getInt("soap.accept_timeout",24*60*60);//nach 24 Stunden Inaktivität - abbrechen
    send_timeout_ = config().getInt("soap.send_timeout",1); /* 1 sec socket idle timeout */
    transfer_timeout_ = config().getInt("soap.transfer_timeout", 5); /* 5 sec message transfer timeout */

	time_format_ = config().getString("time.format", Poco::DateTimeFormat::SORTABLE_FORMAT);
	msg_count_ = 0;

}

void PortalApp::uninitialize()
{
	// add your own uninitialization code here
	Application::uninitialize();
}

void PortalApp::reinitialize(Application& self)
{
	Application::reinitialize(self);
	// add your own reinitialization code here
}

void PortalApp::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);

	options.addOption(
	    Option("help", "h", "display help information on command line arguments")
	    .required(false)
	    .repeatable(false)
	    .callback(OptionCallback<PortalApp>(this, &PortalApp::handleHelp)));

	options.addOption(
	    Option("define", "D", "define a configuration property")
	    .required(false)
	    .repeatable(true)
	    .argument("name=value")
	    .callback(OptionCallback<PortalApp>(this, &PortalApp::handleDefine)));

	options.addOption(
	    Option("cfg", "f", "load configuration data from a file")
	    .required(false)
	    .repeatable(true)
	    .argument("file")
	    .callback(OptionCallback<PortalApp>(this, &PortalApp::handleConfig)));
}

void PortalApp::handleHelp(const std::string& name, const std::string& value)
{
	_helpRequested = true;
	displayHelp();
	stopOptionsProcessing();
}

void PortalApp::handleDefine(const std::string& name, const std::string& value)
{
	defineProperty(value);
}

void PortalApp::handleConfig(const std::string& name, const std::string& value)
{
	config_file_ = value;
	logger().information("config_file: "+value);
	loadConfiguration(value);
}

void PortalApp::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("cls_export");
	helpFormatter.format(std::cout);
}

void PortalApp::defineProperty(const std::string& def)
{
	std::string name;
	std::string value;
	std::string::size_type pos = def.find('=');
	if (pos != std::string::npos) {
		name.assign(def, 0, pos);
		value.assign(def, pos + 1, def.length() - pos);
	} else name = def;
	config().setString(name, value);
}

int PortalApp::main(const std::vector<std::string>& args)
{
	if (!_helpRequested) {
		if (!(my_db_.size() && my_host_.size() && my_user_.size())) {
			logger().error("Database params are not defined");
			return Application::EXIT_DATAERR;
		}


		try {
			//Connections
			{
				std::unique_ptr<odb4cpp::Database> db_(new odb4cpp::MySQLDatabase(my_host_.c_str(),my_user_.c_str(),my_pw_.c_str(),my_db_.c_str()));
				if (my_port_ != 3306)
					db_->setPort(my_port_);
				if (mongo_adr_.size()) {
					dynamic_cast<odb4cpp::MySQLDatabase*>(db_.get())->setMongoAdr(mongo_adr_.c_str());
				}

				db_->connect();
				std::ostringstream m;
				std::string format(config().getString("time.format", Poco::DateTimeFormat::SORTABLE_FORMAT));
				Poco::LocalDateTime now;
				std::string dt(Poco::DateTimeFormatter::format(now, format));

				m << "Starting at "<<dt<<"\n";
				m << "DB host: "<<my_host_<<" DB: "<<my_db_<<" DB Port:" <<my_port_;
				if (mongo_adr_.size())
					m<< " Mongo Adr: "<<mongo_adr_;
				logger().information(m.str());
				logger().information("connected...");

                
                svc_port_ = config().getInt("soap.port",0);
                
                if (svc_port_ == 0)
                {
                    APIBildungsPortalSoapService bp(SOAP_XML_DEFAULTNS);
                    bp.createDataset(dynamic_cast<odb4cpp::MySQLDatabase*>(db_.get()));
                    bp.setDebug(debug_);
                    bp.serve();	// serve as CGI application 
                }
                else
                {
                    // run iterative server on port until fatal error 
                    APIBildungsPortalSoapService bp(SOAP_IO_KEEPALIVE | SOAP_XML_DEFAULTNS);
                    bp.createDataset(dynamic_cast<odb4cpp::MySQLDatabase*>(db_.get()));
                    bp.setDebug(debug_);
                    if (!config().getBool("soap.simple",false))
                    {
                        bp.soap->accept_timeout = accept_timeout_;
                        bp.soap->send_timeout = bp.soap->recv_timeout = send_timeout_;
                        bp.soap->transfer_timeout = transfer_timeout_;
                        SOAP_SOCKET m = bp.bind(NULL, svc_port_, 100);
                        if (soap_valid_socket(m))
                        {
                            signal(SIGCHLD, sigchld_handle);                           // catch child process termination 
                            while (soap_valid_socket(bp.accept()))
                            {
                                pid_t pid;
                                while (num_proc > max_proc_)  // too many active processes, wait to let them terminate 
                                    sleep(1);
                                while ((pid = fork()) == -1) // fork or retry after sleeping for one second 
                                    sleep(1);                        
                                if (pid == 0) // child process 
                                {
                                    bp.soap->master = SOAP_INVALID_SOCKET; // do not close master socket when done 
                                    set_privileges();                           // set child process privileges 
                                    set_timer();                                // set child process max running time 
                                    bp.serve();
                                    bp.destroy();
                                    exit(EXIT_SUCCESS);
                                }
                                else // parent process 
                                { 
                                    ++num_proc;
                                    bp.soap_force_close_socket();           // parent should close its socket 
                                    fprintf(stderr, "PID %d started ... ", pid); // for debugging only 
                                }                            
                            }
                        }
                        
                        bp.soap_stream_fault(std::cerr);
                        bp.destroy(); // clean up 
                    } else 
                    {
                        if (bp.run(svc_port_))
                        {
                          bp.soap_stream_fault(std::cerr);
                          exit(1);
                        }
                    }
                }
                 

				/*std::auto_ptr<odb4cpp::SQLTableRightsManager> rights_(new odb4cpp::SQLTableRightsManager(db_.get()));
				rights_->login_plain("admin","mustchange");
				{
					std::ofstream   ofs("classes.txt");
					boost::archive::text_oarchive oa(ofs);
					oa & *rights_->getSchema();
					ofs.close();

				}*/
			}


		} catch (odb4cpp::DbErrors &e) {
			logger().error(std::string("DbErrors ")+e.getMsg());
			return Application::EXIT_DATAERR;
		} catch (odb4cpp::Error &ex) {
			logger().error("odb4cpp::Error "+ex.what());
			return Application::EXIT_DATAERR;
		}
	}

	return Application::EXIT_OK;
}


void PortalApp::printProperties(const std::string& base)
{
	AbstractConfiguration::Keys keys;
	config().keys(base, keys);
	if (keys.empty()) {
		if (config().hasProperty(base)) {
			std::string msg;
			msg.append(base);
			msg.append(" = ");
			msg.append(config().getString(base));
			logger().information(msg);
		}
	} else {
		for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it) {
			std::string fullKey = base;
			if (!fullKey.empty()) fullKey += '.';
			fullKey.append(*it);
			printProperties(fullKey);
		}
	}
}


