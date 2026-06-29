// Stub header for sanitizer interface
// This file provides empty implementations for sanitizer functions
// when the actual sanitizer headers are not available

#ifndef SANITIZER_LSAN_INTERFACE_H
#define SANITIZER_LSAN_INTERFACE_H

// LSan (Leak Sanitizer) interface functions
// These are typically declared in the actual sanitizer headers

namespace __lsan {
void DisableChecksUnderMemoryPressure();
void EnableChecksUnderMemoryPressure();
}  // namespace __lsan

// Macro to mark root of stack that may be scanned by leak sanitizer
#define LSAN_DEFAULT_SUPPRESSIONS_ATTR

#endif  // SANITIZER_LSAN_INTERFACE_H