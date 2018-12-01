#ifndef __SCHEDULE_TIME_HPP__
#define __SCHEDULE_TIME_HPP__

#include <stdio.h>
#include <ostream>
#include <string> 

namespace RoadAnalyzer {

struct ScheduleTime
{
	static const short MINUTES_IN_HOUR{60};

	ScheduleTime(short _hour, short _minutes)
		:	m_hour(_hour)
		,	m_minutes(_minutes)
	{
	}

	friend std::ostream& operator<<(std::ostream& _os, ScheduleTime const& _time);  

	friend ScheduleTime operator - (ScheduleTime const& _lhs, ScheduleTime const& _rhs);

	ScheduleTime & operator += (ScheduleTime const& _rhs);

	short m_hour;
	short m_minutes;
};

inline
std::ostream & 
operator<< (std::ostream & _os, ScheduleTime const& _time)
{
	auto beatify = 
	[](short _digit)
	{
		std::string asString{""};

		if(_digit < 10)
			asString = "0";

		return asString + std::to_string(_digit);
	};

	_os << beatify(_time.m_hour) << ':' << beatify(_time.m_minutes);

	return _os;
}

inline
ScheduleTime 
operator - (ScheduleTime const& _lhs, ScheduleTime const& _rhs)
{
	static const short HOURS_IN_DAY{24};

	short hours{0};
	short minutes{0};

	// That means the arrival is on the nextDay
	if(_rhs.m_hour <= _lhs.m_hour) 
		hours = 24 - _lhs.m_hour + _rhs.m_hour;
	else 
		hours = _rhs.m_hour - _lhs.m_hour;

	if(_rhs.m_minutes < _lhs.m_minutes)
	{
		hours--;
		minutes = _rhs.m_minutes + ScheduleTime::MINUTES_IN_HOUR - _lhs.m_minutes;
	}
	else 
		minutes = _rhs.m_minutes - _lhs.m_minutes;

	return ScheduleTime(hours, minutes);
}

}

inline
RoadAnalyzer::ScheduleTime & 
RoadAnalyzer::ScheduleTime::operator += (ScheduleTime const & _rhs)
{
	m_hour += _rhs.m_hour;
	m_minutes += _rhs.m_minutes;

	if(m_minutes >= ScheduleTime::MINUTES_IN_HOUR)
	{
		int actualMinutes = m_minutes % 60;
		m_hour += (m_minutes - actualMinutes) / 60;
		m_minutes = actualMinutes;
	}

	return *this;
}

#endif // !__SCHEDULE_TIME_HPP__
