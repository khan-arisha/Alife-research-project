#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_POOLSPEC_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_POOLSPEC_HPP_INCLUDE

#include <vector>

#include "../../../../../spouts/wrappers/TrivialSpoutWrapper.hpp"

#include "../../../../mock/ThrowDuct.hpp"

namespace uit {

template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class PoolSpec {

  using THIS_T = PoolSpec<ImplSpec, ProcDuct>;

public:

  using T = std::vector<typename ImplSpec::T>;

  template<typename Inlet>
  using inlet_wrapper_t =
    typename uit::TrivialSpoutWrapper<T>::template inlet_wrapper_t<Inlet>;
  template<typename Outlet>
  using outlet_wrapper_t =
    typename uit::TrivialSpoutWrapper<T>::template outlet_wrapper_t<Outlet>;

  constexpr inline static size_t N{ ImplSpec::N };
  constexpr inline static size_t B{ ImplSpec::B };

  using IntraDuct = uit::ThrowDuct<THIS_T>;
  using ThreadDuct = uit::ThrowDuct<THIS_T>;

  using ProcInletDuct = typename ProcDuct<THIS_T>::InletImpl;
  using ProcOutletDuct = typename ProcDuct<THIS_T>::OutletImpl;
  using ProcBackEnd = typename ProcDuct<THIS_T>::BackEndImpl;

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_POOLSPEC_HPP_INCLUDE
