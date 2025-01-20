//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"
#include "common/exception.h"

namespace bustub {

std::time_t LRUKNode::CurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t timestamp = now.time_since_epoch().count();
  return timestamp;
}

/**
 *
 * TODO(P1): Add implementation
 *
 * @brief a new LRUKReplacer.
 * @param num_frames the maximum number of frames the LRUReplacer will be required to store
 */
LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}

/**
 * TODO(P1): Add implementation
 *
 * @brief Find the frame with largest backward k-distance and evict that frame. Only frames
 * that are marked as 'evictable' are candidates for eviction.
 *
 * A frame with less than k historical references is given +inf as its backward k-distance.
 * If multiple frames have inf backward k-distance, then evict frame whose oldest timestamp
 * is furthest in the past.
 *
 * Successful eviction of a frame should decrement the size of replacer and remove the frame's
 * access history.
 *
 * @return true if a frame is evicted successfully, false if no frames can be evicted.
 */
auto LRUKReplacer::Evict() -> std::optional<frame_id_t> {
  // std::time_t timestamp = 0;
  // frame_id_t lt_k_node = 0;
  // frame_id_t k_node = 0;
  // for (const auto &node : node_store_) {
  //   if (node.second.is_evictable_ && node.second.history_.size() == k_) {
  //     if (timestamp < node.second.history_.front()) {
  //       timestamp = node.second.history_.front();
  //       k_node = node.first;
  //     }
  //   } else {
  //     if (node.second.is_evictable_ && timestamp < node.second.history_.front()) {
  //       timestamp = node.second.history_.front();
  //       lt_k_node = node.first;
  //     }
  //   }
  // }
  // if (k_node != 0) {
  //   node_store_.erase(k_node);
  //   return std::optional<frame_id_t>(lt_k_node);
  // }
  // frame_id_t should_be_evictable = ;
  return std::nullopt;
}

/**
 * TODO(P1): Add implementation
 *
 * @brief Record the event that the given frame id is accessed at current timestamp.
 * Create a new entry for access history if frame id has not been seen before.
 *
 * If frame id is invalid (ie. larger than replacer_size_), throw an exception. You can
 * also use BUSTUB_ASSERT to abort the process if frame id is invalid.
 *
 * @param frame_id id of frame that received a new access.
 * @param access_type type of access that was received. This parameter is only needed for
 * leaderboard tests.
 */
void LRUKReplacer::RecordAccess(frame_id_t frame_id, [[maybe_unused]] AccessType access_type) {
  BUSTUB_ASSERT(frame_id >= 0 && frame_id < replacer_size_, "Invalid frame id");
  if (const auto it = node_store_.find(frame_id); it == node_store_.end()) {
    LRUKNode node(k_, frame_id);
    node.history_.push_front(LRUKNode::CurrentTimestamp());
    node_store_.insert({frame_id, node});
  } else {
    size_t size = it->second.history_.size();
    if (size > it->second.k_) {
      it->second.history_.pop_back();
    }
    it->second.history_.push_front(LRUKNode::CurrentTimestamp());
  }
  current_timestamp_ = LRUKNode::CurrentTimestamp();
}

/**
 * TODO(P1): Add implementation
 *
 * @brief Toggle whether a frame is evictable or non-evictable. This function also
 * controls replacer's size. Note that size is equal to number of evictable entries.
 *
 * If a frame was previously evictable and is to be set to non-evictable, then size should
 * decrement. If a frame was previously non-evictable and is to be set to evictable,
 * then size should increment.
 *
 * If frame id is invalid, throw an exception or abort the process.
 *
 * For other scenarios, this function should terminate without modifying anything.
 *
 * @param frame_id id of frame whose 'evictable' status will be modified
 * @param set_evictable whether the given frame is evictable or not
 */
void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
  BUSTUB_ASSERT(frame_id >= 0 && frame_id < replacer_size_, "Invalid frame id");
  const auto it = node_store_.find(frame_id);
  if (it->second.is_evictable_ && !set_evictable) {
    it->second.is_evictable_ = set_evictable;
    curr_size_--;
  } else if (!it->second.is_evictable_ && set_evictable) {
    it->second.is_evictable_ = set_evictable;
    curr_size_++;
  }
}

/**
 * TODO(P1): Add implementation
 *
 * @brief Remove an evictable frame from replacer, along with its access history.
 * This function should also decrement replacer's size if removal is successful.
 *
 * Note that this is different from evicting a frame, which always remove the frame
 * with largest backward k-distance. This function removes specified frame id,
 * no matter what its backward k-distance is.
 *
 * If Remove is called on a non-evictable frame, throw an exception or abort the
 * process.
 *
 * If specified frame is not found, directly return from this function.
 *
 * @param frame_id id of frame to be removed
 */
void LRUKReplacer::Remove(frame_id_t frame_id) {
  const auto it = node_store_.find(frame_id);
  if (it == node_store_.end()) {
    return;
  }
  if (it->second.is_evictable_) {
    node_store_.erase(it);
  } else {
    BUSTUB_ASSERT(it->second.is_evictable_, "Invalid frame id");
  }

}

/**
 * TODO(P1): Add implementation
 *
 * @brief Return replacer's size, which tracks the number of evictable frames.
 *
 * @return size_t
 */
auto LRUKReplacer::Size() -> size_t { return curr_size_; }

}  // namespace bustub
