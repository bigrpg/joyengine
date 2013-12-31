#ifndef __NETWORKHELPER__H__
#define __NETWORKHELPER__H__


//常用头文件
#include 	<sys/time.h>
#include 	<iostream>
#include	<stdio.h>
#include	<errno.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<unistd.h>
#include	<arpa/inet.h>
#include 	<string>
#include 	<fcntl.h>


//辅助函数
//helper functions

//get millisecond
time_t getTime();



#endif 	//__NETWORKHELPER__H__