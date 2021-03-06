#include "./include/node.hpp"

std::vector<std::string> node::get_childs_value()
{
    std::vector<std::string> childs;
    for (int i = 0; i < this->children.size(); i++)
    {
        childs.push_back(this->children[i]->value);
    }
    return childs;
}

std::string node::to_string(int tabs)
{
    std::string t;
    for (int i = 0; i < tabs; i++)
    {
        t += "|  ";
    }
    if (this->children.size() != 0)
        t += this->value + " (" + std::to_string(this->children.size()) + ") \t" + this->reference + "\n";
    else
        t += this->value + " \t" + this->reference + "\n";

        for (auto i : this->children)
        {
            t += i->to_string(tabs + 1);
        }
    return t;
}

void node::push_child(node *n)
{
    this->children.push_back(n);
}

node::node(std::string val)
{
    this->value = val;
    this->pre_value = nullptr;
}

node::~node()
{
    for (auto n : this->children)
    {
        delete n;
    }
}