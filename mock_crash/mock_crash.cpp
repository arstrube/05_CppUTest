#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#define DEFINE_THIS_MODULE(name_) static char const l_this_file[] = name_;
#define X_ASSERT_ID(id_, test_) ((test_) ? (void)0 : X_onAssert(&l_this_file[0], (int)(id_)))

DEFINE_THIS_MODULE("MockCrash");

extern "C" {
    #include "xassert.h"
}

TEST_GROUP(MockCrash) {
};

TEST(MockCrash, Crash) {
    mock().expectOneCall("X_onAssert")
          .withParameter("file", "MockCrash")
          .withParameter("line", 3);
    X_ASSERT_ID(1, 0);
    mock().checkExpectations();
    mock().clear();
}

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}

