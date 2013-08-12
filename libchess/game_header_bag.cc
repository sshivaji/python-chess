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

#include "game_header_bag.h"

namespace chess {

int GameHeaderBag::__len__() const {
    return m_headers.size();
}

std::string GameHeaderBag::__getitem__(const std::string& tag) const {
    std::map<std::string, std::string, header_tag_less>::const_iterator item = m_headers.find(tag);
    if (item == m_headers.end()) {
        PyErr_SetNone(PyExc_KeyError);
        throw boost::python::error_already_set();
    } else {
        return item->second;
    }
}

void GameHeaderBag::__setitem__(const std::string& tag, const std::string& value) {
    m_headers[tag] = value;
}

void GameHeaderBag::__delitem__(const std::string& tag) {
    std::map<std::string, std::string, header_tag_less>::iterator item = m_headers.find(tag);
    if (item == m_headers.end()) {
        PyErr_SetNone(PyExc_KeyError);
        throw boost::python::error_already_set();
    } else {
        m_headers.erase(item);
    }
}

bool GameHeaderBag::__contains__(const std::string& tag) const {
    std::map<std::string, std::string, header_tag_less>::const_iterator item = m_headers.find(tag);
    return item != m_headers.end();
}

} // namespace chess
