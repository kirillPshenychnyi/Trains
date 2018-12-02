
#include "xml_reader.hpp"

#include "controller.hpp"
#include "model_dumper.hpp"
#include "path_finder.hpp"

#include <fstream>
#include <string>
#include <boost\property_tree\xml_parser.hpp>

class StreamWrapper
{

public:

	explicit StreamWrapper(std::string const& _path)
	{
		m_fstream.open(_path, std::ifstream::in);
	}

	bool isOpen()
	{
		return m_fstream.is_open();
	}

	~StreamWrapper()
	{
		m_fstream.close();
	}

	std::ifstream& takeStream()
	{
		return m_fstream;
	}

private:

	std::ifstream m_fstream;
};

int main(int argc, char* argv[])
{
	static const int argsSize = 4;
	static const char * const usage = "Usage <data_path> <departure> <arrival>";

	if(argc != argsSize)
	{ 
		std::cout << usage;
		return -1;
	}

	StreamWrapper wrapper(argv[1]);

	if(!wrapper.isOpen())
	{ 
		std::cout << "Failed to open file";
		return -1;
	}

	RoadAnalyzer::Controller cnt;

	try 
	{
		int departureStationId = std::stoi(argv[2]);
		int arrivalStationId = std::stoi(argv[3]);

		RoadAnalyzer::XmlReader reader(cnt);
		reader.read(wrapper.takeStream());

		RoadAnalyzer::Dumper dumper(std::cout);
		dumper.dump(cnt);

		auto departureStation = cnt.getStation(departureStationId);
		auto arrivalStation = cnt.getStation(arrivalStationId);

		if(!departureStation || !arrivalStationId)
		{
			std::cout << "Unknown station(s)";
			return -1;
		}

		RoadAnalyzer::PathFinder finder(*departureStation, *arrivalStation);

		finder.search();

		std::cout << "\tCHEAPEST" << std::endl;

		finder.dumpCheapestPath();
	}
	catch(boost::property_tree::xml_parser_error ex)
	{
		std::cout << ex.what();
	}
	catch(std::invalid_argument ex)
	{
		std::cout << "Wrong station ids" << std::endl;
	}

	return 0;
}
