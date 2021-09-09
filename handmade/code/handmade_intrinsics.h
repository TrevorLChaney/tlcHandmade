#ifndef HANDMADE_INTRINSICS_H
#define HANDMADE_INTRINSICS_H

#include <math.h>

inline int32_t
RoundFloatToInt32(float Real32) {
    int32_t result = (int32_t)(Real32 + 0.5f);
    return result;
}

inline uint32_t
RoundFloatToUInt32(float Real32) {
    uint32_t result = (uint32_t)(Real32 + 0.5f);
    return result;
}

inline uint32_t
FloorFloatToInt32(float Real32) {
    int32_t result = (int32_t)floorf(Real32);
    return result;
}

inline uint32_t
TruncateFloatToInt32(float Real32) {
    int32_t result = (int32_t)(Real32);
    return result;
}

inline float
Sin(float Angle) {
    float Result = sinf(Angle);
    return Result;
}

inline float
Cos(float Angle) {
    float Result = cosf(Angle);
    return Result;
}

inline float
ATan2(float Y, float X) {
    float Result = atan2f(Y, X);
    return Result;
}

#endif
