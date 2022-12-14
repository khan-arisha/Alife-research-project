#pragma once

#include <memory>

#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNode.hpp"

using message_t = size_t;

#include "ImplSpec.hpp"

using submesh_t = typename netuit::Mesh<ImplSpec>::submesh_t;

using node_t = netuit::MeshNode<ImplSpec>;

using inlet_instrumentation_aggregating_t = node_t::output_t;
using outlet_instrumentation_aggregating_t = node_t::input_t;
