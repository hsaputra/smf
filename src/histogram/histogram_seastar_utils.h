// Copyright (c) 2016 Alexander Gallego. All rights reserved.
//
#pragma once
// std
#include <cstdio>
// seastar
#include <core/fstream.hh>
// smf
#include "histogram/histogram.h"


namespace smf {
struct histogram_seastar_utils {
  static seastar::future<seastar::temporary_buffer<char>> print_histogram(
    histogram h);
  static seastar::future<> write_histogram(seastar::sstring filename,
                                           histogram        h);
};

}  // namespace smf
