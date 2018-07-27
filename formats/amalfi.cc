/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include "formats/amalfi.hh"
#include "formats/input_graph.hh"

#include <cstddef>

#include <fstream>
#include <string>

using std::ifstream;
using std::string;
namespace
{
    uint16_t read_word(std::istream & in) {
        uint16_t x = static_cast<unsigned char>(in.get());
        x |= static_cast<uint16_t>(in.get()) << 8;
        return x;
    }

}

auto read_amalfi(ifstream && infile, const string & filename) -> InputGraph
{
    InputGraph result{ 0, false, true };  // HACK: say the graph is edge labelled,
                                          // so that we can use add_directed_edge,
                                          // that adds both directions -- the oposite with label "unlabeled"

    result.resize(read_word(infile));
    if (! infile)
        throw GraphFileError{ filename, "error reading size" };

    for (int r = 0 ; r < result.size() ; ++r) {
        int c_end = read_word(infile);
        if (! infile)
            throw GraphFileError{ filename, "error reading edges count" };

        for (int c = 0 ; c < c_end ; ++c) {
            int e = read_word(infile);

            if (e < 0 || e >= result.size())
                throw GraphFileError{ filename, "edge index out of bounds" };

            result.add_directed_edge(r, e, "");
        }
    }

    string rest;
    if (infile >> rest)
        throw GraphFileError{ filename, "EOF not reached, next text is \"" + rest + "\"" };
    if (! infile.eof())
        throw GraphFileError{ filename, "EOF not reached" };

    return result;
}

