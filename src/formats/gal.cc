/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include "formats/gal.hh"
#include "formats/input_graph.hh"

#include <fstream>
#include <map>

using std::ifstream;
using std::string;
using std::to_string;

namespace
{
    auto read_word(ifstream & infile) -> int
    {
        int x;
        infile >> x;
        return x;
    }
}

auto read_gal(ifstream && infile, const string & filename) -> InputGraph
{
    InputGraph result{ 0, false, true };

    result.resize(read_word(infile));
    if (! infile)
        throw GraphFileError{ filename, "error reading size", true };

    for (int r = 0 ; r < result.size() ; ++r) {
        int c_end = read_word(infile);
        if (! infile)
            throw GraphFileError{ filename, "error reading edges count", true };

        for (int c = 0 ; c < c_end ; ++c) {
            int e = read_word(infile);

            if (e < 0 || e >= result.size())
                throw GraphFileError{ filename, "edge index out of bounds", true };

            result.add_directed_edge(r, e, "");
        }
    }

    string rest;
    if (infile >> rest)
        throw GraphFileError{ filename, "EOF not reached, next text is \"" + rest + "\"", true };
    if (! infile.eof())
        throw GraphFileError{ filename, "EOF not reached", true };

    return result;
}
