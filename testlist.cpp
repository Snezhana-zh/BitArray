#include <gtest/gtest.h>
#include "../bit_array.h"

TEST(construct, test_1)
{
    bit_array ar;

    ASSERT_TRUE(ar.empty());
}

TEST(construct, test_2)
{
    bit_array ar (6);

    ASSERT_FALSE(ar.empty());
    ASSERT_TRUE(ar.size() == 6);
    ASSERT_TRUE(ar.count() == 0);
}

TEST(construct, test_3)
{
    bit_array ar(67, 20);

    ASSERT_FALSE(ar.empty());
    ASSERT_TRUE(ar.size() == 67);
    ASSERT_TRUE(ar.count() == 2);
}

TEST(construct, test_4)
{
    bit_array ar1 (23, 23);
    bit_array ar2 (ar1);

    ASSERT_FALSE(ar2.empty());
    ASSERT_TRUE(ar2.size() == 23);
    ASSERT_TRUE(ar2.count() == 0);
}

TEST(construct, test_5)
{
    bit_array ar2 (147, 23);
    bit_array ar3 (ar2);

    ASSERT_FALSE(ar3.empty());
    ASSERT_TRUE(ar3.size() == 147);
}

TEST(construct, test_6)
{
    EXPECT_DEATH({
        bit_array ar (-1);
    },"");
}

TEST(resize, test_1)
{
    bit_array ar (123, 34);

    ar.resize(129, true);

    ASSERT_EQ(129, ar.size());
    ASSERT_EQ(8, ar.count());
}

TEST(resize, test_2)
{
    bit_array ar (129, 34);

    ar.resize(127);

    ASSERT_EQ(127, ar.size());
    ASSERT_EQ(2, ar.count());
}

TEST(resize, test_3)
{
    bit_array ar (156);

    ar.resize(345);

    ASSERT_EQ(345, ar.size());
    ASSERT_EQ(0, ar.count());
}

TEST(resize, test_4)
{
    bit_array ar (156);
    
    EXPECT_DEATH({
        ar.resize(-20);
    }, "");
}

TEST(clear, test_1)
{
    bit_array ar (943, UINT64_MAX);

    ar.clear();

    ASSERT_EQ(0, ar.size());
}

TEST(push_back, test_1)
{
    bit_array ar;

    ar.push_back(1);
    ar.push_back(0);

    ASSERT_EQ(2, ar.size());
    ASSERT_EQ(1, ar[0]);
    ASSERT_EQ(0, ar[1]);
}

TEST(push_back, test_2)
{
    bit_array ar (63, UINT64_MAX);

    ar.push_back(0);
    ar.push_back(1);

    ASSERT_EQ(64, ar.count());
    ASSERT_EQ(65, ar.size());
    ASSERT_EQ(0, ar[63]);
    ASSERT_FALSE(ar.empty());
}

TEST(push_back, test_3)
{
    bit_array ar (943, UINT64_MAX << 23);
    
    for (int i = 0; i < 23; i++)
    {
        ar.push_back(1);
    }

    ASSERT_EQ(966, ar.size());
    ASSERT_EQ(64, ar.count());
    ASSERT_EQ(0, ar[100]);
}

TEST(push_back, test_4)
{
    bit_array ar (32, UINT64_MAX >> 10);

    ar.resize(65, true);
    ar.push_back(0);

    ASSERT_EQ(55, ar.count());
    ASSERT_EQ(66, ar.size());
}

TEST(set, test_1)
{
    bit_array ar (78, UINT64_MAX << 10);

    ar.set(60);

    ASSERT_EQ(ar[60], 1);
}

TEST(set, test_2)
{
    bit_array ar (78, UINT64_MAX << 10);

    ar.set(0, 0);

    ASSERT_EQ(ar[0], 0);
}

TEST(set, test_3)
{
    bit_array ar (65, UINT64_MAX << 10);

    ar.set(64);

    ASSERT_EQ(ar[64], 1);
}

TEST(set, test_4)
{
    bit_array ar (848, UINT64_MAX << 10);

    ar.set();

    ASSERT_EQ(ar.count(), 848);
}

TEST(set, test_5)
{
    bit_array ar (848, UINT64_MAX << 10);

    EXPECT_DEATH({
        ar.set(848);
    }, "");
}

TEST(set, test_6)
{
    bit_array ar (848, UINT64_MAX << 10);

    EXPECT_DEATH({
        ar.set(-1, -1);
    }, "");
}

TEST(reset, test_1)
{
    bit_array ar (100000, 2);

    ar.reset(62);

    ASSERT_EQ(ar.count(), 0);
}

TEST(reset, test_2)
{
    bit_array ar;

    ar.push_back(1);
    ar.reset();
    string str = "0";

    ASSERT_EQ(0, ar.count());
    ASSERT_EQ(1, ar.size());
    ASSERT_EQ(str, ar.to_string());
}

TEST(reset, test_3)
{
    bit_array ar (87, UINT64_MAX << 10);

    EXPECT_DEATH({
        ar.reset(-4);
    }, "");
}

TEST(reset, test_4)
{
    bit_array ar (87, UINT64_MAX << 10);

    EXPECT_DEATH({
        ar.reset(87);
    }, "");
}

TEST(any, test_1)
{
    bit_array ar (6, UINT64_MAX);
    string str = "111111";

    ASSERT_EQ(true, ar.any());
    ASSERT_EQ(str, ar.to_string());
}

TEST(any, test_2)
{
    bit_array ar (1000000);
    string str = "";

    for (int i = 0; i < 1000000; i++)
    {
        str += '0';
    }

    ASSERT_EQ(false, ar.any());
    ASSERT_EQ(str, ar.to_string());
}

TEST(none, test_1)
{
    bit_array ar;
    string str = "1111111111";

    for (int i = 0; i < 10; i++)
    {
        ar.push_back(1);
    }

    ASSERT_EQ(false, ar.none());
    ASSERT_EQ(str, ar.to_string());
}

TEST(count, test_1)
{
    bit_array ar (64, 1);
    string str = "";

    for (int i = 0; i < 63; i++)
    {
        str += '0';
    }
    str += '1';

    ASSERT_EQ(1, ar.count());
    ASSERT_EQ(str, ar.to_string());
    ASSERT_EQ(64, ar.size());
}

TEST(size, test_1)
{
    bit_array ar1 (999, 23);
    bit_array ar2 (ar1);

    ar2.push_back(1);

    ASSERT_EQ(999, ar1.size());
    ASSERT_EQ(1000, ar2.size());
}

TEST(empty, test_1)
{
    bit_array ar (25, UINT64_MAX >> 24);

    ASSERT_EQ(false, ar.empty());
}

TEST(swap, test_1)
{
    bit_array ar1;
    bit_array ar2 (65, 23);

    ar1.swap(ar2);

    ASSERT_EQ(0, ar2.size());
    ASSERT_EQ("", ar2.to_string());
    ASSERT_EQ(65, ar1.size());
    ASSERT_EQ(4, ar1.count());
}

TEST(equal_operator, test_1)
{
    bit_array ar1;
    bit_array ar2 (788, UINT64_MAX);

    ar1 = ar2;

    ASSERT_EQ(ar2.to_string(), ar1.to_string());
    ASSERT_EQ(ar2.size(), ar1.size());
}

TEST(byte_and_operator, test_1)
{
    bit_array ar1 (16, UINT64_MAX);
    bit_array ar2 (16, UINT64_MAX >> 2);

    ar1 &= ar2;

    ASSERT_EQ(ar1.to_string(), ar2.to_string());
}

TEST(byte_or_operator, test_1)
{
    bit_array ar1 (63, UINT64_MAX);
    bit_array ar2 (63, UINT64_MAX >> 2);

    ar1 |= ar2;

    ASSERT_EQ(63, ar1.count());
}

TEST(byte_xor_operator, test_1)
{
    bit_array ar1 (63, UINT64_MAX);
    bit_array ar2 (63, UINT64_MAX >> 2);

    ar1 ^= ar2;

    ASSERT_EQ(2, ar1.count());
}

TEST(invert_operator, test_1)
{
    bit_array ar (127, UINT64_MAX);

    ar = ~ar;

    ASSERT_EQ(63, ar.count());
}

TEST(left_shift_operator, test_1)
{
    bit_array ar (64, UINT64_MAX);

    for (int i = 0; i < 100; i++)
    {
        ar.push_back(1);
    }
    ar <<= 4;

    ASSERT_EQ(160, ar.count());
}

TEST(left_shift_operator, test_2)
{
    bit_array ar (64, UINT64_MAX);

    for (int i = 0; i < 100; i++)
    {
        ar.push_back(1);
    }
    ar = ar << 4;

    ASSERT_EQ(160, ar.count());
}

TEST(right_shift_operator, test_1)
{
    bit_array ar (64, UINT64_MAX);

    for (int i = 0; i < 100; i++)
    {
        ar.push_back(1);
    }
    ar >>= 4;

    ASSERT_EQ(160, ar.count());
}

TEST(right_shift_operator, test_2)
{
    bit_array ar (64, UINT64_MAX);

    for (int i = 0; i < 100; i++)
    {
        ar.push_back(1);
    }
    ar = ar >> 4;

    ASSERT_EQ(160, ar.count());
}

TEST(equally, test_1)
{
    bit_array ar1 (123, UINT64_MAX);
    bit_array ar2 (ar1);

    ASSERT_TRUE(ar1 == ar2);
}

TEST(not_equally, test_1)
{
    bit_array ar1 (123, UINT64_MAX);
    bit_array ar2 (ar1);

    ASSERT_FALSE(ar1 != ar2);
}

TEST(byte_and_operator, test_2)
{
    bit_array ar1 (978, UINT64_MAX);
    bit_array ar2 (978, UINT64_MAX >> 2);

    ar1 = ar1 & ar2;

    ASSERT_EQ(62, ar1.count());
}

TEST(byte_or_operator, test_2)
{
    bit_array ar1 (237, UINT64_MAX);
    bit_array ar2 (237, UINT64_MAX >> 2);

    ar1 = ar1 | ar2;

    ASSERT_EQ(64, ar1.count());
}

TEST(byte_xor_operator, test_2)
{
    bit_array ar1 (237, UINT64_MAX);
    bit_array ar2 (237);

    ar1 = ar1 ^ ar2;

    ASSERT_EQ(64, ar1.count());
}

TEST(index_operator, test_1)
{
    bit_array ar (22);

    EXPECT_DEATH({
        ar[22];
    }, "");
}

TEST(index_operator, test_2)
{
    bit_array ar (22);

    EXPECT_DEATH({
        ar[-1];
    }, "");
}

TEST(byte_and_operator, test_3)
{
    bit_array ar1 (128);
    bit_array ar2 (127);

    EXPECT_DEATH({
        ar1 &= ar2;
    }, "");
}

TEST(byte_and_operator, test_4)
{
    bit_array ar1 (56);
    bit_array ar2 (127);

    EXPECT_DEATH({
        ar1 = ar1 & ar2;
    }, "");
}

TEST(byte_or_operator, test_3)
{
    bit_array ar1 (738, 34);
    bit_array ar2 (127);

    EXPECT_DEATH({
        ar1 |= ar2;
    }, "");
}

TEST(byte_or_operator, test_4)
{
    bit_array ar1 (56);
    bit_array ar2 (57);

    EXPECT_DEATH({
        ar1 = ar1 | ar2;
    }, "");
}

TEST(byte_xor_operator, test_3)
{
    bit_array ar1 (7, 34);
    bit_array ar2 (12);

    EXPECT_DEATH({
        ar1 ^= ar2;
    }, "");
}

TEST(byte_xor_operator, test_4)
{
    bit_array ar1 (56);
    bit_array ar2 (5);

    EXPECT_DEATH({
        ar1 = ar1 ^ ar2;
    }, "");
}
