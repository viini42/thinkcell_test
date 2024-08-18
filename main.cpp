#include "interval_map.hpp"
#include "types.hpp"
#include <iostream>
#include <cassert>

std::ostream& operator<<(std::ostream& os, const Key& key) {
  os << key.m_value;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
  os << value.m_value;
  return os;
}

template<typename K, typename V>
void printMap(const std::map<K, V>& m) {
  for (const auto& i : m) {
    std::cout << i.first << ": " << i.second << std::endl;
  }
}

void IntervalMapTest() {
  auto checkInvariants = [](const auto& m) {
    assert(m.m_map.empty() || !(m.m_map.begin()->second == m.m_valBegin));
    
    // This condition was not explicitly stated in the requirements, but implicitly follows from
    // the others
    assert(m.m_map.empty() || m.m_map.rbegin()->second == m.m_valBegin);
    
    // Check canonicalness
    for (auto i = m.m_map.begin(); i != m.m_map.end(); ++i) {
      assert(std::next(i) == m.m_map.end() || !(i->second == std::next(i)->second));
    }
  };

  std::cout << "Test empty map returns initial value for all keys" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    checkInvariants(m);
    assert(m[-100] == 'F');
    assert(m[0] == 'F');
    assert(m[100] == 'F');
  };
  
  std::cout << "Insert range into empty map" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(3, 9, 'C');
    checkInvariants(m);
    assert(m[2] == 'F');
    assert(m[3] == 'C');
    assert(m[8] == 'C');
    assert(m[9] == 'F');
  }
  
  std::cout << "Insert range inside of larger range" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(-1, 15, 'A');
    checkInvariants(m);
    m.assign(6, 10, 'B');
    checkInvariants(m);
    assert(m[5] == 'A');
    assert(m[6] == 'B');
    assert(m[9] == 'B');
    assert(m[10] == 'A');
  }

  std::cout << "Insert range around smaller range" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(6, 10, 'B');
    checkInvariants(m);
    m.assign(-1, 15, 'A');
    checkInvariants(m);
    assert(m[-2] == 'F');
    assert(m[-1] == 'A');
    assert(m[8] == 'A');  // Previously 'B'
    assert(m[14] == 'A');
    assert(m[15] == 'F');
  }
  
  std::cout << "Insert range straddling start of another range" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(6, 15, 'A');
    checkInvariants(m);
    m.assign(2, 10, 'B');
    checkInvariants(m);
    assert(m[1] == 'F');
    assert(m[2] == 'B');
    assert(m[9] == 'B');
    assert(m[10] == 'A');
    assert(m[14] == 'A');
    assert(m[15] == 'F');
  }
  
  std::cout << "Insert range straddling end of another range" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(6, 15, 'A');
    checkInvariants(m);
    m.assign(11, 18, 'B');
    checkInvariants(m);
    assert(m[10] == 'A');
    assert(m[11] == 'B');
    assert(m[17] == 'B');
    assert(m[18] == 'F');
  }
  
  std::cout << "Insert range inside larger range with same value" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(-10, 20, 'A');
    checkInvariants(m);
    m.assign(-5, 7, 'A');
    checkInvariants(m);
  }
  
  std::cout << "Insert range on top of other range" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(-10, 20, 'A');
    checkInvariants(m);
    m.assign(-5, 10, 'B');
    checkInvariants(m);
    m.assign(-5, 10, 'C');
    checkInvariants(m);
    assert(m[-5] == 'C');
    assert(m[9] == 'C');
    assert(m[10] == 'A');
  }
  
  std::cout << "Set inner range to same as outer range" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(-10, 20, 'A');
    checkInvariants(m);
    m.assign(-5, 10, 'B');
    checkInvariants(m);
    m.assign(-5, 10, 'A');
    checkInvariants(m);
    assert(m[-5] == 'A');
    assert(m[9] == 'A');
    assert(m[10] == 'A');
  }
  
  std::cout << "Insert range containing the initial value at start" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(5, 10, 'A');
    checkInvariants(m);
    m.assign(2, 7, 'F');
    checkInvariants(m);
    assert(m[6] == 'F');
    assert(m[7] == 'A');
    assert(m[9] == 'A');
    assert(m[10] == 'F');
  }
  
  std::cout << "Insert outer range containing the initial value" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(5, 10, 'A');
    checkInvariants(m);
    m.assign(2, 15, 'F');
    checkInvariants(m);
  }
  
  std::cout << "Insert redundant range" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(2, 15, 'A');
    checkInvariants(m);
    m.assign(3, 13, 'A');
    checkInvariants(m);
  }
  
  std::cout << "Insert range with value same as initial value into empty map" << std::endl;
  {
    interval_map<Key, Value> m{'F'};
    m.assign(2, 15, 'F');
  }
  
  std::cout << "Randomized test" << std::endl;
  {
    const unsigned trials = 100;
    
    for (unsigned trial = 0; trial < trials; ++trial) {
      std::cout << "Trial " << trial << std::endl;
      const int keyMin = 0;
      const int keyMax = 1000;
      const int keyRange = keyMax - keyMin;
      const unsigned numRanges = 1000;
      const int defaultValue = 2;
    
      interval_map<int, int> m{defaultValue};
      std::array<int, keyRange> naiveMap;
      naiveMap.fill(defaultValue);
      
      auto checkEquivalent = [&]() {
        for (unsigned i = 0; i < keyRange; ++i) {
          if (naiveMap[i] != m[i]) {
            std::cout << "FAIL" << std::endl;
            std::cout << "naiveMap[" << i << "] != m[" << i << "]; "
              << naiveMap[i] << " != " << m[i] << std::endl;
            std::cout << "Interval map:" << std::endl;
            printMap(m.m_map);
            for (unsigned j = 0; j < keyRange; ++j) {
              std::cout << j << ": " << naiveMap[j] << std::endl;
            }
            exit(1);
          }
        }
      };

      for (int range = 0; range < numRanges; ++range) {
        int a = 50 + std::rand() % 900; // Leave some space at the ends
        int b = 50 + std::rand() % 900;
        int from = std::min(a, b);
        int to = std::max(a, b);
        int value = std::rand() % 5; // Small number to test canonicalness
        
        m.assign(from, to, value);
        
        for (unsigned i = from; i < to; ++i) {
          naiveMap[i] = value;
        }

        checkInvariants(m);
        checkEquivalent();
      }
    }
  }
  
  std::cout << "Success!\n";
}

int main() {
  IntervalMapTest();
  return 0;
}

