#pragma once

#include <cassert>
#include <utility>

namespace tinystl {

template <class Roolback>
class exception_guard_exceptions {
public:
  exception_guard_exceptions() = delete;

  explicit exception_guard_exceptions(Roolback roolbaclk)
    : m_roolback(std::move(roolbaclk)), m_complete(false) {}

  exception_guard_exceptions(exception_guard_exceptions &&other) noexcept
    : m_roolback(std::move(other.m_roolback)), m_complete(other.m_complete) {
    other.m_complete = true;
  }
  exception_guard_exceptions(const exception_guard_exceptions &) = delete;
  exception_guard_exceptions &
  operator=(const exception_guard_exceptions &) = delete;
  exception_guard_exceptions &
  operator=(exception_guard_exceptions &&other) = delete;

  ~exception_guard_exceptions() noexcept(false) {
    if (!m_complete) {
      m_roolback();
    }
  }

  void complete() noexcept { m_complete = true; }

private:
  Roolback m_roolback;
  bool m_complete;
};

template <class Roolback>
class exception_guard_nonexceptions {
public:
exception_guard_nonexceptions() = delete;

  explicit exception_guard_nonexceptions(Roolback)
    : m_complete(false) {}

  exception_guard_nonexceptions(exception_guard_nonexceptions &&other) noexcept
    : m_complete(other.m_complete) {
    other.m_complete = true;
  }

  exception_guard_nonexceptions(const exception_guard_nonexceptions &) = delete;
  exception_guard_nonexceptions &
  operator=(const exception_guard_nonexceptions &) = delete;
  exception_guard_nonexceptions &
  operator=(exception_guard_nonexceptions &&other) = delete;

  ~exception_guard_nonexceptions() noexcept {
    assert(!m_complete && "exception_guard not complete with exceptions disabled");
  }

  void complete() noexcept { m_complete = true; }

private:
  bool m_complete;
};

template <class Roolback>
using exception_guard = exception_guard_exceptions<Roolback>;

template <class Roolback>
exception_guard<Roolback> make_exception_guard(Roolback roolback) {
  return exception_guard<Roolback>(std::move(roolback));
}

} // namespace tinystl