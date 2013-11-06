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

#ifndef LIBCHESS_GAME_HEADER_BAG_H
#define LIBCHESS_GAME_HEADER_BAG_H

#include <boost/python.hpp>
#include <map>
#include <string>

namespace chess {

/**
 * \brief Case insensitive comparison of header tags.
 */
struct header_tag_less : std::binary_function<std::string, std::string, bool> {
    struct nocase_compare : public std::binary_function<unsigned char, unsigned char, bool> {
        bool operator() (const unsigned char& left, const unsigned char& right) const {
            return tolower(left) < tolower(right);
        }
    };

    bool operator() (const std::string& left, const std::string& right) const {
        return std::lexicographical_compare(
            left.begin(), left.end(), right.begin(), right.end(), nocase_compare());
    }
};

/**
 * \brief The immutable coordinates of a square on the board.
 */
class GameHeaderBag {
public:
    int __len__() const;
    std::string __getitem__(const std::string& tag) const;
    void __setitem__(const std::string& tag, const std::string& value);
    void __delitem__(const std::string& tag);
    bool __contains__(const std::string& tag) const;

private:
    std::map<std::string, std::string, header_tag_less> m_headers;
};

} // namespace chess

#endif // LIBCHESS_GAME_HEADER_BAG_H
