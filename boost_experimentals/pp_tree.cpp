#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>


int main(int argc, char** argv) {

    boost::filesystem::path json_path = "/home/artash/proj/IntegralLabsInternship/experimentals/boost_experimentals/exapmle.json";
    assert(boost::filesystem::exists(json_path));

    boost::filesystem::fstream file(json_path);
    std::string content(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    std::cout << content << std::endl;
    return 0;
}