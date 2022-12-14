#pragma once
#ifndef UITSL_UTILITY_KEYNAME_KEY_UNION_HPP_INCLUDE
#define UITSL_UTILITY_KEYNAME_KEY_UNION_HPP_INCLUDE

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "../../uit_emp/tools/keyname_utils.hpp"

#include "../polyfill/filesystem.hpp"
#include "../polyfill/identity.hpp"

namespace uitsl {

std::set< std::string > keyname_key_union(
  const std::vector< std::string >& packs
) {

  std::set < std:: string > res;
  for ( const auto& pack : packs ) {
    for ( const auto& [k, v] : uit_emp::keyname::unpack( pack ) ) {
      res.insert( k );
    }
  }

  return res;

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_KEYNAME_KEY_UNION_HPP_INCLUDE
