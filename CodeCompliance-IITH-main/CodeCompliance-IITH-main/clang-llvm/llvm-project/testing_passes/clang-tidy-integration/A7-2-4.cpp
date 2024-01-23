#include <cstdint>
enum class Enum1 : std::uint32_t
{
    One,
    Two = 2, // Non-compliant
    Three
};

enum class Enum2 : std::uint32_t // Compliant (none)
{
   One,
   Two,
   Three
};

enum class Enum3 : std::uint32_t // Compliant (the first)
{
   One = 1,
   Two,
   Three
};

enum class Enum4 : std::uint32_t // Compliant (all)
{
    One = 1,
    Two = 2,
    Three = 3
};