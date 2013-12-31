#ifndef __SEMINPDENT__H__
#define __SEMINPDENT__H__



#include <semaphore.h>
#include <fcntl.h>
#include <string>


//for indepdend platform
class MySem_t
{
	sem_t sem_;
	sem_t * psem_;
	std::string name_;
	
	sem_t * sem() 
	{
	    return psem_;
	}
public:
	MySem_t();
	~MySem_t();
	
	bool init(const char * name);
	void post();
	void wait();
	void fini();

};



#endif //__SEMINPDENT__H__
