#include "path_finder.hpp"

#include "transition.hpp"

#include <iostream>

namespace RoadAnalyzer { 

double PathFinder::TIME_WEIGHT = 0.3;
double PathFinder::MONEY_WEIGHT = 0.7;

PathFinder::PathFinder( 
		Station const & _source
	,	Station const & _target 
)
	:	m_firstTransition(nullptr)
	,	m_totalWays(0)
	,	m_visitedWays(0)
	,	m_source(_source)
	,	m_target(_target)
{
}

void 
PathFinder::dumpCheapestPath()
{
	dumpPath(m_ethalon);
}

void 
PathFinder::search()
{	
	initValidSubtree(m_target);
	searchPathFromStation(m_source);
}

void 
PathFinder::walkPath(Station::TransitionCallback _callBack) const
{
	for(auto item : m_ethalon.m_path)
		_callBack(*item);
}

double 
PathFinder::getPathPrice() const
{
	return m_ethalon.m_price;
}

void 
PathFinder::initValidSubtree(Station const & _source)
{
	_source.forEachIncomingTransition(
		[&](Transition const& _transition)
		{
			Station const& departure = _transition.getDepartureStation();

			if(&m_source != &departure && m_siblings.insert(&departure).second)
			{ 
				++m_totalWays;
				initValidSubtree(departure);
			}
		}
	);
}

void 
PathFinder::searchPathFromStation(Station const & _station)
{
	if(m_visitedWays >= m_totalWays)
		return;
	
	const int outgoingSize = _station.getOutgoingTransitionsSize();

	bool isDeparture = &_station == &m_source;

	for(int i = 0; i < outgoingSize; ++i)
	{
		Transition const& transition = _station.getOutgoingTransition(i);
		Station const& arrival = transition.getArrivalStation();

		bool isDestanationStation = &arrival == &m_target;

		if(!m_stationsVisitedInCurrentWalk.insert(arrival).second)
			break;
		
		if(!isDestanationStation && m_siblings.find(&arrival) == m_siblings.end())
			break;

		m_visitedWays++;

		if(isDeparture)
			m_firstTransition = &transition;
		else 
			m_currentPath.m_path.push_back(&transition);

		if(isDestanationStation)
		{
			m_stationsVisitedInCurrentWalk.clear();
			if(m_currentPath.m_path.empty() || m_currentPath.m_path.front() != m_firstTransition)
				m_currentPath.m_path.push_front(m_firstTransition);

			m_currentPath.m_price = calculatePathValue(m_currentPath.m_path);
			dumpPath(m_currentPath);

			checkEthalon();
			
			if(!m_currentPath.m_path.empty())
			{
				m_visitedWays--;
				m_currentPath.m_path.pop_back();
			}
		}
		else 
		{ 
			m_stationsVisitedInCurrentWalk.push_back(&arrival);
			searchPathFromStation(arrival);
		}
	}
}

void 
RoadAnalyzer::PathFinder::checkEthalon()
{
	if(m_ethalon.m_path.empty() || m_currentPath.m_price < m_ethalon.m_price)
		m_ethalon = std::move(m_currentPath);
	else 
		m_currentPath.clear();
}

void 
PathFinder::dumpPath(Path const& _path)
{
	std::cout << std::endl << "\tPATH FROM " << m_source.getId() << " TO " << m_target.getId() << std::endl;

	auto dumpTransition =
		[&](Transition const& _transition)
		{
			std::cout << "From Station # " << _transition.getDepartureStation().getId() << std::endl;
			std::cout << "To Station # " << _transition.getArrivalStation().getId() << std::endl;
			std::cout << "By train # " << _transition.getTrainId() << std::endl;
			std::cout << "Departure time: " << _transition.getDepartureTime() << std::endl;
			std::cout << "Arrival time: " << _transition.getArrivalTime() << std::endl;
			std::cout << "Price: " << _transition.getPrice() << std::endl;
			std::cout << std::endl;
		};

	for(const auto item : _path.m_path)
		dumpTransition(*item);

	std::cout << "PATH VALUE " << _path.m_price << " points " << std::endl;
}

double
PathFinder::calculatePathValue(TransitionsList const& _path)
{
	static const short MINUTES_IN_HOUR = 60;

	ScheduleTime inPath{0, 0};
	const ScheduleTime * previousArrival{nullptr};

	double totalPrice{0};

	for(const auto item : _path)
	{
		ScheduleTime const& departureTime = item->getDepartureTime();
		ScheduleTime const& arrivalTime = item->getArrivalTime();

		if(previousArrival)
			//transfer time
			inPath += ScheduleTime::calculateDifference(*previousArrival, departureTime);

		previousArrival = &arrivalTime;
		
		inPath += 
			ScheduleTime::calculateDifference( 
					departureTime
				,	arrivalTime
			);

		totalPrice += item->getPrice();
	}
	
	return calculatePathValue(inPath, totalPrice);
}

double 
PathFinder::calculatePathValue(ScheduleTime const & _time, double _price) const
{
	// Price formula: TIME * 0.3 + PRICE * 0.7
	return 
		_price * MONEY_WEIGHT + (_time.m_hour * 60 + _time.m_minutes) * TIME_WEIGHT;
}

}
