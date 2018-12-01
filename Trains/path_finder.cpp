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
			m_sourceStationSiblings.push_back(&_transition);
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
		createFinalPath(*common);
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
		previous = &m_path.back()->getDepartureStation();
		toStart = &previous->getOutgoingTransition(0).getArrivalStation();
	}

	auto const & trains = toStart->getTrains(*previous);

	m_path.push_back(trains[0]);

	m_currentDirectSet.clear();

	initSiblings(*toStart);
}

const Transition *
PathFinder::findCommonStation()
{
	for(auto transition : m_sourceStationSiblings)
	{ 
		auto it = m_currentDirectSet.find(&transition->getArrivalStation());

		if(it != m_currentDirectSet.end())
			return transition;
	}

	return nullptr;
}

void 
PathFinder::createFinalPath(Transition const & _commonItem)
{
	const Station * lastItemBeforeTarget{nullptr};

	StationVector newEthalon{&_commonItem};

	if(m_path.empty())
		lastItemBeforeTarget = &_commonItem.getArrivalStation();
	else
	{
		lastItemBeforeTarget = &m_path.front()->getDepartureStation();

		Station const& firstItem = m_path.back()->getDepartureStation();
		auto connection = _commonItem.getArrivalStation().getTrains(firstItem);
		newEthalon.push_back(connection[0]);
	}

	const int nElements = m_path.size();

	for(int i = nElements - 1; i > 0; --i)
		newEthalon.push_back(m_path[i]);

	auto const & transitionsToTarget = lastItemBeforeTarget->getTrains(m_target);

	newEthalon.push_back(transitionsToTarget[0]);

	m_path = std::move(newEthalon);
	calculatePathValue();
}

void 
PathFinder::dumpPath()
{
	std::cout << "\tPATH FROM " << m_source.getId() << " TO " << m_target.getId() << std::endl;

	for(const auto item : m_path)
	{
		std::cout << "From Station # " << item->getDepartureStation().getId() << std::endl;
		std::cout << "To Station # " << item->getArrivalStation().getId() << std::endl;
		std::cout << "By train # " << item->getTrainId() << std::endl;
		std::cout << "Departure time: " << item->getDepartureTime() << std::endl;
		std::cout << "Arrival time: " << item->getArrivalTime() << std::endl;
		std::cout << "Price: " << item->getPrice() << std::endl;
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
		ScheduleTime const& departureTime = item->getDepartureTime();
		ScheduleTime const& arrivalTime = item->getArrivalTime();

		if(previousArrival)
			//transfer time
			inPath += ScheduleTime::calculateDifferance(*previousArrival, departureTime);

		previousArrival = &arrivalTime;
		
		inPath += 
			ScheduleTime::calculateDifferance( 
					departureTime
				,	arrivalTime
			);

		totalPrice += item->getPrice();
	}
	
	// Price formula: TIME * 0.3 + PRICE * 0.7
	return totalPrice * 0.7 + (inPath.m_hour * 60 + inPath.m_minutes) * 0.3;
}

}