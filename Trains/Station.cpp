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

	auto it = m_departures2Trains.find(&targetStation);

	if(it == m_departures2Trains.end())
		m_departures2Trains.emplace(&targetStation, Trains({&_transition}));
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

Station::Trains const & 
Station::getTrains(Station const& _departure) const
{
	auto trains = m_departures2Trains.find(&_departure);

	assert(trains != m_departures2Trains.end());

	return trains->second;
}

}
