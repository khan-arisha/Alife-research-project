#pragma once
#ifndef SGPL_ALGORITHM_ADVANCE_CORE_HPP_INCLUDE
#define SGPL_ALGORITHM_ADVANCE_CORE_HPP_INCLUDE

#include <cassert>
#include <tuple>

#include "../../../third-party/conduit/include/uit_emp/base/macros.hpp"

#include "../debug/sgpl_assert.hpp"
#include "../hardware/Core.hpp"
#include "../program/Instruction.hpp"
#include "../program/Program.hpp"
#include "../utility/ByteEnumeration.hpp"

namespace sgpl {
#include "../../../third-party/conduit/include/uit_emp/vendorization/push_macros.hh"

// tried a dispatch table, seemed to do about the same or worse
// https://eli.thegreenplace.net/2012/07/12/computed-goto-for-efficient-dispatch-tables
template<typename Spec>
__attribute__ ((hot))
inline void advance_core(
  sgpl::Core<Spec>& state,
  const sgpl::Program<Spec>& program,
  typename Spec::peripheral_t& peripheral
) {

  using library_t = typename Spec::library_t;

  assert( program.size() );

  const auto& instruction = program[ state.GetProgramCounter() ];

  assert( instruction.op_code < library_t::GetSize() );

  #define SGPL_CASE_PAYLOAD(N) \
    case N: \
      if constexpr (N < library_t::GetSize()) { \
        using Functor = typename library_t::template Operation<N>; \
        Functor::template run<Spec>(state, instruction, program, peripheral);\
      } else { \
        assert( false && N ); \
        __builtin_unreachable(); \
      } \
    break;

    static_assert( library_t::GetSize() < 256 );

  switch( instruction.op_code ) {

    EMP_WRAP_EACH( SGPL_CASE_PAYLOAD, SGPL_BYTE_ENUMERATION )

    default:
      sgpl_assert( false, instruction.op_code );
      __builtin_unreachable();

  }

  state.AdvanceProgramCounter( program.size() );

};

template<typename Spec>
inline void advance_core(
  sgpl::Core<Spec>& state,
  const sgpl::Program<Spec>& program
) {

  using peripheral_t = typename Spec::peripheral_t;
  peripheral_t peripheral;

  advance_core<Spec>(state, program, peripheral);

}

#include "../../../third-party/conduit/include/uit_emp/vendorization/pop_macros.hh"
} // namespace sgpl

#endif // #ifndef SGPL_ALGORITHM_ADVANCE_CORE_HPP_INCLUDE
