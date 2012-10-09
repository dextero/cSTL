
#define _TEST(value, check, expected, line) if (!((value) check (expected))) return printf("line %d: test (%s %s %s) failed (value = %d, expected = %d)!\n", line, #value, #check, #expected, (const int)value, (const int)expected), line

#define TEST_EQUAL(value, expected)         _TEST(value, ==, expected, __LINE__)
#define TEST_NOT_EQUAL(value, expected)     _TEST(value, !=, expected, __LINE__)
#define TEST_LESS(value, expected)          _TEST(value, <, expected, __LINE__)
#define TEST_LESS_EQUAL(value, expected)    _TEST(value, <=, expected, __LINE__)
#define TEST_GREATER(value, expected)       _TEST(value, >, expected, __LINE__)
#define TEST_GREATER_EQUAL(value, expected) _TEST(value, >=, expected, __LINE__)
#define TEST_BETWEEN(value, min, max)       TEST_GREATER(value, min); TEST_LESS(value, max)

