#include <iostream>
#include <map>
#include <thread>
#include <unistd.h>
#include "stat.hpp"
using namespace std;
using namespace shannon;

int read_stat_fd = -1;
int write_stat_fd = -1;

void sub_read()
{

}
void read_stat()
{
	MeasurePoint m(read_stat_fd);
	usleep(30);
}

void sub_write()
{

}

void write_stat()
{
	MeasurePoint m(write_stat_fd);
	usleep(25);
}

void test_read()
{
	std::cout << "start read" << std::endl;
	for (int s = 0; s < 10; s++) {
		for (int i = 0; i < 1000; i++) {
			read_stat();
		}
	}
	std::cout << "end read" << std::endl;
}

void test_write()
{
	std::cout << "start write" << std::endl;
	for (int s = 0; s < 10; s++) {
		for (int i = 0; i < 1000; i++) {
			write_stat();
		}
	}
	std::cout << "end write" << std::endl;
}

int main(int argc, char **argv) {

	Statistic::Init();
	read_stat_fd = Statistic::AllocStat("Read");
	write_stat_fd = Statistic::AllocStat("Write");

	std::thread read_t(test_read);
	std::thread write_t(test_write);
	read_t.detach();
	write_t.detach();

	Statistic::LoopStat();


	return 0;
}
