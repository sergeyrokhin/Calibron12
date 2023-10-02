#pragma once
#include <iostream>
#include <string>
#include <chrono>


using namespace std;


class SimpleTimer {
public:
	SimpleTimer(const char * name_) : name_(name_) {
		start = std::chrono::high_resolution_clock::now();
	};
	~SimpleTimer() {
		Out("Finish");
	};
	void Out(const std::string step_name) {
		stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = stop - start;
		cout << name_ << ' ' << step_name << ' ' << duration.count() << "s" << endl;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
	string name_;
};
