#include <cstdint>

namespace NS1 {
class G {};
void G() {} // non-compliant, hides class G
} // namespace NS1

namespace NS2 {
enum class H {
  VALUE = 0,
};
std::uint8_t H = 17; // non-compliant, hides scoped enum H
} // namespace NS2

namespace NS3 {
class J {};
enum H {
  J = 0, // non-compliant, hides class J
};
} // namespace NS3





int main(void) {
  class J {};
  int J = 0; // non-compliant
}