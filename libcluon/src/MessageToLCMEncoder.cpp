/*
 * Copyright (C) 2017  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cluon/MessageToLCMEncoder.hpp"

#include <inttypes.h>
#include <cstdio>
#include <iostream>

namespace cluon {

std::string MessageToLCMEncoder::encodedData() const noexcept {
    uint64_t _hash = hash();
    _hash = htobe64(_hash);
    std::stringstream hashBuffer;
    hashBuffer.write(reinterpret_cast<const char *>(&_hash), sizeof(uint64_t));

    const std::string s{hashBuffer.str() + m_buffer.str()};
    return s;
}

////////////////////////////////////////////////////////////////////////////////

void MessageToLCMEncoder::preVisit(uint32_t id, const std::string &shortName, const std::string &longName) noexcept {
    (void)id;
    (void)shortName;
    (void)longName;
}

void MessageToLCMEncoder::postVisit() noexcept {}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, bool &v) noexcept {
    (void)id;
    (void)typeName;
    calculateHash(name);
    calculateHash("boolean");
    calculateHash(0);
//    m_hash = calculateHash(m_hash, name);
//    m_hash = calculateHash(m_hash, "boolean");
//    m_hash = calculateHash(m_hash, 0);
    m_buffer.write(reinterpret_cast<char*>(&v), sizeof(bool));
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, char &v) noexcept {
    (void)id;
    (void)typeName;
    m_hash = calculateHash(m_hash, name);
    m_hash = calculateHash(m_hash, "int8_t");
    m_hash = calculateHash(m_hash, 0);
    m_buffer.write(reinterpret_cast<char*>(&v), sizeof(char));
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, int8_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, uint8_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, int16_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, uint16_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, int32_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, uint32_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, int64_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, uint64_t &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, float &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, double &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

void MessageToLCMEncoder::visit(uint32_t id, std::string &&typeName, std::string &&name, std::string &v) noexcept {
    (void)id;
    (void)typeName;
    std::cout << "LCM " << name << ", " << v << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int64_t MessageToLCMEncoder::hash() const noexcept {
    // Apply ZigZag encoding for hash.
    const int64_t hash = (m_hash << 1) + ( (m_hash >> 63) & 1 );
    return hash;
}

void MessageToLCMEncoder::calculateHash(char c) noexcept {
    m_hash = ((m_hash<<8) ^ (m_hash>>55)) + c;
}

void MessageToLCMEncoder::calculateHash(const std::string &s) noexcept {
    calculateHash(s.length());
    for (auto c : s) {
        calculateHash(c);
    }
}

int64_t MessageToLCMEncoder::calculateHash(int64_t v, char c) const noexcept {
    v = ((v<<8) ^ (v>>55)) + c;
    return v;
}

int64_t MessageToLCMEncoder::calculateHash(int64_t v, const std::string &s) const noexcept {
    v = calculateHash(v, s.length());
    for (auto c : s) {
        v = calculateHash(v, c);
    }
    return v;
}

} // namespace cluon
