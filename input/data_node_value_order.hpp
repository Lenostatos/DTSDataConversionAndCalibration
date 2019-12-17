#ifndef INPUT_DATA_NODE_VALUE_ORDER_H
#define INPUT_DATA_NODE_VALUE_ORDER_H


#include <vector>

namespace input { class configuration; }
namespace input { class tagging; }

namespace input {

class data_node_value_order
{
    private:
    std::vector<int> order_;


    public:
    data_node_value_order(const configuration & config, const tagging & tags);

    std::vector < int > & order()
        { return order_; }
    const std::vector < int > & order() const
        { return order_; }

};

} // namespace input

#endif
