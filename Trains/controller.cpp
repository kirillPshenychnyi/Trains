#include "controller.hpp"

namespace RoadAnalyzer {

void
Controller::addStation(Station::Ptr _station)
{
	m_stations.emplace(std::move(_station));
}

boost::optional<Station&> 
Controller::takeStation(int _id) 
{
	auto it = m_stations.find(_id, Station::Hasher(), Station::Comparator());

	if(it == m_stations.end())
		return boost::none;

	return boost::optional<Station&>(*it->get());
}

boost::optional<Station const&>
Controller::getStation(int _id) const
{
	auto it = m_stations.find(_id, Station::Hasher(), Station::Comparator());

	if(it == m_stations.end())
		return boost::none;

	return boost::optional<const Station&>(*it->get());
}

void 
Controller::forEachStation(StationCallback _callback) const
{
	for(auto const& station : m_stations)
		_callback(*station);
}

void 
Controller::addTransition(std::unique_ptr<Transition> _transition)
{
	m_transitions.push_back(std::move(_transition));
}

}