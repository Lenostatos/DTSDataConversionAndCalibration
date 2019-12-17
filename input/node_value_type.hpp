#ifndef INPUT_NODE_VALUE_TYPE_H
#define INPUT_NODE_VALUE_TYPE_H


namespace input {

/// Describes how the content of an xml node should be interpreted.
enum node_value_type
{
  STRING,
  INT,
  REAL,
  SPECIAL
};

} // namespace input

#endif
