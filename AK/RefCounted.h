/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
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

#include <AK/Assertions.h>
#include <AK/Platform.h>
#include <AK/StdLibExtras.h>

namespace AK {

template<class T>
constexpr auto call_will_be_destroyed_if_present(const T* object) -> decltype(object->will_be_destroyed(), TrueType {})
{
    const_cast<T*>(object)->will_be_destroyed();
    return {};
}

constexpr auto call_will_be_destroyed_if_present(...) -> FalseType
{
    return {};
}

template<class T>
constexpr auto call_one_ref_left_if_present(const T* object) -> decltype(object->one_ref_left(), TrueType {})
{
    const_cast<T*>(object)->one_ref_left();
    return {};
}

constexpr auto call_one_ref_left_if_present(...) -> FalseType
{
    return {};
}

class RefCountedBase {
public:
    ALWAYS_INLINE void ref() const
    {
        ASSERT(m_ref_count);
        ++m_ref_count;
    }

    ALWAYS_INLINE int ref_count() const
    {
        return m_ref_count;
    }

protected:
    RefCountedBase() {}
    ALWAYS_INLINE ~RefCountedBase()
    {
        ASSERT(!m_ref_count);
    }

    ALWAYS_INLINE void deref_base() const
    {
        ASSERT(m_ref_count);
        --m_ref_count;
    }

    mutable int m_ref_count { 1 };
};

template<typename T>
class RefCounted : public RefCountedBase {
public:
    void unref() const
    {
        deref_base();
        if (m_ref_count == 0) {
            call_will_be_destroyed_if_present(static_cast<const T*>(this));
            delete static_cast<const T*>(this);
        } else if (m_ref_count == 1) {
            call_one_ref_left_if_present(static_cast<const T*>(this));
        }
    }
};

}

using AK::RefCounted;
