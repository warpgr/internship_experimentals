#pragma once

#ifdef USING_PLATFORM_WIN_

#include <fiber/windows_context_impl.hpp>
namespace il  { namespace fiber { namespace impl {
using context_impl = windows_context_impl;
}}}
#endif
#ifdef USING_PLATFORM_LINUX_

#include <fiber/posix_context_impl.hpp>
namespace il { namespace fiber { namespace impl  {
using context_impl = posix_context_impl;
}}}
#endif
