/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

/*! 
 *  \link
 *  \file config.hpp
 *  \ingroup shared_memory_fastflow
 *
 *  \brief This file contains general settings for the FastFlow framework.
 */

#ifndef _FF_CONFIG_HPP_
#define _FF_CONFIG_HPP_
/* ***************************************************************************
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License version 3 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************
 */

#include <limits.h>

#if !defined(CACHE_LINE_SIZE)
#define CACHE_LINE_SIZE 64
#endif

/*
 * NOTE: by default FF_BOUNDED_BUFFER is not defined
 * because the uSWSR_Ptr_Buffer may act as a bounded queue.
 */
#if defined(FF_BOUNDED_BUFFER)
#define FFBUFFER SWSR_Ptr_Buffer
#else  // unbounded buffer
#define FFBUFFER uSWSR_Ptr_Buffer
#endif

namespace ff {
     /*!
     *  \ingroup shared_memory_fastflow
     *
     *  @{
     */
    enum { FF_EOS=ULONG_MAX, FF_EOS_NOFREEZE=(FF_EOS-0x1) , FF_GO_ON=(FF_EOS-0x2)};
}

#define GO_ON         (void*)ff::FF_GO_ON
#define EOS_NOFREEZE  (void*)ff::FF_EOS_NOFREEZE
#define EOS           (void*)ff::FF_EOS

#if defined(TRACE_FASTFLOW)
#define FFTRACE(x) x
#else
#define FFTRACE(x)
#endif

// the barrier implementation to use
#if !defined(BARRIER_T)
#define BARRIER_T             spinBarrier
#endif

// maximum number of threads that can be spawned
#if !defined(MAX_NUM_THREADS)
#define MAX_NUM_THREADS       128 
#endif

// NOTE: BACKOFF_MIN/MAX are lower and upper bound backoff values.
// Notice that backoff bounds are highly dependent from the system and 
// from the concurrency levels. This values should be carefully tuned
// in order to achieve the maximum performance.

#if !defined(BACKOFF_MIN)
#define BACKOFF_MIN 128
#endif
#if !defined(BACKOFF_MAX)
#define BACKOFF_MAX 1024
#endif

// TODO:
//#if defined(NO_CMAKE_CONFIG)

// TODO change to __GNUC__ that is portable. GNUC specific code currently works
// on linux only
#if defined(__USE_GNU) //linux
//#if defined(__GNUC__) 
#define HAVE_PTHREAD_SETAFFINITY_NP 1
//#warning "Is GNU compiler"
#endif 

#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#define MAC_OS_X_HAS_AFFINITY (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && (__MAC_OS_X_VERSION_MIN_REQUIRED >= 1050))
//defined (MAC_OS_X_VERSION_10_5) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
#endif

//#else
// the config.h file will be generated by cmake
//#include <ff/config.h>
//#endif // NO_CMAKE_CONFIG

/*!
 *  @}
 *  \endlink
 */

#endif /* _FF_CONFIG_HPP_ */
