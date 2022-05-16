#include <iostream>
#include <map>
#include <thread>
#include <unistd.h>
#include "stat.hpp"
using namespace std;
using namespace shannon;

int read_stat_fd = -1;
int write_stat_fd = -1;
int sub_read_stat_fd = -1;
int sub_write_stat_fd = -1;

void sub_read()
{
	MeasurePoint m(sub_read_stat_fd);
	usleep(5000);
}
void read_stat()
{
	MeasurePoint m(read_stat_fd);
	sub_read();
	usleep(8000);
}

void sub_write()
{
	MeasurePoint m(sub_write_stat_fd);
	usleep(3);
}

void write_stat()
{
	MeasurePoint m(write_stat_fd);
	sub_write();
	usleep(1);
}

void test_read()
{
	std::cout << "start read" << std::endl;
	for (int s = 0; s < 10; s++) {
		for (int i = 0; i < 1000000; i++) {
			read_stat();
		}
		sleep(1);
	}
	std::cout << "end read" << std::endl;
}

void test_write()
{
	std::cout << "start write" << std::endl;
	for (int s = 0; s < 10; s++) {
		for (int i = 0; i < 1000000; i++) {
			write_stat();
		}
		sleep(1);
	}
	std::cout << "end write" << std::endl;
}

int main(int argc, char **argv) {

//	Statistic::Init();
	Statistic::InitWithLog("/tmp/stat.log");
	read_stat_fd = Statistic::AllocStat("Read");
	write_stat_fd = Statistic::AllocStat("Write");
	sub_read_stat_fd = Statistic::AllocStat("SubRead");
	sub_write_stat_fd = Statistic::AllocStat("SubWrite");
	auto read_stat =  Statistic::GetStatByFd(read_stat_fd);
	auto sub_read_stat = Statistic::GetStatByFd(sub_read_stat_fd);
	auto write_stat = Statistic::GetStatByFd(write_stat_fd);
	auto sub_write_stat = Statistic::GetStatByFd(sub_write_stat_fd);
	sub_read_stat->SetParent(read_stat);
	sub_write_stat->SetParent(write_stat);

	std::thread read_t(test_read);
	std::thread write_t(test_write);
	read_t.detach();
	write_t.detach();

	Statistic::LoopStat();


	return 0;
}
