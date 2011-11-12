#include "vector.h"
#include "vector_stream.h"
#include "algorithm.h"
#include <gtest/gtest.h>

#include <string>
#include <sstream>

using namespace p;

#pragma mark - Operators
TEST(utils_vector, mul) {
  vec3 v(1.0f, 2.0f, 3.0f);  
  v *= 3.0f;
  v = v * 2.0f;
  
  EXPECT_FLOAT_EQ(6.0f, v.x);
  EXPECT_FLOAT_EQ(12.0f, v.y);
  EXPECT_FLOAT_EQ(18.0f, v.z);
}

TEST(utils_vector, negation) {
  vec3 v(1.0f, 2.0f, 3.0f);
  vec3 v2 = -v;
  
  EXPECT_FLOAT_EQ(-1.0f, v2.x);
  EXPECT_FLOAT_EQ(-2.0f, v2.y);
  EXPECT_FLOAT_EQ(-3.0f, v2.z);
}

TEST(utils_vector, div) {
  vec3 v(1.0f, 2.0f, 3.0f);  
  v /= 3.0f;
  v = v / 2.0f;
  
  EXPECT_FLOAT_EQ(1.0f/3.0f/2.0f, v.x);
  EXPECT_FLOAT_EQ(2.0f/3.0f/2.0f, v.y);
  EXPECT_FLOAT_EQ(0.5f, v.z);
}

TEST(utils_vector, add) {
  vec3 v(1.0f, 2.0f, 3.0f);  
  v += vec3(3.0f, 4.0f, 5.0f);
  v = v + vec3(2.0f, 2.1f, 2.2f);
  
  EXPECT_FLOAT_EQ(6.0f, v.x);
  EXPECT_FLOAT_EQ(8.1f, v.y);
  EXPECT_FLOAT_EQ(10.2f, v.z);
}


#pragma mark - Algorithms
TEST(utils_vector, minmax) {
  using std::min;
  vec3 v1(1.0f, -3.0f, 4.0f);
  vec3 v2(3.0f, 1.0f, -6.0f);
  
  {
    const vec3& v3 = min(v1, v2);
    EXPECT_FLOAT_EQ(1.0f, v3.x);
    EXPECT_FLOAT_EQ(-3.0f, v3.y);
    EXPECT_FLOAT_EQ(-6.0f, v3.z);
  }
  
  {
    const vec3& v3 = max(v1, v2);
    EXPECT_FLOAT_EQ(3.0f, v3.x);
    EXPECT_FLOAT_EQ(1.0f, v3.y);
    EXPECT_FLOAT_EQ(4.0f, v3.z);
  }
}

TEST(utils_vector, lerp) {
  using p::lerp;
  
  {
    vec3 v1(0.0f, 0.0f, 0.0f);
    vec3 v2(10.0f, 20.0f, 30.0f);
    
    const vec3& v = lerp(v1, v2, 0.5f);
    EXPECT_FLOAT_EQ( 5.0f, v.x);
    EXPECT_FLOAT_EQ(10.0f, v.y);
    EXPECT_FLOAT_EQ(15.0f, v.z);
    
  }
  
}

// TODO: benchmark

TEST(utils_vector, dotproduct) {
  using p::dot_product;

  {
    vec2 v1(1.0f, 2.0f);
    vec2 v2(4.5f, 5.2f);
    
    vec2::value_type dp = dot_product(v1, v2);
    EXPECT_FLOAT_EQ(14.9f, dp);
  
  }

  {
    vec3 v1(1.0f, 2.0f, 3.0f);
    vec3 v2(4.5f, 5.2f, 6.2f);
    
    vec3::value_type dp = dot_product(v1, v2);
    EXPECT_FLOAT_EQ(33.5f, dp);
  }
}

TEST(utils_vector, normalize) {
  using p::normalize;
  
  {
    vec2 v1(0.0f, 2.0f);
    vec2 n = normalize(v1);
    EXPECT_FLOAT_EQ(0.0f, n.x);
    EXPECT_FLOAT_EQ(1.0f, n.y);
    // TODO: some more tests for vec2
  }
  
  // TODO: for vec3 also
}

// TODO: normalized, magnitude

#pragma mark - Conversions
TEST(utils_vector, string) {
  vec3 v(3.0f, 2.0f, 1.5f);
  std::stringstream ss;
  ss << v;
  EXPECT_STREQ(ss.str().c_str(), "3 2 1.5");  
}

TEST(utils_vector, parse) {
  {
    std::stringstream ss("123 -23.3");
    vec3 v;
    EXPECT_TRUE(ss >> v);
    EXPECT_FLOAT_EQ(123.0f, v.x);
    EXPECT_FLOAT_EQ(-23.3f, v.y);  
  }

  {
    std::stringstream ss("Null");
    vec3 v;
    EXPECT_TRUE(ss >> v);
    EXPECT_FLOAT_EQ(0.0f, v.x);
    EXPECT_FLOAT_EQ(0.0f, v.y);  
  }

  {
    std::stringstream ss("marsvin");
    vec3 v;
    EXPECT_FALSE(ss >> v);
  }
}

TEST(utils_vector, color_parse) {
  {
    std::stringstream ss("red");
    vec3 c;
    EXPECT_TRUE(ss >> color_reader(c));
    EXPECT_FLOAT_EQ(1.0f, c.r);
    EXPECT_FLOAT_EQ(0.0f, c.g);
    EXPECT_FLOAT_EQ(0.0f, c.b);
  }
  
  {
    std::stringstream ss("0xFFAA22");
    vec3 c;
    EXPECT_TRUE(ss >> color_reader(c));
    EXPECT_FLOAT_EQ(1.0f, c.r);
    EXPECT_FLOAT_EQ(0.66666666f, c.g);
    EXPECT_NEAR(0.13333333f, c.b, 0.02f);
  }
  
  {
    std::stringstream ss("0xFF0088");
    ubvec3 c;
    EXPECT_TRUE(ss >> color_reader(c));
    EXPECT_EQ(0xFF, c.r);
    EXPECT_EQ(0x00, c.g);
    EXPECT_EQ(0x88, c.b);
  }
}

TEST(utils_vector, color_print) {
  
  {
    std::stringstream ss;
  }

}

// TODO: performance tests, crossproduct

// TODO: vec2, vec3, vec4 truncate

/*TEST(utils_color, trunc) {
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

TEST(utils_color, cast) {
  fcolor4 c1(1.0f, 0.5f, 0.0f, 1.0f);
  ubcolor4 c2 = c1;
  
  EXPECT_EQ(255, (int)c2.r);
  EXPECT_EQ(127, (int)c2.g);
  EXPECT_EQ(0,   (int)c2.b);
  EXPECT_EQ(255, (int)c2.a);
}
*/
