#include <biscuit/assert.hpp>
#include <biscuit/code_buffer.hpp>

#include <cstring>
#include <utility>

namespace biscuit {

CodeBuffer::CodeBuffer(size_t capacity)
    : m_capacity{capacity}, m_is_managed{true} {
    if (capacity == 0) {
        return;
    }

    m_buffer = new uint8_t[capacity]();
    m_cursor = m_buffer;
}

CodeBuffer::CodeBuffer(uint8_t* buffer, size_t capacity)
    : m_buffer{buffer}, m_cursor{buffer}, m_capacity{capacity} {
    BISCUIT_ASSERT(buffer != nullptr);
}

CodeBuffer::CodeBuffer(CodeBuffer&& other) noexcept
    : m_buffer{std::exchange(other.m_buffer, nullptr)}
    , m_cursor{std::exchange(other.m_cursor, nullptr)}
    , m_capacity{std::exchange(other.m_capacity, size_t{0})}
    , m_is_managed{std::exchange(other.m_is_managed, false)} {}

CodeBuffer& CodeBuffer::operator=(CodeBuffer&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    m_buffer = std::exchange(other.m_buffer, nullptr);
    m_cursor = std::exchange(other.m_cursor, nullptr);
    m_capacity = std::exchange(other.m_capacity, size_t{0});
    m_is_managed = std::exchange(other.m_is_managed, false);
    return *this;
}

CodeBuffer::~CodeBuffer() noexcept {
    if (!m_is_managed) {
        return;
    }

    delete[] m_buffer;
}

void CodeBuffer::Grow(size_t new_capacity) {
    BISCUIT_ASSERT(IsManaged());

    // No-op, just return.
    if (new_capacity <= m_capacity) {
        return;
    }

    const auto cursor_offset = GetCursorOffset();
    auto* new_buffer = new uint8_t[new_capacity]();
    std::memcpy(new_buffer, m_buffer, m_capacity);
    delete[] m_buffer;

    m_buffer = new_buffer;
    m_capacity = new_capacity;
    m_cursor = m_buffer + cursor_offset;
}

} // namespace biscuit
