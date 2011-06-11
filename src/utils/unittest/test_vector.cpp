#include <utils/vector.h>
#include <gtest/gtest.h>

#include <string>

using namespace p;

TEST(utils_vec, mul) {
  vector3<float> v(1.0f, 2.0f, 3.0f);
  v *= 3.0f;

  EXPECT_FLOAT_EQ(3.0f, v.x);
  EXPECT_FLOAT_EQ(6.0f, v.y);
  EXPECT_FLOAT_EQ(9.0f, v.z);
}

// TODO: +, -, /, *, conversions between colors, normalization, cross product
//       scalar product, lerp, ...

TEST(utils_vec, string) {
  vector3<float> v(3.0f, 2.0f, 1.5f);
  EXPECT_STREQ(std::string(v).c_str(), "(3, 2, 1.5)");  
}

TEST(utils_vec, minmax) {
  vector3<float> v1(1.0f, -3.0f, 4.0f);
  vector3<float> v2(3.0f, 1.0f, -6.0f);
  
  const vector3<float>& v3 = min(v1, v2);
  EXPECT_FLOAT_EQ(1.0f, v3.x);
  EXPECT_FLOAT_EQ(-3.0f, v3.y);
  EXPECT_FLOAT_EQ(-6.0f, v3.z);
}

TEST(utils_color, trunc) {
  color4<float> v(10.0f, 20.0f, 30.0f, 40.0f);
  color3<float>& v_trunc = v.truncated();
  
  // read
  EXPECT_FLOAT_EQ(10.0f, v_trunc.r);
  EXPECT_FLOAT_EQ(20.0f, v_trunc.g);
  EXPECT_FLOAT_EQ(30.0f, v_trunc.b);
  
  // mutate
  v_trunc.r = 50.0f;
  v_trunc.g = 60.0f;
  v_trunc.b = 70.0f;
  
  EXPECT_FLOAT_EQ(50.0f, v.r);
  EXPECT_FLOAT_EQ(60.0f, v.g);
  EXPECT_FLOAT_EQ(70.0f, v.b);
  EXPECT_FLOAT_EQ(40.0f, v.a);
}

