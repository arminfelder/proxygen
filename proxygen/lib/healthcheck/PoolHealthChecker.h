/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <folly/SocketAddress.h>
#include <proxygen/lib/utils/Time.h>

#include "proxygen/lib/healthcheck/ServerHealthCheckerCallback.h"

namespace proxygen {

/*
 * Interface for a collection of server health checkers.
 * Public methods can be accessed from any thread.
 *
 * It is necessary to remove all servers or call deleteAllCheckers (blocking)
 * before deleting this object.
 */
class PoolHealthChecker {
 public:
  /*
   * Start/stop all the health checkers in the pools.
   */
  virtual void start() = 0;
  virtual void stop() = 0;

  /*
   * Blocking method that deletes all checkers in appropriate threads and waits
   * for them to complete.
   */
  virtual void deleteAllCheckers() = 0;

  virtual void addServer(
      const std::string& name,
      const folly::SocketAddress& address,
      bool isSecure,
      std::shared_ptr<ServerHealthCheckerCallback> callback) = 0;

  virtual void removeServer(const folly::SocketAddress& address) = 0;

  virtual std::chrono::milliseconds getCheckInterval() const = 0;

  /**
   * If external health checking is used, set last update time here to
   * avoid redundant local health checks.
   */
  virtual void setLastExternalUpdateTime(
      std::vector<folly::SocketAddress>&& addresses,
      TimePoint t) {}

  virtual ~PoolHealthChecker() {
  }
};

} // namespace proxygen
