/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_RUNTIME_GC_COLLECTOR_STICKY_MARK_SWEEP_H_
#define ART_RUNTIME_GC_COLLECTOR_STICKY_MARK_SWEEP_H_

#include "base/macros.h"
#include "locks.h"
#include "partial_mark_sweep.h"

namespace art {
namespace gc {
namespace collector {

class StickyMarkSweep : public PartialMarkSweep {
 public:
  GcType GetGcType() const {
    return kGcTypeSticky;
  }

  explicit StickyMarkSweep(Heap* heap, bool is_concurrent, const std::string& name_prefix = "");
  ~StickyMarkSweep() {}

 protected:
  // Bind the live bits to the mark bits of bitmaps for all spaces, all spaces other than the
  // alloc space will be marked as immune.
  void BindBitmaps() SHARED_LOCKS_REQUIRED(Locks::mutator_lock_);

  void MarkReachableObjects()
      SHARED_LOCKS_REQUIRED(Locks::mutator_lock_)
      EXCLUSIVE_LOCKS_REQUIRED(Locks::heap_bitmap_lock_);

  virtual void MarkThreadRoots(Thread* self)
      SHARED_LOCKS_REQUIRED(Locks::mutator_lock_)
      EXCLUSIVE_LOCKS_REQUIRED(Locks::heap_bitmap_lock_);

  void Sweep(bool swap_bitmaps) EXCLUSIVE_LOCKS_REQUIRED(Locks::heap_bitmap_lock_);

  // Don't need to do anything special here since we scan all the cards which may have references
  // to the newly allocated objects.
  virtual void UpdateAndMarkModUnion() { }

 private:
  DISALLOW_COPY_AND_ASSIGN(StickyMarkSweep);
};

}  // namespace collector
}  // namespace gc
}  // namespace art

#endif  // ART_RUNTIME_GC_COLLECTOR_STICKY_MARK_SWEEP_H_
