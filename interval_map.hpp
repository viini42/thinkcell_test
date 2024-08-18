#include <map>

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
	  if (!(keyBegin < keyEnd)) {
	    return;
	  }

    V valueBeyondRange = (*this)[keyEnd];
    m_map.erase(m_map.lower_bound(keyBegin), m_map.upper_bound(keyEnd));
    V valueBeforeRange = (*this)[keyBegin];
    if (!(valueBeforeRange == val)) {
      m_map.insert_or_assign(keyBegin, val);
    }
    if (!(valueBeyondRange == val)) {
      m_map.insert_or_assign(keyEnd, valueBeyondRange);
    }
	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

