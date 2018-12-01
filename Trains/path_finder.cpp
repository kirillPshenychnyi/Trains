#include "path_finder.hpp"

#include "transition.hpp"
#include "station.hpp"

#include <iostream>

namespace RoadAnalyzer { 

PathFinder::PathFinder( 
		Station const & _source
	,	Station const & _target 
)
	:	m_source(_source)
	,	m_target(_target)
{
	m_source.forEachOutgoingTransition(
		[&](Transition const& _transition)
		{
			m_sourceStationSiblings.emplace_back(
					&_transition.getArrivalStation()
				,	&_transition
			);
		}
	);
}

void 
PathFinder::search()
{
	initSiblings(m_target);
	findPath();
}

void 
PathFinder::findPath()
{
	auto common = findCommonStation();

	if(!common)
	{
		initMoreSiblings();
		findPath();
	}
	else
	{
		createFinalPath( *common );
		dumpPath();
	}
}

void
PathFinder::initSiblings(Station const & _source)
{
	_source.forEachIncomingTransition(
		[&](Transition const& _transition)
		{
			m_currentDirectSet.insert(&_transition.getDepartureStation());
		}
	);
}

void 
PathFinder::initMoreSiblings()
{
	const Station * toStart{nullptr};
	const Station * previous{nullptr};

	if(m_path.empty())
	{ 
		toStart = *m_currentDirectSet.begin();
		previous = &m_target;
	}
	else
	{
		previous = m_path.back().first;
		toStart = &previous->getOutgoingTransition(0).getArrivalStation();
	}

	auto const & trains = toStart->getTrains(*previous);

	m_path.emplace_back(toStart, trains[0]);

	m_currentDirectSet.clear();

	initSiblings(*toStart);
}

boost::optional<PathFinder::PathItem> 
PathFinder::findCommonStation()
{
	for(auto station : m_sourceStationSiblings)
	{ 
		auto it = m_currentDirectSet.find(station.first);

		if(it != m_currentDirectSet.end())
			return boost::optional<PathItem>( 
					PathItem{*it, station.second}
				);
	}

	return boost::none;
}

void 
PathFinder::createFinalPath(PathItem const & _commonItem)
{
	const Station * lastItemBeforeTarget{nullptr};

	StationVector newEthalon{_commonItem};

	if(m_path.empty())
		lastItemBeforeTarget = _commonItem.first;
	else
	{
		lastItemBeforeTarget = m_path.front().first;

		auto firstItem = m_path.back();
		auto connection = _commonItem.first->getTrains(*firstItem.first);
		newEthalon.emplace_back(_commonItem.first, connection[0]);
	}

	const int nElements = m_path.size();

	for(int i = nElements - 1; i > 0; --i)
		newEthalon.push_back(m_path[i]);

	auto const & transitionsToTarget = lastItemBeforeTarget->getTrains(m_target);

	newEthalon.emplace_back(lastItemBeforeTarget, transitionsToTarget[0]);

	m_path = std::move(newEthalon);
	calculatePathValue();
}

void 
PathFinder::dumpPath()
{
	std::cout << "\tPATH FROM " << m_source.getId() << " TO " << m_target.getId() << std::endl;

	for(const auto item : m_path)
	{
		std::cout << "From Station # " << item.second->getDepartureStation().getId() << std::endl;
		std::cout << "To Station # " << item.second->getArrivalStation().getId() << std::endl;
		std::cout << "By train # " << item.second->getTrainId() << std::endl;
		std::cout << "Departure time: " << item.second->getDepartureTime() << std::endl;
		std::cout << "Arrival time: " << item.second->getArrivalTime() << std::endl;
		std::cout << "Price: " << item.second->getPrice() << std::endl;
		std::cout << std::endl;
	}

	std::cout << "PATH VALUE " << calculatePathValue() << " poinst " << std::endl;
}

double
PathFinder::calculatePathValue()
{
	static const short MINUTES_IN_HOUR = 60;

	ScheduleTime inPath{0, 0};
	const ScheduleTime * previousArrival{nullptr};

	double totalPrice{0};

	for(const auto item : m_path)
	{
		ScheduleTime const& departureTime = item.second->getDepartureTime();
		ScheduleTime const& arrivalTime = item.second->getArrivalTime();

		if(previousArrival)
		{
			//transfer time
			inPath += departureTime - *previousArrival;
		}

		previousArrival = &arrivalTime;
		inPath += departureTime - arrivalTime;
		totalPrice += item.second->getPrice();
	}
	
	// Price formula: TIME * 0.3 + PRICE * 0.7
	return totalPrice * 0.7 + (inPath.m_hour * 60 + inPath.m_minutes) * 0.3;
}

}