#include "transition.hpp"

namespace RoadAnalyzer {

Transition::Transition(
		int _trainId
	,	Station const& _departureStation
	,	Station const& _arrivalStation
	,	ScheduleTime const& _departureTime
	,	ScheduleTime const& _arrivalTime
	,	double _price
)
	:	
		m_departureTime(_departureTime)
	,	m_arrivalTime(_arrivalTime)
	,	m_price(_price)
	,	m_trainId(_trainId)
	,	m_departureStation(_departureStation)
	,	m_arrivalStation(_arrivalStation)
{
}

}
