#define EFLI_ASSERT_USE_CODEGENNED_ 1
#include <efl/Assert.hpp>
#include <string>

int main() {
  volatile bool b = true;
  std::string s = "b cannot be ";

  EflAssert(b, "Message!");
  EflDynAssert(!b, s, !b, '!');
}