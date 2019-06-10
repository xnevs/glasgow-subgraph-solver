/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include "formats/gfu.hh"
#include "formats/input_graph.hh"

#include <fstream>
#include <string>

using std::ifstream;
using std::string;

namespace
{
    auto read_word(ifstream & infile) -> int
    {
        int x;
        infile >> x;
        return x;
    }
}

auto read_gfu(ifstream && infile, const string & filename) -> InputGraph
{
    InputGraph result{ 0, true, false };

    string name;
    infile >> name;

    result.resize(read_word(infile));
    if (! infile)
        throw GraphFileError{ filename, "error reading size", true };

    for (int r = 0 ; r < result.size(); ++r) {
        string l;
        infile >> l;
        
        if (! infile)
            throw GraphFileError{ filename, "error reading label", true };
            
        result.set_vertex_label(r, l);
    }
    
    std::size_t edge_count;
    infile >> edge_count;
    for (std::size_t i=0; i<edge_count; ++i) {
        int r = read_word(infile);
        int e = read_word(infile);
        
        if (r < 0 || r >= result.size() || e < 0 || e >= result.size())
            throw GraphFileError{ filename, "edge index out of bounds", true };
        
        result.add_edge(r, e);
    }

    string rest;
    if (infile >> rest)
        throw GraphFileError{ filename, "EOF not reached, next text is \"" + rest + "\"", true };
    if (! infile.eof())
        throw GraphFileError{ filename, "EOF not reached", true };

    return result;
}

