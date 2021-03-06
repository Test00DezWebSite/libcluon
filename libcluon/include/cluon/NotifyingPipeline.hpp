/*
 * Copyright (C) 2017-2018  Christian Berger
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CLUON_NOTIFYINGPIPELINE_HPP
#define CLUON_NOTIFYINGPIPELINE_HPP

#include "cluon/cluon.hpp"

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

namespace cluon {

template <class T>
class LIBCLUON_API NotifyingPipeline {
   private:
    NotifyingPipeline(const NotifyingPipeline &) = delete;
    NotifyingPipeline(NotifyingPipeline &&)      = delete;
    NotifyingPipeline &operator=(const NotifyingPipeline &) = delete;
    NotifyingPipeline &operator=(NotifyingPipeline &&) = delete;

   public:
    NotifyingPipeline(std::function<void(T &&)> delegate)
        : m_delegate(delegate) {
        m_pipelineThread = std::thread(&NotifyingPipeline::processPipeline, this);

        // Let the operating system spawn the thread.
        using namespace std::literals::chrono_literals; // NOLINT
        do { std::this_thread::sleep_for(1ms); } while (!m_pipelineThreadRunning.load());
    }

    ~NotifyingPipeline() {
        m_pipelineThreadRunning.store(false);

        // Wake any waiting threads.
        m_pipelineCondition.notify_all();

        // Joining the thread could fail.
        try {
            if (m_pipelineThread.joinable()) {
                m_pipelineThread.join();
            }
        } catch (...) {} // LCOV_EXCL_LINE
    }

   public:
    inline void add(T &&entry) noexcept {
        std::unique_lock<std::mutex> lck(m_pipelineMutex);
        m_pipeline.emplace_back(entry);
    }

    inline void notifyAll() noexcept { m_pipelineCondition.notify_all(); }

    inline bool isRunning() noexcept { return m_pipelineThreadRunning.load(); }

   private:
    inline void processPipeline() noexcept {
        // Indicate to caller that we are ready.
        m_pipelineThreadRunning.store(true);

        while (m_pipelineThreadRunning.load()) {
            std::unique_lock<std::mutex> lck(m_pipelineMutex);
            // Wait until the thread should stop or data is available.
            m_pipelineCondition.wait(lck, [this] { return (!this->m_pipelineThreadRunning.load() || !this->m_pipeline.empty()); });

            // The condition will automatically lock the mutex after waking up.
            // As we are locking per entry, we need to unlock the mutex first.
            lck.unlock();

            uint32_t entries{0};
            {
                lck.lock();
                entries = static_cast<uint32_t>(m_pipeline.size());
                lck.unlock();
            }
            for (uint32_t i{0}; i < entries; i++) {
                T entry;
                {
                    lck.lock();
                    entry = m_pipeline.front();
                    lck.unlock();
                }

                if (nullptr != m_delegate) {
                    m_delegate(std::move(entry));
                }

                {
                    lck.lock();
                    m_pipeline.pop_front();
                    lck.unlock();
                }
            }
        }
    }

   private:
    std::function<void(T &&)> m_delegate;

    std::atomic<bool> m_pipelineThreadRunning{false};
    std::thread m_pipelineThread{};
    std::mutex m_pipelineMutex{};
    std::condition_variable m_pipelineCondition{};

    std::deque<T> m_pipeline{};
};
} // namespace cluon

#endif
