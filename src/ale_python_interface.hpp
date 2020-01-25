#pragma once

#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "ale_interface.hpp"

namespace py = pybind11;

using namespace ale;
using namespace py::literals;


class ALEPyInterface : public ALEInterface {
public:
  using ALEInterface::ALEInterface;

  void getScreenRGB(py::array_t<pixel_t, py::array::c_style> &buffer);
  void getScreenGrayscale(py::array_t<pixel_t, py::array::c_style> &buffer);

  py::buffer& getRAM() const;
};

PYBIND11_MODULE(ale_py_pybind, m) {
  py::enum_<Action>(m, "Action")
        .value("NOOP", PLAYER_A_NOOP)
        .value("FIRE", PLAYER_A_FIRE)
        .value("UP", PLAYER_A_UP)
        .value("RIGHT", PLAYER_A_RIGHT)
        .value("LEFT", PLAYER_A_LEFT)
        .value("DOWN", PLAYER_A_DOWN)
        .value("UPRIGHT", PLAYER_A_UPRIGHT)
        .value("UPLEFT", PLAYER_A_UPLEFT)
        .value("DOWNRIGHT", PLAYER_A_DOWNRIGHT)
        .value("DOWNLEFT", PLAYER_A_DOWNLEFT)
        .value("UPFIRE", PLAYER_A_UPFIRE)
        .value("RIGHTFIRE", PLAYER_A_RIGHTFIRE)
        .value("LEFTFIRE", PLAYER_A_LEFTFIRE)
        .value("DOWNFIRE", PLAYER_A_DOWNFIRE)
        .value("UPRIGHTFIRE", PLAYER_A_UPRIGHTFIRE)
        .value("UPLEFTFIRE", PLAYER_A_UPLEFTFIRE)
        .value("DOWNRIGHTFIRE", PLAYER_A_DOWNRIGHTFIRE)
        .value("DOWNLEFTFIRE", PLAYER_A_DOWNLEFTFIRE)
        .export_values();

  py::class_<ALEPyInterface>(m, "ALEInterface")
    .def(py::init<>())
    .def("getString", &ALEInterface::getString)
    .def("getInt", &ALEInterface::getInt)
    .def("getBool", &ALEInterface::getBool)
    .def("getFloat", &ALEInterface::getFloat)
    .def("setString", &ALEInterface::setString)
    .def("setInt", &ALEInterface::setInt)
    .def("setBool", &ALEInterface::setBool)
    .def("setFloat", &ALEInterface::setFloat)
    .def("loadROM", &ALEInterface::loadROM)
    .def("act", &ALEPyInterface::act)
    .def("game_over", &ALEInterface::game_over)
    .def("reset_game", &ALEInterface::reset_game)
    .def("getAvailableModes", &ALEInterface::getAvailableModes)
    .def("setMode", &ALEInterface::setMode)
    .def("getAvailableDifficulties", &ALEInterface::getAvailableDifficulties)
    .def("setDifficulty", &ALEInterface::setDifficulty)
    .def("getLegalActionSet", &ALEInterface::getLegalActionSet)
    .def("getMinimalActionSet", &ALEInterface::getMinimalActionSet)
    .def("getFrameNumber", &ALEInterface::getFrameNumber)
    .def("lives", &ALEInterface::lives)
    .def("getEpisodeFrameNumber", &ALEInterface::getEpisodeFrameNumber)
    .def("getScreenGrayscale", &ALEPyInterface::getScreenGrayscale)
    .def("getScreenRGB", &ALEPyInterface::getScreenRGB)
    .def("getRAM", &ALEPyInterface::getRAM)
    .def("saveState", &ALEInterface::saveState)
    .def("loadState", &ALEInterface::loadState)
    .def("cloneState", &ALEInterface::cloneState)
    .def("restoreState", &ALEInterface::restoreState)
    .def("cloneSystemState", &ALEInterface::cloneSystemState)
    .def("restoreSystemState", &ALEInterface::restoreSystemState)
    .def("saveScreenPNG", &ALEInterface::saveScreenPNG);
}
