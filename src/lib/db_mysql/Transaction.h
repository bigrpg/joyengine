#ifndef __MYSQLTRANSACTION__H__
#define __MYSQLTRANSACTION__H__

class MySql;

class MySqlTransaction
{
public:
	MySqlTransaction( MySql & sql );
	~MySqlTransaction();

	bool shouldRetry() const;

	void commit();

private:
	MySqlTransaction( const MySqlTransaction& );
	void operator=( const MySqlTransaction& );

	MySql & sql_;
	bool committed_;
};


#endif 