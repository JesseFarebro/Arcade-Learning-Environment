#include "ale_python_interface.hpp"

void ALEPyInterface::getScreenRGB(py::array_t<pixel_t, py::array::c_style> &buffer) {
  py::buffer_info info = buffer.request();
  if (info.ndim != 3)
    throw std::runtime_error("Expected a numpy array with three dimensions.");

  size_t w = environment->getScreen().width();
  size_t h = environment->getScreen().height();

  if (info.shape[0] != h || info.shape[1] != w || info.shape[2] != 3) {
    std::stringstream msg;
    msg << "Invalid shape, expecting shape (" << w << ", " << h << ", 3)";
    throw std::runtime_error(msg.str());
  }

  pixel_t* src = environment->getScreen().getArray();
  pixel_t* dst = (pixel_t*) buffer.mutable_data();

  theOSystem->colourPalette().applyPaletteRGB(dst, src, w * h);
}


void ALEPyInterface::getScreenGrayscale(py::array_t<pixel_t, py::array::c_style> &buffer) {
  py::buffer_info info = buffer.request();
  if (info.ndim != 2)
    throw std::runtime_error("Expected a numpy array with two dimensions.");

  size_t w = environment->getScreen().width();
  size_t h = environment->getScreen().height();

  if (info.shape[0] != h || info.shape[1] != w) {
    std::stringstream msg;
    msg << "Invalid shape, expecting shape (" << w << ", " << h << ")";
    throw std::runtime_error(msg.str());
  }

  pixel_t* src = environment->getScreen().getArray();
  pixel_t* dst = (pixel_t*) buffer.mutable_data();

  theOSystem->colourPalette().applyPaletteGrayscale(dst, src, w * h);
}
