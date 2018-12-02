#include "Station.hpp"
#include "transition.hpp"

namespace RoadAnalyzer {

Station::Station(int _id)
	:	m_id(_id)
{
}

void
Station::addIncomingTransition(Transition const & _transition)
{
	m_incomingTransitions.push_back(&_transition);
}

void
Station::addOutcomingTransition(Transition const & _transition)
{
	Station const& targetStation = _transition.getArrivalStation();

	auto it = m_arrivals2Trains.find(&targetStation);

	if(it == m_arrivals2Trains.end())
		m_arrivals2Trains.emplace(&targetStation, Trains({&_transition}));
	else 
		it->second.push_back(&_transition);

	m_outcomingTransitions.push_back(&_transition);
}

void 
Station::forEachOutgoingTransition(TransitionCallback _callback) const
{
	for(auto transition : m_outcomingTransitions)
		_callback(*transition);
}

void 
Station::forEachIncomingTransition(TransitionCallback _callback) const
{
	for(auto transition : m_incomingTransitions)
		_callback(*transition);
}

Transition const & Station::getOutgoingTransition(int _idx) const
{
	assert(_idx >= 0 && _idx < m_outcomingTransitions.size());

	return *m_outcomingTransitions.at(_idx);
}

void
Station::forEachTrainThatLeadsTo(
		Station const& _arrival
	,	TransitionCallback _callBack
	,	std::function<bool(Transition const&)> _stopFunction
) const
{
	auto trains = m_arrivals2Trains.find(&_arrival);

	if(trains != m_arrivals2Trains.end())
	{
		for(auto transition : trains->second)
		{
			_callBack(*transition);

			if(_stopFunction(*transition))
				return;
		}
	}
}

}
