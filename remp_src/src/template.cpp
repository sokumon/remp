#include <iostream>
#include <sstream>
#include "template.hpp"


std::stringstream readme_contents;

void one_pound(std::string content){
    std::stringstream title;
    title << "# ";
    title << content;
    title << "\n\n";
    readme_contents << title.str();
}

void second_pound(std::string content){
    std::stringstream sub_title;
    sub_title << "## ";
    sub_title << content;
    sub_title << "\n\n";
    readme_contents << sub_title.str();
}

std::string fetch_readme(){
    return readme_contents.str();
}

void default_template(std::string repo){
    one_pound(repo);
    second_pound("Why build this");
    second_pound("Tech Stack");
}