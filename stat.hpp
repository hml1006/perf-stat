/*
 * stat.hpp
 *
 *  Created on: 2022-05-10
 *      Author: mengliang.huang
 */

#ifndef STAT_HPP_
#define STAT_HPP_

#include <atomic>
#include <time.h>
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace shannon {
class MeasurePoint;
class SecondStat;
class StatItem;
class Statistic;

class SecondStat {
private:
	time_t time_; // 哪一秒的数据
};

class Statistic {
public:
	static void init()
	{
		Statistic::current_stat_fd = -1;
		for (int i = 0; i < 1024; i++) {
			stats_.push_back(nullptr);
		}
	}

	// stat_name 标记一个统计指标
	int AllocStat(std::string stat_name)
	{
		current_stat_fd++;
		if (current_stat_fd > 1024) {
			current_stat_fd--;
			return -1;
		}
		if (stat_name.empty()) {
			return -1;
		}
		stat_map.insert(std::make_pair<int, std::string>(current_stat, stat_name));
		return current_stat_fd;
	}

	// 查询统计指标名称
	std::string GetStatName(int stat)
	{
		if (current_stat_fd >= stats_.size()) {
			return "";
		}
		StatItem *item = stats_[stat];
		if (item != nullptr) {
			return item->GetStatName();
		}

		return "";
	}
private:
	static int current_stat_fd {-1};
	static std::vector<StatItem*> stats_;
};

class StatItem {
public:
	StatItem()
	{
		for (int i = 0; i < 10; i++) {
			ten_second_stats_.push_back(nullptr);
		}
	}
	std::string GetStatName()
	{
		return stat_name_;
	}

private:
	int GetNextPtr()
	{
		if (ptr_ == ten_second_stats_.size() - 1) {
			return 0;
		}
		return ptr_ + 1;
	}

	int MoveToNextPtr()
	{
		if (ptr_ == ten_second_stats_.size() - 1) {
			ptr_ = 0;
		} else {
			ptr_++;
		}
		return ptr_;
	}

private:
	std::vector<std::shared_ptr<SecondStat>> ten_second_stats_;
	std::string stat_name_;
	int ptr_{0};
};

// 用计算时间的对象, 析构时间减去构造函数时间为时间间隔
class MeasurePoint {
public:
	MeasurePoint(int stat_fd)
	{
		stat_fd_ = stat_fd;
		gettimeofday(&start_, NULL);
	}
	~MeasurePoint()
	{
		struct timeval end;
		gettimeofday(&end, NULL);

		time_t sec = end.tv_sec - start_.tc_sec;
		suseconds_t usec = end.tv_usec - start_.tv_usec;

		int32_t usec = (int32_t)sec * 1000000 + usec;

	}
private:
	struct timeval start_;
	int stat_fd_;
};
}


#endif /* STAT_HPP_ */
