#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__

#include <memory>
#include <vector>
#include <functional>

#include <boost\unordered_set.hpp>
#include <boost\noncopyable.hpp>
#include <boost\optional.hpp>

#include "transition.hpp"
#include "station.hpp"

namespace RoadAnalyzer {

class Controller
	:	public boost::noncopyable
{
	using Transitions = std::vector<Transition::Ptr>;
	using Stations = boost::unordered_set<Station::Ptr, Station::Hasher, Station::Comparator>;

	using StationCallback = std::function<void(Station const&)>;

public:

	boost::optional<Station&> takeStation(int _id);

	boost::optional<Station const&> getStation(int _id) const;

	void forEachStation(StationCallback _callback) const;

	void addTransition(std::unique_ptr<Transition> _transition);

	void addStation(Station::Ptr _station);

private:

	Transitions m_transitions;
	Stations m_stations;
};

}

#endif // !__CONTROLLER_HPP__
