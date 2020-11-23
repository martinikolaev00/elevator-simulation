#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include"elevator.hpp"
class building
{
	elevator* Elevator1 = nullptr;
	elevator* Elevator2 = nullptr;
	int floors;
	int requests;
	std::vector<std::pair<elevator::request, elevator::request>> Requests;
public:
	building();
	building(const building& other) = delete;
	building& operator=(const building& other) = delete;
	~building();
	void run();
	int whichcloser(int wanted_floor);
	void test();
	void callelevator(std::vector<std::pair<elevator::request, elevator::request>>::iterator it, int elevator);
};
