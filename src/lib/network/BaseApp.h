#ifndef __BASEAPP__H__
#define __BASEAPP__H__

class BaseApp
{
public:
	BaseApp();
	
	virtual bool init(int argc,const char ** argv);
	virtual void fini();
	int run();
	
	void exit(int code) { exit_ = code; }

protected:
	int exit_;
	virtual void update(float dTime);
	
};

template<typename APPT>
int runApp(int argc,const char ** argv)
{
	APPT app;
	if(!app.init(argc,argv))
		return -1;
	int ret = app.run();
	app.fini();
	return ret;
}

#endif //__BASEAPP__H__