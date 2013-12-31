#include "BgTask.h"


WorkThread::WorkThread() :
	tid_(0)
	{}

WorkThread::~WorkThread()
{
  if(tid_)
  {
    addTask(NULL);
    pthread_join(tid_,NULL);
    pthread_mutex_destroy(&mutex_);
    sem_.fini();
  }
}
    
void WorkThread::run()
{
	while(1)
	{ 
		sem_.wait();
		pthread_mutex_lock(&mutex_);
		void * arg = taskList_.front();
		taskList_.pop_back();
		pthread_mutex_unlock(&mutex_); 
		
		if(!arg)
		    break;
		
		BgTask* task= (BgTask*)arg;
		if(task->canceled())
		{
		    task->completed_ = true;
		    continue;
		}
		task->workInBackThread();
		task->completed_= true;
	}    
}


void WorkThread::addTask(void * arg)
{
  pthread_mutex_lock(&mutex_);
  taskList_.push_back(arg);
  pthread_mutex_unlock(&mutex_);
  sem_.post(); 
}

bool WorkThread::init(const char * name)
{
  if(0 != pthread_mutex_init(&mutex_,NULL))
  {
      printf("thread init mutex error\n");
      return false;
  }    
  if(!sem_.init(name))
  {
      printf("thread init sem error\n");
      return false;
  }
  if(pthread_create(&tid_,NULL,work_func,this))
  {
      printf("thread create thread error\n");
      return false;
  }
  return true;
}


//   
WorkThreadManager & WorkThreadManager::instance()
{
	static WorkThreadManager s_inst;
	return s_inst;
}

bool WorkThreadManager::init(int count)
{
	char name[256];
	for(int i=0;i<count;++i)
	{
	    sprintf(name,"/mysema%d",i);	//for MacOX
	    WorkThread * work = new WorkThread;    
	    if(work->init(name))
	    {
	        work_threads_.push_back(work);
	    }        
	    else
	       delete work;
	}
  printf("init thread pool size is:%i\n",(int)work_threads_.size());

	return !work_threads_.empty();
}

void WorkThreadManager::fini()
{
	for(int i=0;i<work_threads_.size();++i)
	{
	    delete work_threads_[i];
	}
	work_threads_.clear();
}

void WorkThreadManager::addTask(BgTask *task,int threadID,int key)
{
	threadID = threadID%count();
	WorkThread * workthread = work_threads_[threadID];
	workthread->addTask(task);
	BgTaskVec & tasks = allTasks_[key];
	tasks.push_back(task);
}

void WorkThreadManager::cancelByKey(int key)
{
	AllTasks::iterator it = allTasks_.find(key);
	for(;it != allTasks_.end();++it)
	{
	    BgTaskVec & tasks = it->second;
	    BgTaskVec::iterator tit = tasks.begin();
	    for(;tit != tasks.end();++tit)
	    {
	        BgTask * task = *tit;
	        task->cancel();
	    }
	}
}


void WorkThreadManager::tick(time_t dTime)
{
	AllTasks::iterator it = allTasks_.begin();
	for(;it != allTasks_.end();)
	{
		BgTaskVec & tasks = it->second;
		BgTaskVec::iterator tit = tasks.begin();
		for(;tit != tasks.end();)
		{
			BgTask * task = *tit; 
			if(task->isCompleted())
			{
			     task->workInMainThread();
			     delete task;
			     tit = tasks.erase(tit);
			     printf("destroy a bgTask\n");
			}
			else
			    ++tit;   
		}
		if(tasks.empty())
		{
			AllTasks::iterator curit = it++;
			int nextFd = 0;
			if(it == allTasks_.end())
			    allTasks_.erase(curit);
			else
			{
			    nextFd = it->first;
			    allTasks_.erase(curit);
			}
			if(nextFd)
			    it = allTasks_.find(nextFd);
			else
			    it = allTasks_.end();
		}
		else
			++it; 
	}
}