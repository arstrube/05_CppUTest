#include "xassert.h"
#include "CppUTestExt/MockSupport_c.h"

void X_onAssert(char const * const file, int line) {
    mock_c()->actualCall("X_onAssert")
            ->withStringParameters("file", file)
            ->withIntParameters("line", line);
}
