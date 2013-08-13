// This file is part of the python-chess library.
// Copyright (C) 2013 Niklas Fiekas <niklas.fiekas@tu-clausthal.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have recieved a copy of the GNU General Public License
// along with this program. If not, see <http://gnu.org/licenses/>.

#include <iostream>
#include <fstream>
#include <boost/regex.hpp>

#include "pgn_index.h"

namespace chess {

PgnIndex::PgnIndex(std::string file_name) {
    m_file_name = file_name;

    boost::filesystem::path path(m_file_name);
    m_last_write_time = boost::filesystem::last_write_time(path);

    std::ifstream infile(file_name.c_str(), std::ifstream::in);
    std::string line;
    boost::regex header_regex("\\[([A-Za-z0-9]+)\\s\"(.*)\"\\]");
    boost::cmatch match_results;
    GameHeaderBag game;

    long int last_pos = 0;

    while (getline(infile, line)) {
        if (boost::regex_match(line.c_str(), match_results, header_regex)) {
            game.__setitem__(match_results[1], match_results[2]);
        } else {
            if (game.__len__()) {
                m_games.push_back(std::make_pair<long int, GameHeaderBag>(last_pos, game));
                game = GameHeaderBag();
            }

            last_pos = infile.tellg();
        }
    }

    if (game.__len__()) {
        m_games.push_back(std::make_pair<long int, GameHeaderBag>(last_pos, game));
    }

    infile.clear();
    infile.seekg(0, std::ios_base::end);
    m_file_size = infile.tellg();
    infile.close();
}

std::string PgnIndex::file_name() const {
    return m_file_name;
}

long int PgnIndex::file_size() const {
    return m_file_size;
}

std::time_t PgnIndex::last_write_time() const {
    return m_last_write_time;
}

int PgnIndex::__len__() const {
    return m_games.size();
}

GameHeaderBag PgnIndex::get_game_headers(size_t index) const {
    if (index >= m_games.size()) {
        PyErr_SetNone(PyExc_KeyError);
        throw boost::python::error_already_set();
    } else {
        return m_games[index].second;
    }
}

long int PgnIndex::get_pos(size_t index) const {
    if (index >= m_games.size()) {
        PyErr_SetNone(PyExc_KeyError);
        throw boost::python::error_already_set();
    } else {
        return m_games[index].first;
    }
}

bool PgnIndex::is_valid() const {
    boost::filesystem::path path(m_file_name);

    try {
        if (boost::filesystem::is_directory(path)) {
            return false;
        }

        long int current_file_size = boost::filesystem::file_size(path);
        if (current_file_size != m_file_size) {
            return false;
        }

        if (boost::filesystem::last_write_time(path) != m_last_write_time) {
            return false;
        }
        
        return true;
    } catch (const boost::filesystem::filesystem_error& e) {
        return false;
    }
}

} // namespace chess
