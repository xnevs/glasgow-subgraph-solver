/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef GLASGOW_SUBGRAPH_SOLVER_SOLVER_FORMATS_AMALFI_HH
#define GLASGOW_SUBGRAPH_SOLVER_SOLVER_FORMATS_AMALFI_HH 1

#include "formats/input_graph.hh"
#include "formats/graph_file_error.hh"

#include <iosfwd>
#include <string>

/**
 * Read a LAD format file into an InputGraph.
 *
 * \throw GraphFileError
 */
auto read_amalfi(std::ifstream && infile, const std::string & filename) -> InputGraph;

#endif
