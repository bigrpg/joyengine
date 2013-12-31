#ifndef __DB__CONFIG__H__
#define __DB__CONFIG__H__

#include <string>


class ConnectionInfo
{
public:
	ConnectionInfo(): port( 0 ) {}

	std::string	host;
	unsigned int port;
	std::string	username;
	std::string	password;
	std::string	database;

	/**
	 *	Generates a name used by all BigWorld processes to lock the database.
	 *	Only one connection can successfully obtain a lock with this name
	 *	at any one time.
	 */
	std::string generateLockName() const
	{
		std::string lockName( "JoyGame ");
		lockName += database;

		return lockName;
	}
};


#endif
