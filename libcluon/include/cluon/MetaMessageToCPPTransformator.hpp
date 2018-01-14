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

#ifndef METAMESSAGETOCPPTRANSFORMATOR_HPP
#define METAMESSAGETOCPPTRANSFORMATOR_HPP

#include "cluon/cluon.hpp"
#include "cluon/MetaMessage.hpp"
#include "Mustache/mustache.hpp"

#include <string>

namespace cluon {
/**
This class transforms a given MetaMessage to a C++ header and source content.

Transformation is using https://github.com/kainjow/Mustache.
*/
class LIBCLUON_API MetaMessageToCPPTransformator {
   private:
    MetaMessageToCPPTransformator(MetaMessageToCPPTransformator &&) = delete;
    MetaMessageToCPPTransformator &operator=(const MetaMessageToCPPTransformator &) = delete;
    MetaMessageToCPPTransformator &operator=(MetaMessageToCPPTransformator &&) = delete;

   public:
    MetaMessageToCPPTransformator()                                      = default;
    MetaMessageToCPPTransformator(const MetaMessageToCPPTransformator &) = default;

    /**
     * The method is called from MetaMessage to visit itself using this transformator.
     *
     * @param mm MetaMessage to visit.
     */
    void visit(const MetaMessage &mm) noexcept;

    /**
     * @return Content of the C++ header.
     */
    std::string contentHeader() noexcept;
    /**
     * @return Content of the C++ source.
     */
    std::string contentSource() noexcept;

   private:
    kainjow::mustache::data m_dataToBeRendered{};
    kainjow::mustache::data m_fields{kainjow::mustache::data::type::list};
};
} // namespace cluon

#endif
