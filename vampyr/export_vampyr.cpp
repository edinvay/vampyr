/*
 *  \date Mar 08, 2018
 *  \author Magnar Bjørgve <magnar.bjorgve@uit.no> \n
 *          Hylleraas Centre for Quantum Molecular Sciences \n
 *          UiT - The Arctic University of Norway
 */

#include <pybind11/pybind11.h>

#include <string>

#include <MRCPP/version.h>
#include <MRCPP/constants.h>

#include "core/bases.h"
#include "functions/functions.h"
#include "functions/gaussians.h"
#include "operators/derivatives.h"
#include "treebuilders/arithmetics.h"
#include "treebuilders/applys.h"
#include "treebuilders/grids.h"
#include "treebuilders/project.h"
#include "trees/trees.h"
#include "trees/world.h"

namespace py = pybind11;
using namespace mrcpp;
using namespace pybind11::literals;

namespace vampyr {

void constants(py::module &m) {
    py::enum_<Traverse>(m, "Traverse")
        .value("TopDown", Traverse::TopDown)
        .value("BottomUp", Traverse::BottomUp)
        .export_values();
    py::enum_<Iterator>(m, "Iterator")
        .value("Lebesgue", Iterator::Lebesgue)
        .value("Hilbert", Iterator::Hilbert)
        .export_values();
}

template <int D> void bind_mr(py::module &mod) noexcept {
    applys<D>(mod);
    arithmetics<D>(mod);
    project<D>(mod);
    grids<D>(mod);

    std::string name = "D" + std::to_string(D);
    py::module sub_mod = mod.def_submodule(name.c_str());
    sub_mod.doc() = std::to_string(D) + "-dimensional bindings";

    functions<D>(sub_mod);
    gaussians<D>(sub_mod);
    derivatives<D>(sub_mod);
    trees<D>(sub_mod);
    world<D>(sub_mod);
}

PYBIND11_MODULE(_vampyr, m) {
    using namespace mrcpp;
    m.doc() = "VAMPyR makes the MRCPP functionality available through a Python interface";

    m.def("mrcpp_version", &mrcpp::program_version);

    // Dimension-independent bindings go in the main module
    constants(m);
    bases(m);

    // py::class_<BandWidth>(m, "BandWidth")
    //     .def(py::init<int>())
    //     .def("clear", &BandWidth::clear)
    //     .def("isEmpty", &BandWidth::isEmpty)
    //     .def("getDepth", &BandWidth::getDepth)
    //     .def("getMaxWidth", &BandWidth::getMaxWidth)
    //     .def("setWidth", &BandWidth::setWidth);

    // mw_operator(m);
    // NOTE We bind GreensKernel first, since it's the base class for the 3 following
    // py::class_<GreensKernel>(m, "GreensKernel");
    // derivative_kernel(m);
    // helmholtz_kernel(m);
    // identity_kernel(m);

    // Dimension-dependent bindings go into submodules
    bind_mr<1>(m);
    // NOTE We bind these 1D functions here as we need RepresentableFunction<1> bound first
    // polynomial(m);
    // legendre_poly(m);
    // boys_function(m);

    bind_mr<2>(m);
    // NOTE We bind these down here as we need MWTree<2> and SerialTree<2> as base classes
    // py::class_<OperatorTree, MWTree<2>>(m, "OperatorTree")
    //     .def(py::init<MultiResolutionAnalysis<2> &, double>())
    //     .def("getNormPrecision", &OperatorTree::getNormPrecision)
    //     .def("calcBandWidth", py::overload_cast<double>(&OperatorTree::calcBandWidth))
    //     .def("clearBandWidth", &OperatorTree::clearBandWidth)
    //     .def("setupOperNodeCache", &OperatorTree::setupOperNodeCache)
    //     .def("clearOperNodeCache", &OperatorTree::clearOperNodeCache)
    //     .def("mwTransformDown", &OperatorTree::mwTransformDown)
    //     .def("mwTransformUp", &OperatorTree::mwTransformUp);
    // pybind11::class_<SerialOperatorTree, SerialTree<2>>(m, "SerialOperatorTree")
    //     .def(py::init<OperatorTree *>())
    //     .def("allocRoots", &SerialOperatorTree::allocRoots)
    //     .def("allocChildren", &SerialOperatorTree::allocChildren)
    //     .def("allocGenChildren", &SerialOperatorTree::allocGenChildren)
    //     .def("deallocNodes", &SerialOperatorTree::deallocNodes)
    //     .def("deallocGenNodes", &SerialOperatorTree::deallocGenNodes)
    //     .def("deallocGenNodeChunks", &SerialOperatorTree::deallocGenNodeChunks);

    bind_mr<3>(m);
}
} // namespace vampyr
