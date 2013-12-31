#ifndef __BGTASK__H__
#define __BGTASK__H__

#include "Concurrency.h"

#include <pthread.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <map>

class WorkThread;

class BgTask 
{
	friend class WorkThread;
	
protected:
  bool completed_;
  bool canceled_;

public:
  BgTask() : completed_(false),canceled_(false)
  {}

  virtual ~BgTask()
  {
  }
  bool isCompleted() const  { return completed_;}
  bool canceled() const { return canceled_;}
  void cancel() 
  {
      canceled_= true;
  }
  virtual void workInMainThread() {}
  virtual void workInBackThread() = 0;    //don't use this object after call of "work*" func
};

class WorkThread
{
  pthread_t tid_;
  pthread_mutex_t mutex_;
  MySem_t sem_;
  typedef std::deque<void *>   TaskVec;
  TaskVec     taskList_;
 
  static void * work_func(void * arg)
  {
      WorkThread * thread = (WorkThread*)arg;
      thread->run();
      return NULL;
  }

  void run();

public:
  WorkThread();
  ~WorkThread();

  void addTask(void * arg);
  bool init(const char * name);

};


class WorkThreadManager
{
	typedef std::vector<WorkThread*> WorkThreadVec;
	WorkThreadVec work_threads_;
	typedef std::list<BgTask*>  BgTaskVec;
	typedef std::map<int,BgTaskVec> AllTasks;
	AllTasks	allTasks_;


	WorkThreadManager() {}
public:
	bool init(int count);
	void fini();
	
	void addTask(BgTask *task,int threadID,int key);
	void cancelByKey(int fd);
	void tick(time_t dTime);
	size_t count() const { return work_threads_.size(); }
	
	static WorkThreadManager& instance();

};
#endif //__BGTASK__H__