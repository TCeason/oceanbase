/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef OCEANBASE_SHARE_VECTOR_OB_VECTOR_DEFINE_H_
#define OCEANBASE_SHARE_VECTOR_OB_VECTOR_DEFINE_H_

#include "share/vector/type_traits.h"
namespace oceanbase
{
namespace common
{

using ConstUniformFormat = ObUniformFormat<true>;
using UniformFormat = ObUniformFormat<false>;
#define DEFINE_FIXED_VECTOR(vector_name, vec_tc) \
  using vector_name = ObFixedLengthVector<RTCType<vec_tc>, VectorBasicOp<vec_tc>>;

#define FIXED_VEC_LIST           \
  VEC_TC_INTEGER,                \
  VEC_TC_UINTEGER,               \
  VEC_TC_FLOAT,                  \
  VEC_TC_DOUBLE,                 \
  VEC_TC_FIXED_DOUBLE,           \
  VEC_TC_DATETIME,               \
  VEC_TC_DATE,                   \
  VEC_TC_TIME,                   \
  VEC_TC_YEAR,                   \
  VEC_TC_ENUM_SET,               \
  VEC_TC_UNKNOWN,                \
  VEC_TC_BIT,                    \
  VEC_TC_TIMESTAMP_TZ,           \
  VEC_TC_TIMESTAMP_TINY,         \
  VEC_TC_INTERVAL_YM,            \
  VEC_TC_INTERVAL_DS,            \
  VEC_TC_DEC_INT32,              \
  VEC_TC_DEC_INT64,              \
  VEC_TC_DEC_INT128,             \
  VEC_TC_DEC_INT256,             \
  VEC_TC_DEC_INT512,             \
  VEC_TC_MYSQL_DATETIME,         \
  VEC_TC_MYSQL_DATE

DEFINE_FIXED_VECTOR(IntegerFixedVec, VEC_TC_INTEGER);
DEFINE_FIXED_VECTOR(UIntegerFixedVec, VEC_TC_UINTEGER);
DEFINE_FIXED_VECTOR(FloatFixedVec, VEC_TC_FLOAT);
DEFINE_FIXED_VECTOR(DoubleFixedVec, VEC_TC_DOUBLE);
DEFINE_FIXED_VECTOR(FixedDoubleFixedVec, VEC_TC_FIXED_DOUBLE);
DEFINE_FIXED_VECTOR(DateTimeFixedVec, VEC_TC_DATETIME);
DEFINE_FIXED_VECTOR(DateFixedVec, VEC_TC_DATE);
DEFINE_FIXED_VECTOR(TimeFixedVec, VEC_TC_TIME);
DEFINE_FIXED_VECTOR(YearFixedVec, VEC_TC_YEAR);
DEFINE_FIXED_VECTOR(EnumSetFixedVec, VEC_TC_ENUM_SET);
DEFINE_FIXED_VECTOR(UnknownFixedVec, VEC_TC_UNKNOWN);
DEFINE_FIXED_VECTOR(BitTypeFixedVec, VEC_TC_BIT);
DEFINE_FIXED_VECTOR(TimestampTZFixedVec, VEC_TC_TIMESTAMP_TZ);
DEFINE_FIXED_VECTOR(TimestampTinyFixedVec, VEC_TC_TIMESTAMP_TINY);
DEFINE_FIXED_VECTOR(IntervalYMFixedVec, VEC_TC_INTERVAL_YM);
DEFINE_FIXED_VECTOR(IntervalDSFixedVec, VEC_TC_INTERVAL_DS);
DEFINE_FIXED_VECTOR(DecInt32FixedVec, VEC_TC_DEC_INT32);
DEFINE_FIXED_VECTOR(DecInt64FixedVec, VEC_TC_DEC_INT64);
DEFINE_FIXED_VECTOR(DecInt128FixedVec, VEC_TC_DEC_INT128);
DEFINE_FIXED_VECTOR(DecInt256FixedVec, VEC_TC_DEC_INT256);
DEFINE_FIXED_VECTOR(DecInt512FixedVec, VEC_TC_DEC_INT512);
DEFINE_FIXED_VECTOR(MySQLDateTimeFixedVec, VEC_TC_MYSQL_DATETIME);
DEFINE_FIXED_VECTOR(MySQLDateFixedVec, VEC_TC_MYSQL_DATE);
#undef DEFINE_FIXED_VECTOR

constexpr bool is_fixed_length_vec(const VecValueTypeClass tc)
{
  return (tc == VEC_TC_INTEGER
          || tc == VEC_TC_UINTEGER
          || tc == VEC_TC_FLOAT
          || tc == VEC_TC_DOUBLE
          || tc == VEC_TC_FIXED_DOUBLE
          || tc == VEC_TC_DATETIME
          || tc == VEC_TC_DATE
          || tc == VEC_TC_TIME
          || tc == VEC_TC_YEAR
          || tc == VEC_TC_UNKNOWN
          || tc == VEC_TC_BIT
          || tc == VEC_TC_ENUM_SET
          || tc == VEC_TC_TIMESTAMP_TZ
          || tc == VEC_TC_TIMESTAMP_TINY
          || tc == VEC_TC_INTERVAL_YM
          || tc == VEC_TC_INTERVAL_DS
          || tc == VEC_TC_DEC_INT32
          || tc == VEC_TC_DEC_INT64
          || tc == VEC_TC_DEC_INT128
          || tc == VEC_TC_DEC_INT256
          || tc == VEC_TC_DEC_INT512
          || tc == VEC_TC_MYSQL_DATETIME
          || tc == VEC_TC_MYSQL_DATE);
}

constexpr bool is_decint_vec(const VecValueTypeClass tc)
{
  return tc >= VEC_TC_DEC_INT32 && tc <= VEC_TC_DEC_INT512;
}

#define DEFINE_CONTINUOUS_VECTOR(vector_name, vec_tc) \
  using vector_name = ObContinuousVector<VectorBasicOp<vec_tc>>;

DEFINE_CONTINUOUS_VECTOR(NumberContVec, VEC_TC_NUMBER);
DEFINE_CONTINUOUS_VECTOR(ExtendContVec, VEC_TC_EXTEND);
DEFINE_CONTINUOUS_VECTOR(StrContVec, VEC_TC_STRING);
DEFINE_CONTINUOUS_VECTOR(ESInnerContVec, VEC_TC_ENUM_SET_INNER);
DEFINE_CONTINUOUS_VECTOR(RawContVec, VEC_TC_RAW);
DEFINE_CONTINUOUS_VECTOR(RowidContVec, VEC_TC_ROWID);
DEFINE_CONTINUOUS_VECTOR(LobContVec, VEC_TC_LOB);
DEFINE_CONTINUOUS_VECTOR(JsonContVec, VEC_TC_JSON);
DEFINE_CONTINUOUS_VECTOR(GeoContVec, VEC_TC_GEO);
DEFINE_CONTINUOUS_VECTOR(UdtContVec, VEC_TC_UDT);
DEFINE_CONTINUOUS_VECTOR(CollContVec, VEC_TC_COLLECTION);
DEFINE_CONTINUOUS_VECTOR(RoaringBitmapContVec, VEC_TC_ROARINGBITMAP);
#undef DEFINE_CONTINUOUS_VECTOR

constexpr bool is_continuous_vec(const VecValueTypeClass tc)
{
  return (tc == VEC_TC_NUMBER
         || tc == VEC_TC_EXTEND
         || tc == VEC_TC_STRING
         || tc == VEC_TC_ENUM_SET_INNER
         || tc == VEC_TC_RAW
         || tc == VEC_TC_ROWID
         || tc == VEC_TC_LOB
         || tc == VEC_TC_JSON
         || tc == VEC_TC_GEO
         || tc == VEC_TC_UDT
         || tc == VEC_TC_COLLECTION
         || tc == VEC_TC_ROARINGBITMAP);
}

#define DEFINE_DISCRETE_VECTOR(vector_name, vec_tc) \
  using vector_name = ObDiscreteVector<VectorBasicOp<vec_tc>>;
DEFINE_DISCRETE_VECTOR(NumberDiscVec, VEC_TC_NUMBER);
DEFINE_DISCRETE_VECTOR(ExtendDiscVec, VEC_TC_EXTEND);
DEFINE_DISCRETE_VECTOR(StrDiscVec, VEC_TC_STRING);
DEFINE_DISCRETE_VECTOR(ESInnerDiscVec, VEC_TC_ENUM_SET_INNER);
DEFINE_DISCRETE_VECTOR(RawDiscVec, VEC_TC_RAW);
DEFINE_DISCRETE_VECTOR(RowidDiscVec, VEC_TC_ROWID);
DEFINE_DISCRETE_VECTOR(LobDiscVec, VEC_TC_LOB);
DEFINE_DISCRETE_VECTOR(JsonDiscVec, VEC_TC_JSON);
DEFINE_DISCRETE_VECTOR(GeoDiscVec, VEC_TC_GEO);
DEFINE_DISCRETE_VECTOR(UdtDiscVec, VEC_TC_UDT);
DEFINE_DISCRETE_VECTOR(CollDiscVec, VEC_TC_COLLECTION);
DEFINE_DISCRETE_VECTOR(RoaringBitmapDiscVec, VEC_TC_ROARINGBITMAP);
#undef DEFINE_DISCRETE_VECTOR

constexpr bool is_discrete_vec(const VecValueTypeClass vec_tc)
{
  return (vec_tc ==  VEC_TC_NUMBER
          || vec_tc == VEC_TC_EXTEND
          || vec_tc == VEC_TC_STRING
          || vec_tc == VEC_TC_ENUM_SET_INNER
          || vec_tc == VEC_TC_RAW
          || vec_tc == VEC_TC_ROWID
          || vec_tc == VEC_TC_LOB
          || vec_tc == VEC_TC_JSON
          || vec_tc == VEC_TC_GEO
          || vec_tc == VEC_TC_UDT
          || vec_tc == VEC_TC_COLLECTION
          || vec_tc == VEC_TC_ROARINGBITMAP);
}

#define DEFINE_UNIFORM_VECTOR(vector_name, vec_tc) \
  using vector_name = ObUniformVector<false, VectorBasicOp<vec_tc>>;
DEFINE_UNIFORM_VECTOR(NullUniVec, VEC_TC_NULL);
DEFINE_UNIFORM_VECTOR(IntegerUniVec, VEC_TC_INTEGER);
DEFINE_UNIFORM_VECTOR(UIntegerUniVec, VEC_TC_UINTEGER);
DEFINE_UNIFORM_VECTOR(FloatUniVec, VEC_TC_FLOAT);
DEFINE_UNIFORM_VECTOR(DoubleUniVec, VEC_TC_DOUBLE);
DEFINE_UNIFORM_VECTOR(FixedDoubleUniVec, VEC_TC_FIXED_DOUBLE);
DEFINE_UNIFORM_VECTOR(DateTimeUniVec, VEC_TC_DATETIME);
DEFINE_UNIFORM_VECTOR(DateUniVec, VEC_TC_DATE);
DEFINE_UNIFORM_VECTOR(TimeUniVec, VEC_TC_TIME);
DEFINE_UNIFORM_VECTOR(YearUniVec, VEC_TC_YEAR);
DEFINE_UNIFORM_VECTOR(EnumSetUniVec, VEC_TC_ENUM_SET);
DEFINE_UNIFORM_VECTOR(UnknownUniVec, VEC_TC_UNKNOWN);
DEFINE_UNIFORM_VECTOR(BitTypeUniVec, VEC_TC_BIT);
DEFINE_UNIFORM_VECTOR(TimestampTZUniVec, VEC_TC_TIMESTAMP_TZ);
DEFINE_UNIFORM_VECTOR(TimestampTinyUniVec, VEC_TC_TIMESTAMP_TINY);
DEFINE_UNIFORM_VECTOR(IntervalYMUniVec, VEC_TC_INTERVAL_YM);
DEFINE_UNIFORM_VECTOR(IntervalDSUniVec, VEC_TC_INTERVAL_DS);
DEFINE_UNIFORM_VECTOR(DecInt32UniVec, VEC_TC_DEC_INT32);
DEFINE_UNIFORM_VECTOR(DecInt64UniVec, VEC_TC_DEC_INT64);
DEFINE_UNIFORM_VECTOR(DecInt128UniVec, VEC_TC_DEC_INT128);
DEFINE_UNIFORM_VECTOR(DecInt256UniVec, VEC_TC_DEC_INT256);
DEFINE_UNIFORM_VECTOR(DecInt512UniVec, VEC_TC_DEC_INT512);
DEFINE_UNIFORM_VECTOR(NumberUniVec, VEC_TC_NUMBER);
DEFINE_UNIFORM_VECTOR(ExtendUniVec, VEC_TC_EXTEND);
DEFINE_UNIFORM_VECTOR(StrUniVec, VEC_TC_STRING);
DEFINE_UNIFORM_VECTOR(ESInnerUniVec, VEC_TC_ENUM_SET_INNER);
DEFINE_UNIFORM_VECTOR(RawUniVec, VEC_TC_RAW);
DEFINE_UNIFORM_VECTOR(RowidUniVec, VEC_TC_ROWID);
DEFINE_UNIFORM_VECTOR(LobUniVec, VEC_TC_LOB);
DEFINE_UNIFORM_VECTOR(JsonUniVec, VEC_TC_JSON);
DEFINE_UNIFORM_VECTOR(GeoUniVec, VEC_TC_GEO);
DEFINE_UNIFORM_VECTOR(UdtUniVec, VEC_TC_UDT);
DEFINE_UNIFORM_VECTOR(CollUniVec, VEC_TC_COLLECTION);
DEFINE_UNIFORM_VECTOR(MySQLDateTimeUniVec, VEC_TC_MYSQL_DATETIME);
DEFINE_UNIFORM_VECTOR(MySQLDateUniVec, VEC_TC_MYSQL_DATE);
DEFINE_UNIFORM_VECTOR(RoaringBitmapUniVec, VEC_TC_ROARINGBITMAP);
#undef DEFINE_UNIFORM_VECTOR

constexpr bool is_uniform_vec(const VecValueTypeClass tc)
{
  return true;
}

#define DEFINE_UNIFORM_CONST_VECTOR(vector_name, vec_tc) \
  using vector_name = ObUniformVector<true, VectorBasicOp<vec_tc>>;
DEFINE_UNIFORM_CONST_VECTOR(NullUniCVec, VEC_TC_NULL);
DEFINE_UNIFORM_CONST_VECTOR(IntegerUniCVec, VEC_TC_INTEGER);
DEFINE_UNIFORM_CONST_VECTOR(UIntegerUniCVec, VEC_TC_UINTEGER);
DEFINE_UNIFORM_CONST_VECTOR(FloatUniCVec, VEC_TC_FLOAT);
DEFINE_UNIFORM_CONST_VECTOR(DoubleUniCVec, VEC_TC_DOUBLE);
DEFINE_UNIFORM_CONST_VECTOR(FixedDoubleUniCVec, VEC_TC_FIXED_DOUBLE);
DEFINE_UNIFORM_CONST_VECTOR(DateTimeUniCVec, VEC_TC_DATETIME);
DEFINE_UNIFORM_CONST_VECTOR(DateUniCVec, VEC_TC_DATE);
DEFINE_UNIFORM_CONST_VECTOR(TimeUniCVec, VEC_TC_TIME);
DEFINE_UNIFORM_CONST_VECTOR(YearUniCVec, VEC_TC_YEAR);
DEFINE_UNIFORM_CONST_VECTOR(EnumSetUniCVec, VEC_TC_ENUM_SET);
DEFINE_UNIFORM_CONST_VECTOR(UnknownUniCVec, VEC_TC_UNKNOWN);
DEFINE_UNIFORM_CONST_VECTOR(BitTypeUniCVec, VEC_TC_BIT);
DEFINE_UNIFORM_CONST_VECTOR(TimestampTZUniCVec, VEC_TC_TIMESTAMP_TZ);
DEFINE_UNIFORM_CONST_VECTOR(TimestampTinyUniCVec, VEC_TC_TIMESTAMP_TINY);
DEFINE_UNIFORM_CONST_VECTOR(IntervalYMUniCVec, VEC_TC_INTERVAL_YM);
DEFINE_UNIFORM_CONST_VECTOR(IntervalDSUniCVec, VEC_TC_INTERVAL_DS);
DEFINE_UNIFORM_CONST_VECTOR(DecInt32UniCVec, VEC_TC_DEC_INT32);
DEFINE_UNIFORM_CONST_VECTOR(DecInt64UniCVec, VEC_TC_DEC_INT64);
DEFINE_UNIFORM_CONST_VECTOR(DecInt128UniCVec, VEC_TC_DEC_INT128);
DEFINE_UNIFORM_CONST_VECTOR(DecInt256UniCVec, VEC_TC_DEC_INT256);
DEFINE_UNIFORM_CONST_VECTOR(DecInt512UniCVec, VEC_TC_DEC_INT512);
DEFINE_UNIFORM_CONST_VECTOR(NumberUniCVec, VEC_TC_NUMBER);
DEFINE_UNIFORM_CONST_VECTOR(ExtendUniCVec, VEC_TC_EXTEND);
DEFINE_UNIFORM_CONST_VECTOR(StrUniCVec, VEC_TC_STRING);
DEFINE_UNIFORM_CONST_VECTOR(ESInnerUniCVec, VEC_TC_ENUM_SET_INNER);
DEFINE_UNIFORM_CONST_VECTOR(RawUniCVec, VEC_TC_RAW);
DEFINE_UNIFORM_CONST_VECTOR(RowidUniCVec, VEC_TC_ROWID);
DEFINE_UNIFORM_CONST_VECTOR(LobUniCVec, VEC_TC_LOB);
DEFINE_UNIFORM_CONST_VECTOR(JsonUniCVec, VEC_TC_JSON);
DEFINE_UNIFORM_CONST_VECTOR(GeoUniCVec, VEC_TC_GEO);
DEFINE_UNIFORM_CONST_VECTOR(UdtUniCVec, VEC_TC_UDT);
DEFINE_UNIFORM_CONST_VECTOR(CollUniCVec, VEC_TC_COLLECTION);
DEFINE_UNIFORM_CONST_VECTOR(MySQLDateTimeUniCVec, VEC_TC_MYSQL_DATETIME);
DEFINE_UNIFORM_CONST_VECTOR(MySQLDateUniCVec, VEC_TC_MYSQL_DATE);
DEFINE_UNIFORM_CONST_VECTOR(RoaringBitmapUniCVec, VEC_TC_ROARINGBITMAP);
#undef DEFINE_UNIFORM_CONST_VECTOR

}
}
#endif // OCEANBASE_SHARE_VECTOR_OB_VECTOR_DEFINE_H_
