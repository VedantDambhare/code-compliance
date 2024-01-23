void f (volatile  char_t * p )
{
*const_cast< char_t * >( p ) = '\0';
}
// Non-compliant
int main ( )
{
const int *a=20;
b=(char)*a;
f ( "Hello World!" );
}