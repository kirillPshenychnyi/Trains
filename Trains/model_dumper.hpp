#ifndef __MODEL_DUMPER_HPP__
#define __MODEL_DUMPER_HPP__

#include <boost\noncopyable.hpp>

#include <ostream>

namespace RoadAnalyzer {

class Controller;
class Station;

class Dumper
	:	public boost::noncopyable
{

public:

	explicit Dumper(std::ostream & _output);

	void dump(Controller const & _controller);

	void dumpStation(Station const & _station);

private:

	std::ostream & m_output;
};

}

#endif // !__MODEL_DUMPER_HPP__
