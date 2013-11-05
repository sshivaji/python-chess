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

#include <boost/python.hpp>
#include <boost/format.hpp>

#include "libchess.h"
#include "uint.h"

namespace chess {

char opposite_color(char color) {
    if (color == 'w') {
        return 'b';
    } else if (color == 'b') {
        return 'w';
    } else {
        throw new std::invalid_argument("color");
    }
}

void python_translate_invalid_argument(const std::invalid_argument *e) {
    std::string str = boost::str(
        boost::format("Invalid argument: %1%.") % std::string(e->what()));
    PyErr_SetString(PyExc_ValueError, str.c_str());
}

void python_translate_logic_error(const std::logic_error *e) {
    PyErr_SetString(PyExc_RuntimeError, e->what());
}

} // namespace chess

BOOST_PYTHON_MODULE(libchess) {

using namespace boost::python;
using namespace chess;

scope().attr("__author__") = "Niklas Fiekas";
scope().attr("__copyright__") = "Copyright 2013, Niklas Fiekas";
scope().attr("__license__") = "GPL";
scope().attr("__version__") = "0.0.5";
scope().attr("__maintainer__") = "Niklas Fiekas";
scope().attr("__email__") = "niklas.fiekas@tu-clausthal.de";
scope().attr("__status__") = "Development";

register_exception_translator<std::logic_error *>(&python_translate_logic_error);
register_exception_translator<std::invalid_argument *>(&python_translate_invalid_argument);

def("opposite_color", &opposite_color);

scope().attr("START_FEN") = START_FEN;

class_<Piece>("Piece", init<char>())
    .add_property("symbol", &Piece::symbol)
    .add_property("color", &Piece::color)
    .add_property("type", &Piece::type)
    .add_property("full_color", &Piece::full_color)
    .add_property("full_type", &Piece::full_type)
    .def(self == other<Piece>())
    .def(self != other<Piece>())
    .def(self_ns::str(self))
    .def("__repr__", &Piece::__repr__)
    .def("__hash__", &Piece::__hash__)
    .def("from_color_and_type", &Piece::from_color_and_type)
    .staticmethod("from_color_and_type");

class_<Square>("Square", init<const std::string&>())
    .add_property("rank", &Square::rank)
    .add_property("file", &Square::file)
    .add_property("index", &Square::index)
    .add_property("x88_index", &Square::x88_index)
    .add_property("name", &Square::name)
    .add_property("file_name", &Square::file_name)
    .add_property("rank_name", &Square::rank_name)
    .def("is_dark", &Square::is_dark)
    .def("is_light", &Square::is_light)
    .def("is_backrank", &Square::is_backrank)
    .def("is_seventh", &Square::is_seventh)
    .def(self == other<Square>())
    .def(self != other<Square>())
    .def(self_ns::str(self))
    .def("__repr__", &Square::__repr__)
    .def("__hash__", &Square::__hash__)
    .def("from_rank_and_file", &Square::from_rank_and_file)
    .staticmethod("from_rank_and_file")
    .def("from_index", &Square::from_index)
    .staticmethod("from_index")
    .def("from_x88_index", &Square::from_x88_index)
    .staticmethod("from_x88_index");

class_<Move>("Move", init<const Square&, const Square&>())
    .def(init<const Square&, const Square&, char>())
    .add_property("source", &Move::source)
    .add_property("target", &Move::target)
    .add_property("promotion", &Move::promotion)
    .add_property("full_promotion", &Move::full_promotion)
    .add_property("uci", &Move::uci)
    .def("is_promotion", &Move::is_promotion)
    .def(self == other<Move>())
    .def(self != other<Move>())
    .def(self_ns::str(self))
    .def("__repr__", &Move::__repr__)
    .def("__hash__", &Move::__hash__)
    .def("from_uci", &Move::from_uci)
    .staticmethod("from_uci");

class_<MoveInfo>("MoveInfo", init<const Move&, const Piece&>())
    .def(init<const MoveInfo&>())
    .add_property("move", &MoveInfo::move, &MoveInfo::set_move)
    .add_property("piece", &MoveInfo::piece, &MoveInfo::set_piece)
    .add_property("captured", &MoveInfo::python_captured, &MoveInfo::python_set_captured)
    .add_property("is_enpassant", &MoveInfo::is_enpassant, &MoveInfo::set_is_enpassant)
    .add_property("is_kingside_castle", &MoveInfo::is_kingside_castle, &MoveInfo::set_is_kingside_castle)
    .add_property("is_queenside_castle", &MoveInfo::is_queenside_castle, &MoveInfo::set_is_queenside_castle)
    .add_property("is_castle", &MoveInfo::is_castle)
    .add_property("is_check", &MoveInfo::is_check, &MoveInfo::set_is_check)
    .add_property("is_checkmate", &MoveInfo::is_checkmate, &MoveInfo::set_is_checkmate)
    .add_property("san", &MoveInfo::san, &MoveInfo::set_san);

class_<Position>("Position")
    .def(init<const std::string&>())
    .def(init<const Position&>())
    .def("reset", &Position::reset)
    .def("clear_board", &Position::clear_board)
    .add_property("turn", &Position::turn, &Position::set_turn)
    .def("toggle_turn", &Position::toggle_turn)
    .add_property("ep_file", &Position::python_ep_file, &Position::python_set_ep_file)
    .add_property("half_moves", &Position::half_moves, &Position::set_half_moves)
    .add_property("ply", &Position::ply, &Position::set_ply)
    .def("get_ep_square", &Position::python_get_ep_square)
    .def("get_pseudo_legal_moves", &Position::get_pseudo_legal_moves, return_value_policy<manage_new_object>())
    .def("get_legal_moves", &Position::get_legal_moves, return_value_policy<manage_new_object>())
    .def("get_attackers", &Position::get_attackers, return_value_policy<manage_new_object>())
    .def("get_king", &Position::python_get_king)
    .def("is_king_attacked", &Position::is_king_attacked)
    .def("is_check", &Position::is_check)
    .def("is_checkmate", &Position::is_checkmate)
    .def("is_stalemate", &Position::is_stalemate)
    .def("is_insufficient_material", &Position::is_insufficient_material)
    .def("is_game_over", &Position::is_game_over)
    .def("has_kingside_castling_right", &Position::has_kingside_castling_right)
    .def("has_queenside_castling_right", &Position::has_queenside_castling_right)
    .def("set_kingside_castling_right", &Position::set_kingside_castling_right)
    .def("set_queenside_castling_right", &Position::set_queenside_castling_right)
    .def("could_have_kingside_castling_right", &Position::could_have_kingside_castling_right)
    .def("could_have_queenside_castling_right", &Position::could_have_queenside_castling_right)
    .def("make_move", &Position::make_move)
    .def("make_move_fast", &Position::make_move_fast)
    .def("get_move_from_san", &Position::get_move_from_san)
    .def("make_move_from_san", &Position::make_move_from_san)
    .add_property("fen", &Position::fen, &Position::set_fen)
    .def(self == other<Position>())
    .def(self != other<Position>())
    .def(self_ns::str(self))
    .def("__getitem__", &Position::__getitem__)
    .def("__setitem__", &Position::__setitem__)
    .def("__delitem__", &Position::__delitem__)
    .def("__repr__", &Position::__repr__)
    .def("__hash__", &Position::__hash__);

class_<PseudoLegalMoveGenerator, boost::noncopyable>("PseudoLegalMoveGenerator", init<const Position&>())
    .def("__iter__", &PseudoLegalMoveGenerator::__iter__, return_internal_reference<>())
    .def("__contains__", &PseudoLegalMoveGenerator::__contains__)
    .def("__len__", &PseudoLegalMoveGenerator::__len__)
    .def("__nonzero__", &PseudoLegalMoveGenerator::__nonzero__)
    .def("next", &PseudoLegalMoveGenerator::python_next);

class_<AttackerGenerator, boost::noncopyable>("AttackerGenerator", init<const Position&, char, const Square&>())
    .def("__iter__", &AttackerGenerator::__iter__, return_internal_reference<>())
    .def("__contains__", &AttackerGenerator::__contains__)
    .def("__len__", &AttackerGenerator::__len__)
    .def("__nonzero__", &AttackerGenerator::__nonzero__)
    .def("next", &AttackerGenerator::python_next);

class_<LegalMoveGenerator, boost::noncopyable>("LegalMoveGenerator", init<const Position&>())
    .def("__len__", &LegalMoveGenerator::__len__)
    .def("__nonzero__", &LegalMoveGenerator::__nonzero__)
    .def("__iter__", &LegalMoveGenerator::__iter__, return_internal_reference<>())
    .def("__contains__", &LegalMoveGenerator::__contains__)
    .def("next", &LegalMoveGenerator::python_next);

class_<PolyglotOpeningBookEntry>("PolyglotOpeningBookEntry", init<const Position&, const Move&, uint16_t, uint32_t>())
    .def(init<uint64_t, uint16_t, uint16_t, uint32_t>())
    .def(init<const PolyglotOpeningBookEntry&>())
    .add_property("key", &PolyglotOpeningBookEntry::key)
    .add_property("raw_move", &PolyglotOpeningBookEntry::raw_move)
    .add_property("move", &PolyglotOpeningBookEntry::move)
    .add_property("weight", &PolyglotOpeningBookEntry::weight, &PolyglotOpeningBookEntry::set_weight)
    .add_property("learn", &PolyglotOpeningBookEntry::learn, &PolyglotOpeningBookEntry::set_learn);

class_<GameHeaderBag>("GameHeaderBag", init<>())
     .def("__len__", &GameHeaderBag::__len__)
     .def("__getitem__", &GameHeaderBag::__getitem__)
     .def("__setitem__", &GameHeaderBag::__setitem__)
     .def("__delitem__", &GameHeaderBag::__delitem__)
     .def("__contains__", &GameHeaderBag::__contains__);
 
class_<PgnIndex>("PgnIndex", init<std::string>())
     .add_property("file_name", &PgnIndex::file_name)
     .add_property("file_size", &PgnIndex::file_size)
     .add_property("last_write_time", &PgnIndex::last_write_time)
     .def("__len__", &PgnIndex::__len__)
     .def("get_game_headers", &PgnIndex::get_game_headers)
     .def("get_pos", &PgnIndex::get_pos)
     .def("is_valid", &PgnIndex::is_valid);

} // BOOST_PYTHON_MODULE(libchess)
