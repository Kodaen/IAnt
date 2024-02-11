#pragma once

#include "Location.h"

class Route
{
	// ------------ ATTRIBUTES ------------ //
private:
	Location _start, _end;
	int _distance;

	// ------------ CONSTRUCTOR ------------ //
public:
	Route(Location start, Location end, int distance) {
		_start = start;
		_end = end;
		_distance = distance;
	}

	// ------------ GETTERS/SETTERS ------------ //
public:
	Location getStart() {
		return _start;
	}

	Location getEnd() {
		return _end;
	}

	int getDistance() {
		return _distance;
	}

	// ------------ OPERATORS ------------ //
public:
	bool operator==(void* other) const {
		bool result = false;
		if (static_cast<Route*>(other)) {
			Route* route = static_cast<Route*>(other);
			result = (_start == route->_start) && (_end == route->_end);
		}
		return result;
	}
	bool operator<(const Route& other) const { return (_distance < other._distance); }

};

