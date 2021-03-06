/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef _HLLSKETCHIMPL_HPP_
#define _HLLSKETCHIMPL_HPP_

#include "HllUtil.hpp"
#include "hll.hpp" // for TgtHllType
#include "PairIterator.hpp"

#include <memory>

namespace datasketches {

template<typename A = std::allocator<char>>
class HllSketchImpl {
  public:
    HllSketchImpl(int lgConfigK, target_hll_type tgtHllType, hll_mode mode, bool startFullSize);
    virtual ~HllSketchImpl();

    virtual void serialize(std::ostream& os, bool compact) const = 0;
    virtual std::pair<std::unique_ptr<uint8_t, std::function<void(uint8_t*)>>, const size_t> serialize(bool compact, unsigned header_size_bytes) const = 0;

    virtual HllSketchImpl* copy() const = 0;
    virtual HllSketchImpl* copyAs(target_hll_type tgtHllType) const = 0;
    HllSketchImpl<A>* reset();

    virtual std::function<void(HllSketchImpl<A>*)> get_deleter() const = 0;

    virtual HllSketchImpl* couponUpdate(int coupon) = 0;

    hll_mode getCurMode() const;

    virtual double getEstimate() const = 0;
    virtual double getCompositeEstimate() const = 0;
    virtual double getUpperBound(int numStdDev) const = 0;
    virtual double getLowerBound(int numStdDev) const = 0;

    virtual pair_iterator_with_deleter<A> getIterator() const = 0;

    int getLgConfigK() const;

    virtual int getMemDataStart() const = 0;

    virtual int getPreInts() const = 0;

    target_hll_type getTgtHllType() const;

    virtual int getUpdatableSerializationBytes() const = 0;
    virtual int getCompactSerializationBytes() const = 0;

    virtual bool isCompact() const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool isOutOfOrderFlag() const = 0;
    virtual void putOutOfOrderFlag(bool oooFlag) = 0;
    bool isStartFullSize() const;

  protected:
    static target_hll_type extractTgtHllType(uint8_t modeByte);
    static hll_mode extractCurMode(uint8_t modeByte);
    uint8_t makeFlagsByte(bool compact) const;
    uint8_t makeModeByte() const;

    const int lgConfigK;
    const target_hll_type tgtHllType;
    const hll_mode mode;
    const bool startFullSize;
};

}

#endif // _HLLSKETCHIMPL_HPP_
