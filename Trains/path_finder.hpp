#ifndef __PATH_FINDER_HPP__
#define __PATH_FINDER_HPP__

#include "station.hpp"
#include <unordered_set>
#include <list>

#include <boost\noncopyable.hpp>

namespace RoadAnalyzer {

class Station;
class Transition;
struct ScheduleTime;

class PathFinder
	:	boost::noncopyable
{
	static double TIME_WEIGHT;
	static double MONEY_WEIGHT;

	using StationsSet = std::unordered_set<const Station *>;
	using StationList = std::vector<const Station *>;
	using TransitionsList = std::list<const Transition*>;

	struct Path
	{
		TransitionsList m_path;
		double m_price{0.0};

		Path& operator = (Path const& _other)
		{
			if(&_other == this)
				return *this;

			m_path = _other.m_path;
			m_price = _other.m_price;

			return *this;
		}

		void clear()
		{
			m_path.clear();
			m_price = 0.0;
		}
	};

public:

	PathFinder(Station const& _source, Station const& _target);

	void dumpCheapestPath();

	void search();

	void walkPath(Station::TransitionCallback _callBack) const;

	double getPathPrice() const;

private:

	void initValidSubtree(Station const& _source);

	void searchPathFromStation(Station const& _station);

	double calculatePathValue(TransitionsList const& _path);

	double calculatePathValue(
			ScheduleTime const& _time
		,	double _price
	) const;

	void checkEthalon();

	void dumpPath(Path const& _path);

private:

	StationsSet m_siblings;

	StationList m_stationsVisitedInCurrentWalk;

	Path m_currentPath;
	Path m_ethalon;

	const Transition * m_firstTransition;
	int m_totalWays;
	int m_visitedWays;

	Station const& m_source;
	Station const& m_target;
};

}

#endif // !__PATH_FINDER_HPP__
