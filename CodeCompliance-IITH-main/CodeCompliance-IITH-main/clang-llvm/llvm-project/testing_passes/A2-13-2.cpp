
// Non-Compliant as the encoding is different.

wchar_t mixed1[] = u"Hello" L"World";


// Compliant as both encoding are same.
char16_t nArray[] = u"Hello" u"World";


// Non-Compliant as the encoding is different for three literals.
wchar_t mixed2[] = u"Hello" u"Hello" L"World";
