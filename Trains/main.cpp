
#include "xml_reader.hpp"

#include "controller.hpp"
#include "model_dumper.hpp"
#include "path_finder.hpp"
#include <fstream>
#include <string>

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

int main()
{
	StreamWrapper wrapper("e:\\Education\\programming\\Trains\\Debug\\data.xml");

	if(!wrapper.isOpen())
		return 0;

	RoadAnalyzer::Controller cnt;

	try 
	{
		RoadAnalyzer::XmlReader reader(cnt);
		reader.read(wrapper.takeStream());

		RoadAnalyzer::Dumper dumper(std::cout);
		dumper.dump(cnt);

		//1981 to 1937
		RoadAnalyzer::PathFinder finder(
				*cnt.getStation(1981)
			,	*cnt.getStation(1940)
		);

		finder.search();
	}
	catch(std::exception ex)
	{
		std::cout << ex.what();
	}

	return 0;
}