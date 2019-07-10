#include <Ark/VM/Frame.hpp>

namespace Ark::internal
{
    Frame::Frame(std::size_t length) :
        m_addr(0), m_page_addr(0),
        m_environment(length),
        m_stack(ARK_MAX_STACK_SIZE), m_i(0)
    {
        for (std::size_t i=0; i < length; ++i)
            m_environment[i] = FFI::nil;
    }

    Frame::Frame(std::size_t length, std::size_t caller_addr, std::size_t caller_page_addr) :
        m_addr(caller_addr), m_page_addr(caller_page_addr),
        m_environment(length),
        m_stack(ARK_MAX_STACK_SIZE), m_i(0)
    {
        for (std::size_t i=0; i < length; ++i)
            m_environment[i] = FFI::nil;
    }

    std::size_t Frame::stackSize()
    {
        return m_i;
    }

    std::size_t Frame::callerAddr() const
    {
        return m_addr;
    }

    std::size_t Frame::callerPageAddr() const
    {
        return m_page_addr;
    }

    std::ostream& operator<<(std::ostream& os, const Frame& F)
    {
        os << "Frame";
        return os;
    }
}
