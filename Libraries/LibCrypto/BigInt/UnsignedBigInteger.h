/*
 * Copyright (c) 2020, Itamar S. <itamar8910@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include <AK/LogStream.h>
#include <AK/Types.h>
#include <AK/Vector.h>

namespace Crypto {
class UnsignedBigInteger {
public:
    UnsignedBigInteger(u32 x) { m_words.append(x); }
    UnsignedBigInteger() {}

    static UnsignedBigInteger create_invalid();

    const AK::Vector<u32>& words() const { return m_words; }

    UnsignedBigInteger add(const UnsignedBigInteger& other);
    UnsignedBigInteger sub(const UnsignedBigInteger& other);

    size_t length() const { return m_words.size(); }

    // The "trimmed length" is the number of words after trimming leading zeroed words
    size_t trimmed_length() const;

    bool operator==(const UnsignedBigInteger& other) const;
    bool operator<(const UnsignedBigInteger& other) const;

    void invalidate() { m_is_invalid = true; }
    bool is_invalid() const { return m_is_invalid; }

private:
    AK::Vector<u32> m_words;

    // Used to indicate a negative result, or a result of an invalid operation
    bool m_is_invalid { false };
};

}

inline const LogStream& operator<<(const LogStream& stream, const Crypto::UnsignedBigInteger value)
{
    if (value.is_invalid()) {
        stream << "Invalid BigInt";
        return stream;
    }
    for (int i = value.length() - 1; i >= 0; --i) {
        stream << value.words()[i] << "|";
    }
    return stream;
}