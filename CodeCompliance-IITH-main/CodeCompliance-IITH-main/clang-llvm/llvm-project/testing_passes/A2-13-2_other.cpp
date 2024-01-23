

/*
    Both cases are non complaint as they have different encoding.
    Here the speciality is that we have added string literal without any encoding in between.
*/

wchar_t mixed3[] = u"Hello" u"Hello" ";"
L"World";


wchar_t mixed3[] = u"Hello" L"Hello" ";"
L"World";
