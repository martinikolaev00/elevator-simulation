#pragma once
#include <iostream>
#include<fstream>
#include "elevator.hpp"


elevator::elevator(int tcapacity) :max_capacity(tcapacity), currfloor(1), current_capacity(0), time(0) {}

void elevator::call(std::pair<request*, request*>& req)
{

	requests.push_front(req);
	/// if(current_capacity<max_capacity) current_capacity++;
}

void elevator::go()
{
	if (requests.empty())
	{
		std::cout << "There are no requests in the elevator";
		return;
	}
	///should i check for stopontheway here as well?
	int buff_floor = requests.back().first->floor;///1st floor we gotta go to pick up passenger
	int buff_floor2 = requests.back().second->floor;/// passenger's desired floor
	if (requests.back().first->dir != "done") /// passenger is not on elevator/// check space???
	{
		addpassenger();/// reserve 1 place for the current request
		pickupeveryone(buff_floor);
		time += abs(currfloor - buff_floor) * 5;
		currfloor = buff_floor;/// went to pick up passenger and changing current floor
		requests.back().second->time = time;/// setting go request time on what time passenger went inside
	}
	pickupeveryone(buff_floor2); /// goes and picks everyone on the way
	time += abs(currfloor - buff_floor2) * 5;/// seting up time acording to floor
	//requests.back().second->time = time;///changing time to know when he got off, will help when printing output
	dropall(buff_floor2);///removes all passengers in elevator for this floor
	currfloor = buff_floor2;/// fixing current floor
	/// for print we already have time/dir-from call req/floor- go req = need just which elevator
}

int elevator::timeforreq()
{
	if (requests.empty())
		return 0;
	int time2 = abs(requests.back().second->floor - requests.back().first->floor) * 5;
	if (requests.back().first->dir != "done")
	{
		int time1 = abs(currfloor - requests.back().first->floor) * 5;
		return time1 + time2;
	}
	else return time2;
}

bool elevator::stopontheway(int floor, std::string& direction, int ttime, int next_floor)///wannabe passenger request
{
	if (requests.empty())
		return true;

	if ((currfloor - next_floor < 0 && direction == "up" && currfloor <= floor <= next_floor) ///checks if elevator direction == wannabe passenger direction
		|| (currfloor - next_floor > 0 && direction == "down" && currfloor >= floor >= next_floor))
	{
		if (time + abs(currfloor - floor) * 5 >= ttime)/// checks if the new request was made before the elevator actually passed that floor
			if (current_capacity < max_capacity)
				return true;
	}
	return false;
}

void elevator::addpassenger()
{
	current_capacity++;
}
void elevator::pickupeveryone(int next_floor)
{
	std::list<std::pair<request*, request*>>::iterator it = requests.begin();/// prep to iterate untill possible
	int buff_floor = it->first->floor;
	//requests.sort(my_compare);
	for (; *it != requests.back(); ++it)
	{
		if (stopontheway(buff_floor, it->first->dir, it->first->time, next_floor))/// will work as long people go the same direction and therå is space
		{
			//time += abs(currfloor - buff_floor) * 5;///changing time according to new floor
			//currfloor = buff_floor;/// setting new floor	
			addpassenger();
			it->first->dir = "done";///changing call request to know passeger is in elevator
			it->second->time = time;///putting go request time on what time he got on board
			buff_floor = it->first->floor;/// setting desired floor
		}

	}/// now finishing the started request
}
bool my_compare(const elevator::request& a, const elevator::request& b) { return a.floor < b.floor; }
void elevator::dropall(int floor)
{
	std::list<std::pair<request*, request*>>::iterator it = requests.begin();
	while (it != requests.end())
	{
		if (it->second->floor == floor && it->first->dir == "done")
		{
			it->second->time = time;
			current_capacity--;
			it = requests.erase(it);
		}
		else ++it;
		
	}
}