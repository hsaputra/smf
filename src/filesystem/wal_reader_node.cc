// Copyright (c) 2016 Alexander Gallego. All rights reserved.
//
#include "filesystem/wal_reader_node.h"

#include <memory>
#include <system_error>
#include <utility>
// third party
#include <core/reactor.hh>
#include <core/seastar.hh>
// smf
#include "platform/log.h"
#include "platform/macros.h"

namespace smf {
wal_reader_node::wal_reader_node(uint64_t         epoch,
                                 seastar::sstring _filename,
                                 reader_stats *   s)
  // needed signed for comparisons
  : starting_epoch(static_cast<int64_t>(epoch)),
    filename(_filename),
    rstats_(DTHROW_IFNULL(s)) {}
wal_reader_node::~wal_reader_node() {}

seastar::future<> wal_reader_node::close() {
  if (io_) {
    return io_->close();
  }
  return seastar::make_ready_future<>();
}

seastar::future<> wal_reader_node::open_node() {
  return seastar::open_file_dma(filename, seastar::open_flags::ro)
    .then([this](seastar::file ff) {
      auto f = seastar::make_lw_shared<seastar::file>(std::move(ff));
      io_    = std::make_unique<wal_clock_pro_cache>(f, file_size_);
      return seastar::make_ready_future<>();
    });
}

seastar::future<wal_read_reply::maybe> wal_reader_node::get(
  wal_read_request r) {
  if (SMF_UNLIKELY(io_ == nullptr)) {
    return open_node().then([this, r = std::move(r)] { return this->get(r); });
  }
  if (r.offset >= starting_epoch) {
    r.offset -= starting_epoch;
    r.size = std::min(r.size, ending_epoch());
    return io_->read(r).then([](auto buf) {
      return seastar::make_ready_future<wal_read_reply::maybe>(std::move(buf));
    });
  }
  return seastar::make_ready_future<wal_read_reply::maybe>();
}

seastar::future<> wal_reader_node::open() {
  return seastar::file_size(filename).then(
    [this](auto size) { file_size_ = size; });
}

}  // namespace smf
