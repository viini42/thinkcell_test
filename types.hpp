// Some very constrained example types to use with the interval_map.

#include <ostream>

class Key {
public:
  friend std::ostream& operator<<(std::ostream& os, const Key& key);

  Key(int value) : m_value(value) {}
  Key(const Key& cp) : m_value(cp.m_value) {}
  
  Key& operator=(const Key& rhs) {
    m_value = rhs.m_value;
    return *this;
  }
  
  bool operator<(const Key& rhs) const {
    return m_value < rhs.m_value;
  }

private:
  int m_value;
};

class Value {
public:
  friend std::ostream& operator<<(std::ostream& os, const Value& value);

  Value(char value) : m_value(value) {}
  Value(const Value& cp) : m_value(cp.m_value) {}
  
  Value& operator=(const Value& rhs) {
    m_value = rhs.m_value;
    return *this;
  }
  
  bool operator==(const Value& rhs) const {
    return m_value == rhs.m_value;
  }

private:
  char m_value;
};

