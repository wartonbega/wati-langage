#include "iostream"
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
    else if (this->pre_value != nullptr)
        t += this->value + " (" + std::to_string(this->children.size()) + ") \tpreval use=" + std::to_string(this->pre_value->use) + "\n" ;
    else
        t += this->value + " \t" + this->reference + "\n";

        for (auto i : this->children)
        {
            t += i->to_string(tabs + 1);
        }
    return t;
}

std::string node::to_string()
{
    return this->to_string(0);
}

node *node::set_prevalue(w_variable *v)
{
    this->pre_value = v;
    v->explicit_definition = true;
    v->use += 1;
    return this;
}

node *node::push_child(node *n)
{
    this->children.push_back(n);
    return this;
}

node *node::push_multiple_childs(node *n)
{
    for (auto i : n->children)
    {
        this->push_child(i);
    }
    return this;
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

void node::print()
{
    std::cout << this->to_string() << std::endl;
}