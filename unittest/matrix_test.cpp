#include "matrix.h"
#include "vector.h"
#include <gtest/gtest.h>

TEST(matrix, ctor) {
  {
	p::mat<float, 1, 1> m(0.0f);
	EXPECT_FLOAT_EQ(m.components[0], 0.0f);
  }

  {
	p::mat3 m(1.0f);
	for (std::size_t i = 0; i < 3; ++i)
	  for (std::size_t j = 0; j < 3; ++j)
		EXPECT_FLOAT_EQ(m.components[i*3+j], 1.0f);
  }
  
}

TEST(matrix, access) {
  p::mat3 m;
  m.components[0] = 1.0f;
  m.components[1] = 2.0f;
  m.components[2] = 3.0f;
  m.components[3] = 4.0f;
  m.components[4] = 5.0f;
  m.components[5] = 6.0f;
  m.components[6] = 7.0f;
  m.components[7] = 8.0f;
  m.components[8] = 9.0f;

  const p::vec3 &r1 = m.row(0);
  const p::vec3 &r3 = m.row(2);

  EXPECT_FLOAT_EQ(r1.x, 1.0f);
  EXPECT_FLOAT_EQ(r1.y, 2.0f);
  EXPECT_FLOAT_EQ(r1.z, 3.0f);
  EXPECT_FLOAT_EQ(r3.x, 7.0f);
  EXPECT_FLOAT_EQ(r3.y, 8.0f);
  EXPECT_FLOAT_EQ(r3.z, 9.0f);

  m.row(1).y = 300.0f;
  EXPECT_FLOAT_EQ(m.components[3], 4.0f);
  EXPECT_FLOAT_EQ(m.components[4], 300.0f);
  EXPECT_FLOAT_EQ(m.components[5], 6.0f);

  m.row(2).z = 100.0f;
  EXPECT_FLOAT_EQ(m.components[8], 100.0f);
}

  
