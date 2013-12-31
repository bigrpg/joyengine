#include "Concurrency.h"


#include <stdio.h>


MySem_t::MySem_t() :
	psem_(NULL)
{}
	
MySem_t::~MySem_t()
{
	fini();
}

bool MySem_t::init(const char * name)
{
   name_ = name;
#ifdef MAC_OS
   psem_ = sem_open(name,O_CREAT/*|O_EXCL*/, S_IRUSR | S_IWUSR, 0);
   if(psem_ == SEM_FAILED)
   	psem_ = NULL;
#else
   if(0 == sem_init(&sem_,0,0))
       psem_ = &sem_;
   else
       psem_ = NULL;
#endif
   return !!psem_;
}


void MySem_t::post()
{
    sem_t * psem = sem();
    if(psem)
    {
        if(sem_post(psem))
            printf("post failed\n");
    }
}

void MySem_t::wait()
{
    sem_t * psem = sem();
    if(psem)
        sem_wait(psem);
}

void MySem_t::fini()
{
    if(psem_)
    {
#ifdef MAC_OS
        sem_close(psem_);
        sem_unlink(name_.c_str());
#else
        sem_destroy(&sem_);
#endif
				psem_ = NULL;
    }
}