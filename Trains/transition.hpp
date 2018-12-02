#ifndef __TRANSITION_HPP__
#define __TRANSITION_HPP__

#include "schedule_time.hpp"

#include <memory>
#include <boost\noncopyable.hpp>

namespace RoadAnalyzer {

class Station;

class Transition
	:	public boost::noncopyable
{

public:

	using Ptr = std::unique_ptr<Transition>;

public:

	Transition(
			int _trainId
		,	Station const& _departureStation
		,	Station const& _arrivalStation
		,	ScheduleTime const& _departureTime
		,	ScheduleTime const& _arrivalTime
		,	double _price
	);
	
	ScheduleTime const& getArrivalTime() const;
	ScheduleTime const& getDepartureTime() const;

	Station const& getArrivalStation() const;
	Station const& getDepartureStation() const;

	double getPrice() const;

	int getTrainId() const;

private:

	ScheduleTime const m_departureTime;
	ScheduleTime const m_arrivalTime;

	const double m_price;

	const int m_trainId;

	Station const& m_departureStation;
	Station const& m_arrivalStation;
};

inline ScheduleTime const&
Transition::getArrivalTime() const
{
	return m_arrivalTime;
}

inline ScheduleTime const& 
Transition::getDepartureTime() const
{
	return m_departureTime;
}

inline Station const&
Transition::getArrivalStation() const
{
	return m_arrivalStation;
}

inline Station const&
Transition::getDepartureStation() const
{
	return m_departureStation;
}

inline double 
Transition::getPrice() const
{
	return m_price;
}

inline int 
Transition::getTrainId() const
{
	return m_trainId;
}


}

#endif // !__TRANSITION_HPP__
