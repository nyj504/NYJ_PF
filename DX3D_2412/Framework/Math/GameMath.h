#pragma once

class GameMath
{
public:
    enum Direction
    {
        UP, DOWN, RIGHT, LEFT, NONE
    };

    static int Random(const int& min, const int& max);
    static float Random(const float& min, const float& max);
    static Vector3 Random(const Vector3& min, const Vector3& max);

    static float Clamp(const float& min, const float& max, const float& value);
    //static float Lerp(const float& start, const float& end, const float& t);
    //static Vector2 Lerp(const Vector2& start, const Vector2& end, const float& t);    
    //static Vector3 Lerp(const Vector3& start, const Vector3& end, const float& t);

    template <typename T>
    static T Lerp(const T& start, const T& end, const float& t);

    static bool NearlyEqual(const float& s1, const float& s2)
    {
        return abs(s1 - s2) < FLT_EPSILON;
    }
    static Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point);

    static UINT64 ChunkPosToKey(int x, int z)
    {
        return ((UINT64)(x) << 32) | ((UINT64)(z) & 0xFFFFFFFF);
    }

    static UINT64 GenerateBlockID(const Vector3 globalPos)
    {
        UINT64 x = (UINT64)(globalPos.x) & 0x1FFFFF;
        UINT64 y = (UINT64)(globalPos.y) & 0x3FF;
        UINT64 z = (UINT64)(globalPos.z) & 0x1FFFFF;

        return (x << 42) | (y << 32) | z;
    }

    static void KeyToChunkPos(UINT64 key, int& x, int& z)
    {
        x = (int)(key >> 32);
        z = (int)(key & 0xFFFFFFFF);
    }

};

template<typename T>
inline T GameMath::Lerp(const T& start, const T& end, const float& t)
{
    float temp = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * temp;
}


