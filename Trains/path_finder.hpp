#ifndef __PATH_FINDER_HPP__
#define __PATH_FINDER_HPP__

#include <unordered_set>
#include <stack>
#include <boost\optional.hpp>

namespace RoadAnalyzer {

class Station;
class Transition;

class PathFinder
{
	using StationsSet = std::unordered_set<const Station *>;

	using PathItem = std::pair<const Station*, const Transition*>;

	using StationVector = std::vector<PathItem>;

public:

	PathFinder(Station const& _source, Station const& _target);

	void search();

private:

	void findPath();

	void initSiblings(Station const& _source);

	void initMoreSiblings();

	boost::optional<PathItem> findCommonStation();

	void createFinalPath(PathItem const& _coomonItem);

	void dumpPath();

	double calculatePathValue();

private:

	StationsSet m_currentDirectSet;

	StationsSet m_seenSiblings;

	StationVector m_path;

	StationVector m_sourceStationSiblings;

	Station const& m_source;
	Station const& m_target;
};

}

#endif // !__PATH_FINDER_HPP__
