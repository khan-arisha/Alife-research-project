#pragma once
#ifndef UIT_EMP_IO_CONTIGUOUSSTREAM_HPP_INCLUDE
#define UIT_EMP_IO_CONTIGUOUSSTREAM_HPP_INCLUDE

#ifdef UIT_VENDORIZE_EMP
#include "../vendorization/push_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

#include "../../../third-party/Empirical/include/emp/io/ContiguousStream.hpp"

#ifndef UIT_VENDORIZE_EMP
namespace emp {}
namespace uit_emp = emp;
#endif // #ifndef UIT_VENDORIZE_EMP

#ifdef UIT_VENDORIZE_EMP
#include "../vendorization/pop_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

#endif // #ifndef UIT_EMP_IO_CONTIGUOUSSTREAM_HPP_INCLUDE
