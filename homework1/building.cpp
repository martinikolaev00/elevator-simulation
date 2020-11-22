#pragma once
#include "building.hpp"
#include<iostream>
#include<fstream>
#include"elevator.hpp"

building::building():floors(0),requests(0)
{
	int  firstcap = 0, secondcap = 0;
	std::ifstream file("input.txt");
	if (file.is_open())
		file >> floors >> firstcap >> secondcap >> requests;
	else
	{
	std::cout << "Problem occured when opening file\n";
	return;
	}
	Elevator1 = new elevator(firstcap);
	Elevator2 = new elevator(secondcap);
	Requests.reserve(requests);
	std::string dir;
	int call_req_floor;
	int go_req_floor;
	int call_req_time;
	int go_req_time;
	std::getline(file, dir);
	int i = 0;
	while (i < requests)
	{
		file >> dir >> call_req_floor >> call_req_time >> go_req_floor>>go_req_time;
		Requests.emplace_back(std::make_pair(elevator::request(call_req_floor, call_req_time, dir), elevator::request(go_req_floor, go_req_time)));
		++i;
	}
}

building::~building()
{
	delete Elevator1;
	delete Elevator2;
}

void building::run()
{
	elevator::request* ptr1 = &Requests.begin()->first;
	elevator::request* ptr2 = &Requests.begin()->second;
	std::pair<elevator::request*, elevator::request*> combine = std::make_pair(ptr1, ptr2);
	Elevator1->call(combine);
	int i=1;
	int buff_floor=0;
	std::string buff_direction;
	std::vector<std::pair<elevator::request, elevator::request>>::iterator it = Requests.begin();
	++it;
	while (i < requests)
		buff_floor = it->first.floor;///the floor where next request is
	buff_direction = it->first.dir;
	if (Elevator1->stopontheway(buff_floor, buff_direction,it->first.time,ptr1->floor) && whichcloser(Elevator1->getcurrentfloor(),Elevator2->getcurrentfloor,buff_floor)==1)
	{
		ptr1 = &it->first;
		ptr2 = &it->second;
		combine = std::make_pair(ptr1, ptr2);
		Elevator1->call(combine);
	}
	

}

int building::whichcloser(int el1_floor, int el2_floor, int wanted_floor)
{
	if (abs(el1_floor - wanted_floor) <= abs(el2_floor - wanted_floor))
		return 1;
	else return 2;
}

void building::test()
{
	elevator::request* ptr1 = &Requests.begin()->first;
	elevator::request* ptr2 = &Requests.begin()->second;
	std::pair<elevator::request*, elevator::request*> combine = std::make_pair(ptr1, ptr2);
	ptr2->dir = "S";
	Elevator1->call(combine);
	std::vector<std::pair<elevator::request, elevator::request>>::iterator it = Requests.begin();
	++it;
	ptr1 = &it->first;
	ptr2 = &it->second;
	combine = std::make_pair(ptr1, ptr2);
	ptr2->dir = "L";
	Elevator2->call(combine);
	++it;
	ptr1 = &it->first;
	ptr2 = &it->second;
	combine = std::make_pair(ptr1, ptr2);
	ptr2->dir = "S";
	Elevator1->call(combine);
	
	
}

void building::print()
{
	for (int i = 0; i < requests; ++i)
	{
		std::cout << Requests[i].second.dir << "	" << Requests[i].second.floor << "	" << Requests[i].second.time << "\n";
	}
}
