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

#ifndef LIBCHESS_PGN_INDEX_H
#define LIBCHESS_PGN_INDEX_H

#include <string>
#include <utility>
#include <vector>
#include <boost/filesystem.hpp>

#include "game_header_bag.h"

namespace chess {

/**
 * \brief An index of a PGN database.
 */
class PgnIndex {
public:
    PgnIndex(std::string file_name);

    std::string file_name() const;
    long int file_size() const;
    std::time_t last_write_time() const;

    int __len__() const;
    GameHeaderBag get_game_headers(size_t index) const;
    long int get_pos(size_t index) const;

    bool is_valid() const;

private:
    std::vector<std::pair<long int, GameHeaderBag> > m_games;
    std::string m_file_name;
    long int m_file_size;
    std::time_t m_last_write_time;
};

} // namespace chess

#endif // LIBCHESS_PGN_INDEX_H
