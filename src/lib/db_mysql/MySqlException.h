#ifndef __MYSQL__EXCEPTION__H__
#define __MYSQL__EXCEPTION__H__

#include <string>
#include <mysql/mysql.h>

/**
 *	This class is an exception that can be thrown when a database query fails.
 */
class DatabaseException : public std::exception
{
public:
	DatabaseException( MYSQL * pConnection );
	~DatabaseException() throw();

	virtual const char * what() const throw() { return errStr_.c_str(); }

	bool shouldRetry() const;
	bool isLostConnection() const;

private:
	std::string errStr_;
	unsigned int errNum_;
};


#endif
