#ifndef __STATION_HPP__
#define __STATION_HPP__

#include <boost\noncopyable.hpp>

#include <memory>

#include <vector>
#include <functional>
#include <unordered_map>

#include <boost\functional\hash.hpp>

namespace RoadAnalyzer {

class Transition;

class Station
	:	public boost::noncopyable
{

public:

	using Ptr = std::unique_ptr<Station>;

	using TransitionCallback = std::function<void(Transition const&)>;

	struct Comparator
	{
		bool operator() (int _id, Ptr const& _station) const
		{
			return _id == _station->getId();
		}

		bool operator() (Ptr const& _station, int _id) const
		{
			return _station->getId() == _id;
		}

		bool operator() (
				Ptr const& _station1
			,	Ptr const& _station2
		) const
		{
			return _station1->getId() == _station2->getId();
		}
	};

	struct Hasher
	{
		std::size_t operator() (Ptr const& _station) const
		{
			return boost::hash_value(_station->getId());
		}

		std::size_t operator() (int _id) const
		{
			return boost::hash_value(_id);
		}
	};

private:

	using Transitions = std::vector<const Transition *>;

	using Trains = std::vector<const Transition*>;

	using Arrivals2Trains = std::unordered_map<const Station*, Trains>;

public:	

	explicit Station(int _id);

	void addIncomingTransition(Transition const& _transition);
	void addOutcomingTransition(Transition const& _transition);

	void forEachOutgoingTransition(TransitionCallback _callback) const;
	void forEachIncomingTransition(TransitionCallback _callback) const;

	Transition const& getOutgoingTransition(int _idx) const;

	int getOutgoingTransitionsSize() const;
	int getIncomingTransitionsSize() const;

	int getId() const;

	void forEachTrainThatLeadsTo(
			Station const& _departure
		,	TransitionCallback _callBack
		,	std::function<bool(Transition const&)> _stopFunction
			= []( Transition const& ) {return false;}
	) const;

private:

	const int m_id;

	Arrivals2Trains m_arrivals2Trains;

	Transitions m_incomingTransitions;
	Transitions m_outcomingTransitions;
};

inline int 
Station::getId() const
{
	return m_id;
}

inline int 
Station::getOutgoingTransitionsSize() const
{
	return m_outcomingTransitions.size();
}

inline int 
Station::getIncomingTransitionsSize() const
{
	return m_incomingTransitions.size();
}

}

#endif // __STATION_HPP__
