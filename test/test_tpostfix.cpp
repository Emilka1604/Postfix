#include "postfix.h"
#include <gtest.h>

TEST(translyator, no_throws_with_correct_string) {
	translyator trans("(2.4+3.89)*5");
	ASSERT_NO_THROW(trans.check());
}
TEST(translyator, throws_with_string_with_not_correct_symbol) {
	translyator trans("2.9&3.67");
	ASSERT_ANY_THROW(trans.check());
}
TEST(translyator, throws_thith_not_correct_string) {
	translyator trans("2+(23*6.09)7");
	ASSERT_ANY_THROW(trans.check());
}
TEST(translyator, throw_with_pz_without_check) {
	translyator trans("2+78");
	ASSERT_ANY_THROW(trans.pz());
}
TEST(translyator, correct_pz_with_check) {
	translyator trans("2+78");
	trans.check();
	ASSERT_NO_THROW(trans.pz());
}
TEST(translyator, throw_with_result_without_check_and_pz) {
	translyator trans("2+78");
	ASSERT_ANY_THROW(trans.result());
}
TEST(translyator, throw_with_result_without_pz){
	translyator trans("2+78");
	trans.check();
	ASSERT_ANY_THROW(trans.result());
	}
TEST(translyator, no_throw_with_result_correct_string_with_pz_and_check) {
	translyator trans("2.5+78.5");
	trans.check();
	trans.pz();
	EXPECT_TRUE(81 == trans.result());
}