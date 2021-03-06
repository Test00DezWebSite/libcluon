/*
 * Copyright (C) 2017-2018  Christian Berger
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// clang-format off
#ifdef WIN32
    #include <Winsock2.h> // for htonll
#endif
// clang-format on

#include "cluon/ToLCMVisitor.hpp"

#include <cstring>

namespace cluon {

std::string ToLCMVisitor::encodedData(bool withHash) const noexcept {
    int64_t _hash = hash();
    _hash         = static_cast<int64_t>(htobe64(_hash));

    std::stringstream hashBuffer;
    hashBuffer.write(reinterpret_cast<const char *>(&_hash), sizeof(int64_t));

    const std::string s{(withHash ? hashBuffer.str() : "") + m_buffer.str()};
    return s;
}

////////////////////////////////////////////////////////////////////////////////

void ToLCMVisitor::preVisit(int32_t id, const std::string &shortName, const std::string &longName) noexcept {
    (void)id;
    (void)shortName;
    (void)longName;
}

void ToLCMVisitor::postVisit() noexcept {}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, bool &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("boolean");
    calculateHash(0);
    m_buffer.write(reinterpret_cast<char *>(&v), sizeof(bool));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, char &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int8_t");
    calculateHash(0);
    m_buffer.write(reinterpret_cast<char *>(&v), sizeof(char));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, int8_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int8_t");
    calculateHash(0);
    m_buffer.write(reinterpret_cast<char *>(&v), sizeof(int8_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, uint8_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int8_t");
    calculateHash(0);
    m_buffer.write(reinterpret_cast<char *>(&v), sizeof(uint8_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, int16_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int16_t");
    calculateHash(0);
    int16_t _v = static_cast<int16_t>(htobe16(v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int16_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, uint16_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int16_t");
    calculateHash(0);
    int16_t _v = static_cast<int16_t>(htobe16(v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int16_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, int32_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int32_t");
    calculateHash(0);
    int32_t _v = static_cast<int32_t>(htobe32(v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int32_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, uint32_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int32_t");
    calculateHash(0);
    int32_t _v = static_cast<int32_t>(htobe32(v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int32_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, int64_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int64_t");
    calculateHash(0);
    int64_t _v = static_cast<int64_t>(htobe64(v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int64_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, uint64_t &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("int64_t");
    calculateHash(0);
    int64_t _v = static_cast<int64_t>(htobe64(v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int64_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, float &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("float");
    calculateHash(0);
    int32_t _v{0};
    std::memmove(&_v, &v, sizeof(int32_t));
    _v = static_cast<int32_t>(htobe32(_v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int32_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, double &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("double");
    calculateHash(0);
    int64_t _v{0};
    std::memmove(&_v, &v, sizeof(int64_t));
    _v = static_cast<int64_t>(htobe64(_v));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int64_t));
}

void ToLCMVisitor::visit(uint32_t id, std::string &&typeName, std::string &&name, std::string &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("string");
    calculateHash(0);

    const std::size_t LENGTH = v.length();
    int32_t _v               = static_cast<int32_t>(htobe32(static_cast<uint32_t>(LENGTH + 1)));
    m_buffer.write(reinterpret_cast<char *>(&_v), sizeof(int32_t));
    m_buffer.write(v.c_str(), static_cast<std::streamsize>(LENGTH)); // LENGTH won't be negative.
    m_buffer << '\0';
}

////////////////////////////////////////////////////////////////////////////////

int64_t ToLCMVisitor::hash() const noexcept {
    // Apply ZigZag encoding for hash from this message's fields and depending
    // hashes for complex nested types.
    int64_t tmp{m_hash};
    for (int64_t v : m_hashes) { tmp += v; }

    const int64_t hash = (tmp << 1) + ((tmp >> 63) & 1);
    return hash;
}

void ToLCMVisitor::calculateHash(char c) noexcept {
    m_hash = ((m_hash << 8) ^ (m_hash >> 55)) + c;
}

void ToLCMVisitor::calculateHash(const std::string &s) noexcept {
    const std::string tmp{(s.length() > 255 ? s.substr(0, 255) : s)};
    const uint8_t length{static_cast<uint8_t>(tmp.length())};
    calculateHash(static_cast<char>(length));
    for (auto c : s) { calculateHash(c); }
}

} // namespace cluon
