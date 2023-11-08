#include <efl/Assert.hpp>

int main() {
    EflAssert(true, "Message!");
#   if !EFL_ASSERT_FALLBACK
    EflDynAssert(false, "Message");
#   endif
}