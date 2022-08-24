#pragma once

#ifdef USING_PLATFORM_WIN_

#include <fiber/windows_fiber_impl.hpp>
namespace il  { namespace fiber { namespace impl {
using fiber_impl = windows_fiber_impl;
}}}
#endif
#ifdef USING_PLATFORM_LINUX_

#include <fiber/posix_fiber_impl.hpp>
namespace il { namespace fiber { namespace impl  {
using fiber_impl = posix_fiber_impl;
}}}
#endif