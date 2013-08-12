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

#include "pgn_index.h"

namespace chess {

PgnIndex::PgnIndex(std::string file_name) {
    m_file_name = file_name;

    boost::filesystem::path path(m_file_name);
    m_last_write_time = boost::filesystem::last_write_time(path);

    // TODO: Actually scan the file.
    // TODO: Consider creating a hash of the contents.
    m_file_size = boost::filesystem::file_size(path);
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
