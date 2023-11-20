#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "node.hpp"
#ifndef MAIN_H
#define MAIN_H

extern std::string base_dir;

extern std::string main_filename; 
extern bool show_tree;
extern bool short_error;
extern std::string list_creation_class_name;
int main(int argc, char *argv[]);
std::string separate_base_dir(std::string expr);
std::string open_file(const char *name);
std::string throw_error(w_variable *);
void write_file(std::string name, std::string content);

#endif