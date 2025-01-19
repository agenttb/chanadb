//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

#include <algorithm>

namespace bustub {

/**
 * Create a new LRUReplacer.
 * @param num_pages the maximum number of pages the LRUReplacer will be required to store
 */
LRUReplacer::LRUReplacer(size_t num_pages) : num_pages_(num_pages) {}

/**
 * Destroys the LRUReplacer.
 */
LRUReplacer::~LRUReplacer() = default;

auto LRUReplacer::Victim(frame_id_t *frame_id) -> bool {

  if (frame_id == nullptr) {
    return false;
  }
  if (unpinned_.empty()) {
    return false;
  }
  *frame_id = unpinned_.back();
  unpinned_.pop_back();
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {

  auto iter = std::find(unpinned_.begin(), unpinned_.end(), frame_id);
  if (iter != unpinned_.end()) {
    unpinned_.erase(iter);
    pinned_.push_front(frame_id);
  }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  auto size = unpinned_.size() + pinned_.size();
  if (size >= num_pages_) {
    return;
  }
  auto iter = std::find(unpinned_.begin(), unpinned_.end(), frame_id);
  if (iter == unpinned_.end()) {
    unpinned_.push_front(frame_id);
  }
}

auto LRUReplacer::Size() -> size_t { return unpinned_.size(); }

}  // namespace bustub
