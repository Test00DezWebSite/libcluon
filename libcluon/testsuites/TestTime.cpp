/*
 * Copyright (C) 2017-2018  Christian Berger
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

#include "catch.hpp"

#include "cluon/Time.hpp"
#include "cluon/cluonDataStructures.hpp"
#include <chrono>
#include <thread>

TEST_CASE("Test Time.") {
    cluon::data::TimeStamp before{cluon::time::now()};

    using namespace std::literals::chrono_literals; // NOLINT
    std::this_thread::sleep_for(1s);

    cluon::data::TimeStamp after{cluon::time::now()};

    REQUIRE((after.seconds() * 1000 * 1000 + after.microseconds()) > (before.seconds() * 1000 * 1000 + before.microseconds()));
}

TEST_CASE("Test convert Time.") {
    cluon::data::TimeStamp before{cluon::time::now()};

    using namespace std::literals::chrono_literals; // NOLINT
    std::this_thread::sleep_for(1s);

    std::chrono::system_clock::time_point after = std::chrono::system_clock::now();

    cluon::data::TimeStamp after2 = cluon::time::convert(after);

    REQUIRE((after2.seconds() * 1000 * 1000 + after2.microseconds()) > (before.seconds() * 1000 * 1000 + before.microseconds()));
}
