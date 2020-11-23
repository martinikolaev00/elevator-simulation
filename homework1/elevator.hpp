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
		request(int tfloor, int ttime, std::string direction = "") :
			dir(direction), floor(tfloor), time(ttime) {}

	};

private:
	int max_capacity;
	int current_capacity;
	std::list<std::pair<request*, request*>> requests;
	int currfloor;
	int time;
	/// strd::string direction???
public:
	elevator(int tcapacity);
	void call(std::pair<request*, request*>& req);
	void go();	/// check if requests is not empty and fix!!!
	bool isfull() { return current_capacity == max_capacity; }
	int getcurrentfloor() { return currfloor; }/// will help when we want to decide which evelator is closer to pick up someone
	int getnextfloor();
	bool isreqempty();
	int getcurrentcap() { return current_capacity; }///???
	int getlastfloor();
	int gettime() { return time; }
	int timeforreq();
	int whereafter(int seconds);///where the elevator will be after certain amount of seconds
	bool stopontheway(int floor, std::string& direction, int ttime, int next_floor);
	void addpassenger();
	void pickupeveryone(int next_floor);
	void dropall(int floor);
	bool wasreqontime(int ttime,int tfloor);
	int maxtime(); /// returns the maximum amount of the the elevator would need to finish all his requests in worst case
};