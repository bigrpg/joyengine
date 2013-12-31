#include "Query.h"


Query::Query( const std::string & stmt, bool shouldPartitionArgs )
{
	if (!stmt.empty())
	{
		this->init( stmt, shouldPartitionArgs );
	}
}


bool Query::init( const std::string & stmt, bool shouldPartitionArgs )
{
	if( !queryParts_.empty() )
	{
		return false;
	}

	if (!shouldPartitionArgs)
	{
		queryParts_.push_back( stmt );

		return true;
	}

	std::string::const_iterator partStart = stmt.begin();
	std::string::const_iterator partEnd = stmt.begin();

	while (partEnd != stmt.end())
	{
		if (*partEnd == '?')
		{
			queryParts_.push_back( std::string( partStart, partEnd ) );
			partStart = partEnd + 1;
		}

		++partEnd;
	}

	queryParts_.push_back( std::string( partStart, partEnd ) );

	return true;
}
