
#include "xml_reader.hpp"
#include "xml_resources.hpp"
#include "schedule_time.hpp"
#include "controller.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace RoadAnalyzer {

XmlReader::XmlReader(Controller & _cnt)
	:	m_controller(_cnt)
{
}

void
XmlReader::read(std::istream & _source)
{
	boost::property_tree::read_xml( _source, m_tree );

	auto const & root = m_tree.get_child(Resources::ROOT_ELEMENT);

	for(auto const& element : root)
	{
		auto const& attributes = element.second;

		int trainId = attributes.get<int>(Resources::TRAIN_ID);
		
		std::string departureTime = attributes.get<std::string>(Resources::DEPARTURE_TIME);
		std::string arrivalTime = attributes.get<std::string>(Resources::ARRIVAL_TIME);

		double price = attributes.get<double>(Resources::PRICE);

		int departureId = attributes.get<int>(Resources::DEPARTURE_ID);
		int arrivalId = attributes.get<int>(Resources::ARRIVAL_ID);

		Station& departure = addStation(departureId);
		Station& arrival = addStation(arrivalId);

		addTransition(
				trainId
			,	price
			,	departure
			,	arrival
			,	departureTime
			,	arrivalTime
		);
	}
}

Station&
XmlReader::addStation(int _id)
{
	auto station = m_controller.takeStation(_id);

	if(station)
		return *station;

	Station * newStation = new Station(_id);

	m_controller.addStation(std::unique_ptr<Station>(newStation));

	return *newStation;
}

void 
XmlReader::addTransition( 
		int _trainId
	,	double _price
	,	Station& _departureStation
	,	Station& _arrivalStation
	,	std::string const& _departureTime
	,	std::string const& _arrivalTime
)
{
	ScheduleTime departure{createTime(_departureTime)};
	ScheduleTime arrival{createTime(_arrivalTime)};

	Transition * transition 
		=	new Transition(
					_trainId
				,	_departureStation
				,	_arrivalStation
				,	departure
				,	arrival
				,	_price
			);

	_departureStation.addOutcomingTransition(*transition);
	_arrivalStation.addIncomingTransition(*transition);

	m_controller.addTransition(std::unique_ptr<Transition>(transition));
}

ScheduleTime 
XmlReader::createTime(std::string const & _arrivalTime)
{
	static const char s_separator = ':';
	static const int s_nDigits = 2;

	std::size_t separatorPostion = _arrivalTime.find_first_of(s_separator);

	assert(separatorPostion != -1);

	const int hours = std::stoi(_arrivalTime.substr(0, s_nDigits));
	const int seconds = std::stoi(_arrivalTime.substr(separatorPostion + 1, s_nDigits));

	return ScheduleTime(hours, seconds);
}

}