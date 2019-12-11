// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>
#include "header.hpp"
#include <iostream>
#include <string>
#include <atomic>
#include <map>

TEST(Example, EmptyTest) {
int a = 7;
int b = 8;

SharedPtr<int> s1(&a);
EXPECT_EQ(s1.use_count(), 1);
EXPECT_EQ(s1.get(), &a);

SharedPtr<int> s2(&a);
SharedPtr<int> s3(&b);

EXPECT_EQ(*s1, a);
s1.Pswap(s2);
EXPECT_EQ(s1, s2);

s1.reset();
EXPECT_EQ(s1.use_count(), 0);

s1 = s2;
EXPECT_EQ(s1, s2);

s1.reset(&b);
EXPECT_EQ(s1.use_count(), s3.use_count());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
