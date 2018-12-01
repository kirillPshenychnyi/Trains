#ifndef __XML_READER_HPP__
#define __XML_READER_HPP__

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include "schedule_time.hpp"

namespace RoadAnalyzer {	

class Controller;
class Station;

class XmlReader
{

public:

	explicit XmlReader(Controller & _cnt);

	void read(std::istream & _source);

	Station& addStation(int _id);

	void addTransition(
			int _trainId
		,	double _price
		,	Station& _departureStation
		,	Station& _arrivalStation
		,	std::string const& _departureTime
		,	std::string const& _arrivalTime
	);

	ScheduleTime createTime(std::string const& _arrivalTime);

private:

	boost::property_tree::ptree m_tree;

	Controller & m_controller;
};

}

#endif // !__XML_READER_HPP__
