#pragma once
#include<iostream>
#include"building.hpp"
building::building()
{
	int  firstcap = 0, secondcap = 0;
	std::string filename;
	std::cout << "Enter the filename for test: ";
	std::cin >> filename;
	std::ifstream file("filename");
	if (file.is_open())
		file >> floors >> firstcap >> secondcap >> requests;
	else
	{
		std::cout << "Problem occured when opening file\n";
		throw std::logic_error("file with such name doest exist in the directory");
	}
	Elevator1 = new elevator(firstcap);
	Elevator2 = new elevator(secondcap);
	Requests.reserve(requests);
	std::string dir;
	std::string buff;
	int call_req_floor;
	int go_req_floor;
	int call_req_time;
	int go_req_time;
	std::getline(file, dir);
	int i = 0;
	while (i < requests)
	{
		file >> buff >> dir >> call_req_floor >> call_req_time >> buff >> go_req_floor >> go_req_time;
		Requests.emplace_back(std::make_pair(elevator::request(call_req_floor, call_req_time, dir), elevator::request(go_req_floor, go_req_time)));
		++i;
	}
	file.close();
}
building::~building()
{
	delete Elevator1;
	delete Elevator2;
}

void building::run()
{
	bool same = false;///will change this variable to false everytime an elevator picks up a request if after all the checks same==true then we send elevators to move
	callelevator(Requests.begin(), 1);
	std::vector<std::pair<elevator::request, elevator::request>>::iterator it = Requests.begin();
	++it;
	for (; it!=Requests.end();)/// starts iterating through vector of requests
	{
			if (Elevator1->wasreqontime(it->first.time, it->first.floor))/// checks if its possible for Elevator1 to pick up the request instantly
			{
				if (Elevator2->wasreqontime(it->first.time, it->first.floor))/// checks if its possible for Elevator2 to pick up the request instantly
				{
					if (whichcloser(it->first.floor) == 1)///if its possible for both elevators to pick the request instantly, we choose the closer one
					{
						callelevator(it, 1);
						++it; same = false;
					}
					else
					{
						callelevator(it, 2);
						++it; same = false;
					}
				}
				else///elevator2 cant pick up the request
				{
					callelevator(it, 1);
					++it; same = false;
				}
			}
			else if (Elevator2->wasreqontime(it->first.time, it->first.floor))///elevator1 cant pick it up, now chcking Elevator2
			{
				callelevator(it, 2);
				++it; same = false;
			}
			else if (Elevator1->isreqempty() && Elevator2->isreqempty())///none of them can instantly pick up the request, now we check if they are both empty and pick the closer one
			{
				if (whichcloser(it->first.floor) == 1)
				{
					callelevator(it, 1);
					++it; same = false;
				}
				else
				{
					callelevator(it, 2);
					++it; same = false;
				}
			}
			else if (Elevator1->isreqempty() && abs(Elevator1->getcurrentfloor()-it->first.floor)*5 <= Elevator2->maxtime() + abs(Elevator2->getlastfloor() - it->first.floor )*5)
			{///this long spaghetti checks if elevator1 is empty and if he is checks wether his travel time to the floor will be less than the time
			/// Elevator2 will take to complete all of his requests + his travel time to the floor(most likely this will help us give a lazy elevator request)
				callelevator(it, 1); ++it; same = false;
			}
			else if ((Elevator2->isreqempty()) && abs(1 - it->first.floor) * 5 <= Elevator1->maxtime()+ abs(Elevator1->getlastfloor() - it->first.floor) * 5)
			{///same spaghet as before but this time we check if elevator2 is empty ....
				callelevator(it, 2); ++it; same = false;
			}
			if (same)///went through everything w.o changing anything now we send elevators to complete requests
			{
				Elevator1->go();
				Elevator2->go();
			}
			else same = true; /// something changed, prepare for next iteration
	}

	while (!Elevator1->isreqempty() || !Elevator2->isreqempty())
	{
		if (Elevator1->gettime() > Elevator2->gettime())
		{
			Elevator2->go();
			Elevator2->isreqempty() ? Elevator1->go() : Elevator2->go();
		}
		else
		{
			Elevator1->go();
			Elevator1->isreqempty() ? Elevator2->go() : Elevator1->go();
		}
	}
}

int building::whichcloser(int wanted_floor)
{
	if (abs(Elevator1->getnextfloor() - wanted_floor + Elevator1->timeforreq()) <= abs(Elevator2->getnextfloor() - wanted_floor + Elevator2->timeforreq()))
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
	Elevator1->go();
	Elevator1->go();
	Elevator2->go();


}

void building::callelevator(std::vector<std::pair<elevator::request, elevator::request>>::iterator it, int elevator)
{

	elevator::request* ptr1 = &it->first;
	elevator::request* ptr2 = &it->second;
	std::pair<elevator::request*, elevator::request*> combine = std::make_pair(ptr1, ptr2);
	if (elevator == 1)
	{
		ptr2->dir = "S";
		Elevator1->call(combine);
	}
	else
	{
		ptr2->dir = "L";
		Elevator2->call(combine);
	}
}


