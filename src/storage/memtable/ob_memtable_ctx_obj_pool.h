/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef OCEANBASE_STORAGE_MEMTABLE_OB_MEMTABLE_CTX_OBJ_POOL_
#define OCEANBASE_STORAGE_MEMTABLE_OB_MEMTABLE_CTX_OBJ_POOL_

#include <stdint.h>
#include "storage/tablelock/ob_mem_ctx_table_lock.h"

namespace oceanbase
{
namespace memtable
{

class ObMemtableCtxObjPool
{
public:

#ifdef OB_ENABLE_MEMTABLE_CTX_OBJ_CACHE_DEBUG
  ObMemtableCtxObjPool(common::ObIAllocator &allocator)
      : lock_op_node_pool_(allocator),
        lock_callback_pool_(allocator),
        mvcc_callback_pool_(allocator),
        hit_mvcc_cb_cache_cnt_(0),
        alloc_mvcc_cb_cnt_(0),
        hit_lock_cb_cache_cnt_(0),
        alloc_lock_cb_cnt_(0),
        hit_lock_op_cache_cnt_(0),
        alloc_lock_op_cnt_(0) {}
#else
  ObMemtableCtxObjPool(common::ObIAllocator &allocator)
      : lock_op_node_pool_(allocator), lock_callback_pool_(allocator), mvcc_callback_pool_(allocator) {}
#endif

  ObMemtableCtxObjPool() = delete;

  template <typename T>
  void *alloc();

#ifdef OB_ENABLE_MEMTABLE_CTX_OBJ_CACHE_DEBUG
  template <>
  void *alloc<transaction::tablelock::ObMemCtxLockOpLinkNode>()
  {
    bool hit_cache = false;
    void *res = lock_op_node_pool_.alloc(hit_cache);
    ATOMIC_INC(&alloc_lock_op_cnt_);
    if (hit_cache) {
      ATOMIC_INC(&hit_lock_op_cache_cnt_);
    }
    if (REACH_TIME_INTERVAL(1000 * 1000)) {
      STORAGE_LOG(INFO, "cache statistic", K(alloc_lock_op_cnt_), K(hit_lock_op_cache_cnt_));
    }
    return res;
  }

  template <>
  void *alloc<transaction::tablelock::ObOBJLockCallback>()
  {
    bool hit_cache = false;
    void *res = lock_callback_pool_.alloc(hit_cache);
    ATOMIC_INC(&alloc_lock_cb_cnt_);
    if (hit_cache) {
      ATOMIC_INC(&hit_lock_cb_cache_cnt_);
    }
    if (REACH_TIME_INTERVAL(1000 * 1000)) {
      STORAGE_LOG(INFO, "cache statistic", K(alloc_lock_cb_cnt_), K(hit_lock_cb_cache_cnt_));
    }
    return res;
  }

  template <>
  void *alloc<memtable::ObMvccRowCallback>()
  {
    bool hit_cache = false;
    void *res = mvcc_callback_pool_.alloc(hit_cache);
    ATOMIC_INC(&alloc_mvcc_cb_cnt_);
    if (hit_cache) {
      ATOMIC_INC(&hit_mvcc_cb_cache_cnt_);
    }
    if (REACH_TIME_INTERVAL(1000 * 1000)) {
      STORAGE_LOG(INFO, "cache statistic", K(alloc_mvcc_cb_cnt_), K(hit_mvcc_cb_cache_cnt_));
    }
    return res;
  }
#else
  template <>
  void *alloc<transaction::tablelock::ObMemCtxLockOpLinkNode>()
  {
    return lock_op_node_pool_.alloc();
  }

  template <>
  void *alloc<transaction::tablelock::ObOBJLockCallback>()
  {
    return lock_callback_pool_.alloc();
  }

  template <>
  void *alloc<memtable::ObMvccRowCallback>()
  {
    return mvcc_callback_pool_.alloc();
  }
#endif

  template <typename T>
  void free(void *);

  template <>
  void free<transaction::tablelock::ObMemCtxLockOpLinkNode>(void *obj)
  {
    lock_op_node_pool_.free(obj);
  }

  template <>
  void free<transaction::tablelock::ObOBJLockCallback>(void *obj)
  {
    lock_callback_pool_.free(obj);
  }

  template <>
  void free<memtable::ObMvccRowCallback>(void *obj)
  {
    mvcc_callback_pool_.free(obj);
  }

  void reset()
  {
    lock_op_node_pool_.reset();
    lock_callback_pool_.reset();
    mvcc_callback_pool_.reset();
  }

private:
  static constexpr int64_t OBJ_NUM = 2;
  ObArenaObjPool<transaction::tablelock::ObMemCtxLockOpLinkNode, OBJ_NUM> lock_op_node_pool_;
  ObArenaObjPool<transaction::tablelock::ObOBJLockCallback, OBJ_NUM> lock_callback_pool_;
  ObArenaObjPool<memtable::ObMvccRowCallback, OBJ_NUM> mvcc_callback_pool_;

#ifdef OB_ENABLE_MEMTABLE_CTX_OBJ_CACHE_DEBUG
  int64_t hit_mvcc_cb_cache_cnt_;
  int64_t alloc_mvcc_cb_cnt_;
  int64_t hit_lock_cb_cache_cnt_;
  int64_t alloc_lock_cb_cnt_;
  int64_t hit_lock_op_cache_cnt_;
  int64_t alloc_lock_op_cnt_;
#endif

};

}  // namespace memtable
}  // namespace oceanbase

#endif
