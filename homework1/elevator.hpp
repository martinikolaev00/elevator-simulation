#pragma once
#include<iostream>
#include<string>
#include<list>


 class elevator
{
public:
	struct request
	{
	
		std::string dir;
		int floor;
		int time;

	public:
		request(int tfloor,int ttime, std::string direction = "") :
			dir(direction), floor(tfloor),time(ttime) {}

	};
	
private:
	int max_capacity;
	int current_capacity;
	std::list<std::pair<request*, request*>> requests;///why pointers??
	int currfloor;
	int time;
	/// strd::string direction???
public:
	elevator(int tcapacity);
	void call(std::pair<request*,request*>& req);
	void go();	/// check if requests is not empty and fix!!!
	bool isfull() { return current_capacity == max_capacity; }
	int getcurrentfloor() { return currfloor; }/// will help when we want to decide which evelator is closer to pick up someone
	int getcurrentcap() { return current_capacity; }///???
	int gettime() { return time; }
	int timeforreq();
	bool stopontheway(int floor, std::string& direction,int ttime,int next_floor);
	void addpassenger();
	void pickupeveryone(int next_floor);///fix!
	void dropall(int floor);///check
 };