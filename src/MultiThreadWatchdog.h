/**
 *
 *
 */

#ifndef _H_MultiThreadWatchdog_H_
#define _H_MultiThreadWatchdog_H_

#include <iostream>
#include <unordered_map>
#include <list>
#include <mutex>
#include <atomic>
#include <thread>
#include <exception>

class MultiThreadWatchdog {
public:
    using Interval_t = std::chrono::duration<std::chrono::seconds>;
    using ThreadId_t = std::thread::id;
    using Time_t = std::chrono::time_point<std::chrono::system_clock>;

    MultiThreadWatchdog(double interval, size_t maxNumClients) :  m_maxNumClients(maxNumClients), m_interval(interval) {
        m_bgThread = std::thread(&MultiThreadWatchdog::bgThread, this);
    }

    bool Kick(const ThreadId_t &id = std::this_thread::get_id()) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_hashMap.count(id)) {
            m_list.erase(m_hashMap[id]);
            m_hashMap.erase(id);
            --m_currentNumClients;
        } else if (m_currentNumClients == m_maxNumClients) {
            return false;
        }

        auto expireTime = std::chrono::system_clock::now();
        expireTime += std::chrono::duration_cast<std::chrono::seconds>(m_interval);
        m_list.emplace_front(std::make_pair(id, expireTime));
        m_hashMap[id] = m_list.begin();
        ++m_currentNumClients;

        std::cout << "Kickd from: " << id << std::endl;
        return true;
    }

    bool Done(const ThreadId_t &id = std::this_thread.get_id()) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_hashMap.count(id)) {
            m_list.erase(m_hashMap[id]);
            m_hashMap.erase(id);
            --m_currentNumClients;

            std::cout << "Thread: " << id << " is done!" << std::endl;
            return true;
        }
        return false;
    }

    bool IsExpired(ThreadId_t &expiredId) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_list.size() && m_list.back().second < std::chrono::system_clock::now()) {
            expiredId = m_list.back().first;
            return true;
        }
        return false;
    }

    auto GetNextExpireTime() {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto expireTime = std::chrono::system_clock::now();
        expireTime += std::chrono::duration_cast<std::chrono::seconds>(m_interval);
        return m_list.size() ? m_list.back().second : expireTime;



}
