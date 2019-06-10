/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include "verify.hh"
#include <map>

using std::map;
using std::string;

BuggySolution::BuggySolution(const string & message) noexcept :
    _what(message)
{
}

auto BuggySolution::what() const noexcept -> const char *
{
    return _what.c_str();
}

auto verify_homomorphism(const std::pair<InputGraph, InputGraph> & graphs,
        bool injective,
        bool locally_injective,
        bool induced,
        const std::map<int, int> & mapping) -> void
{
    // nothing to verify, if unsat
    if (mapping.empty())
        return;

    // totality and range
    for (int i = 0 ; i < graphs.first.size() ; ++i) {
        if (! mapping.count(i))
            throw BuggySolution{ "No mapping for vertex " + graphs.first.vertex_name(i) };
        else if (mapping.find(i)->second < 0 || mapping.find(i)->second >= graphs.second.size())
            throw BuggySolution{ "Mapping " + graphs.first.vertex_name(i) + " -> " +
                graphs.second.vertex_name(mapping.find(i)->second) + " out of range" };
    }

    // no extra stuff
    for (auto & i : mapping)
        if (i.first < 0 || i.first >= graphs.first.size())
            throw BuggySolution{ "Vertex " + graphs.first.vertex_name(i.first)  + " out of range" };

    // labels
    if (graphs.first.has_vertex_labels())
        for (int i = 0 ; i < graphs.first.size() ; ++i)
            if (graphs.first.vertex_label(i) != graphs.second.vertex_label(mapping.find(i)->second))
                throw BuggySolution{ "Mismatched vertex label for assignment " + graphs.first.vertex_name(i) + " -> " +
                    graphs.second.vertex_name(mapping.find(i)->second) };

    // injectivity
    if (injective) {
        map<int, int> seen;
        for (auto & [ i, j ] : mapping) {
            if (! seen.emplace(j, i).second)
                throw BuggySolution{ "Non-injective mapping: " + graphs.first.vertex_name(i) + " -> " +
                    graphs.second.vertex_name(mapping.find(i)->second) + " and " +
                    graphs.first.vertex_name(seen.find(j)->second) + " -> " + graphs.second.vertex_name(j) };
        }
    }

    // local injectivity
    if (locally_injective) {
        for (int v = 0 ; v < graphs.first.size() ; ++v) {
            map<int, int> seen;
            for (auto & [ i, j ] : mapping) {
                if (graphs.first.adjacent(v, i) && ! seen.emplace(j, i).second)
                    throw BuggySolution{ "Non locally-injective mapping: on neighbourhood of "
                        + graphs.first.vertex_name(v) + ", " + graphs.first.vertex_name(i) + " -> " +
                        graphs.second.vertex_name(mapping.find(i)->second) + " and " +
                        graphs.first.vertex_name(seen.find(j)->second) + " -> " + graphs.second.vertex_name(j) };
            }
        }
    }

    // loops
    for (int i = 0 ; i < graphs.first.size() ; ++i) {
        if (graphs.first.adjacent(i, i) && ! graphs.second.adjacent(mapping.find(i)->second, mapping.find(i)->second))
            throw BuggySolution{ "Vertex " + graphs.first.vertex_name(i) + " has a loop but mapped vertex " +
                graphs.second.vertex_name(mapping.find(i)->second) + " does not" };
        else if (induced && graphs.second.adjacent(mapping.find(i)->second, mapping.find(i)->second) &&
                    ! graphs.first.adjacent(i, i))
            throw BuggySolution{ "Vertex " + graphs.first.vertex_name(i) + " has no loop but mapped vertex " +
                graphs.second.vertex_name(mapping.find(i)->second) + " does" };
    }

    // adjacency, non-adjacency, and edge labels
    for (auto & [ i, t ] : mapping) {
        for (auto & [ j, u ] : mapping) {
            if (graphs.first.adjacent(i, j) && ! graphs.second.adjacent(t, u))
                throw BuggySolution{ "Edge " + graphs.first.vertex_name(i) + " -- " + graphs.first.vertex_name(j) +
                    " mapped to non-edge " + graphs.second.vertex_name(t) + " -/- " + graphs.second.vertex_name(u) };
            else if (induced && ! graphs.first.adjacent(i, j) && graphs.second.adjacent(t, u))
                throw BuggySolution{ "Non-edge " + graphs.first.vertex_name(i) + " -/- " + graphs.first.vertex_name(j) +
                    " mapped to edge " + graphs.second.vertex_name(t) + " -- " + graphs.second.vertex_name(u) };
        }
    }
}

