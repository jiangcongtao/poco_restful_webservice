#include "Infrastructure/Persistence/ConnectionManager.h"

#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include "Poco/Environment.h"


namespace Infrastructure {
namespace Persistence {


    ConnectionManager::ConnectionManager()
        : db(),
          host(),
          user(),
          password()
    {
        init();
    }

    void ConnectionManager::init()
    {

        std::string environment;
        auto environmentVariableValue = std::getenv("POCO_API_ENVIRONMENT");
        if ( environmentVariableValue != nullptr )
            environment = std::string(environmentVariableValue);

        if ( environment.empty() || environment == "development" ) {
            user="developer";
            password="abc12345";
            host="192.168.1.100";
            db="poco_webservice_schema";
        }

        if ( environment == "build" ) {
            user="ci_environment";
            password="abc12345";
            host="127.0.0.1";
            db="poco_webservice_schema";
        }

        // Nick 2018-04-17: Detect and use environment variables for setup connection configurations
        host = Poco::Environment::get("mysql_host","127.0.0.1");
        user = Poco::Environment::get("mysql_user", "developer");
        password = Poco::Environment::get("mysql_pass", "abc12345");
        db = Poco::Environment::get("mysql_db", "poco_webservice_schema");

        std::cout << "--------- MySQL Configurations ----------" << std::endl;
        std::cout << "Host : " << host << std::endl;
        std::cout << "Schema : " << db << std::endl;
        std::cout << "User : " << user << std::endl;
        std::cout << "Password : " << password << std::endl;
    }

    Poco::Data::Session ConnectionManager::getSession()
    {
        std::string connectionString = "host=" + host + "; user=" + user + "; password=" + password +"; db=" + db;
        return Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, connectionString);
    }


} }
