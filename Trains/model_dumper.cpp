#include "model_dumper.hpp"

#include "controller.hpp"
#include "model_dumper.hpp"

#include "schedule_time.hpp"

namespace RoadAnalyzer {

Dumper::Dumper(std::ostream & _output)
	: m_output(_output)
{
}

void
Dumper::dump(Controller const & _controller)
{
	_controller.forEachStation(
		std::bind(&Dumper::dumpStation, this, std::placeholders::_1)
	);
}

void
Dumper::dumpStation(Station const & _station)
{
	auto transitonDump
		=	[&](
					int _trainId
				,	ScheduleTime const& _time
				,	Station const& _station
				,	std::string const & _destanation
				,	double _price
			)
			{
				m_output 
					<< _destanation
					<< _station.getId()
					<< " at "
					<< _time 
					<< " train # " << _trainId
					<< " price " << _price
					<< std::endl;
			};

	m_output << "\tStation #: " << _station.getId() << std::endl << std::endl;

	m_output << "\tARRIVALS" << std::endl;

	_station.forEachIncomingTransition(
		[&](Transition const& _transition)
		{
			transitonDump(
					_transition.getTrainId()
				,	_transition.getArrivalTime()
				,	_transition.getDepartureStation()
				,	" from "
				,	_transition.getPrice()
			);
		}
	);

	m_output << std::endl;

	m_output << "\tDEPARTURES" << std::endl;

	_station.forEachOutgoingTransition(
		[&](Transition const& _transition)
		{
			transitonDump(
					_transition.getTrainId()
				,	_transition.getDepartureTime()
				,	_transition.getArrivalStation()
				,	" to "
				,	_transition.getPrice()
			);
		}
	);

	m_output << std::endl << "###############" << std::endl << std::endl;
}

}