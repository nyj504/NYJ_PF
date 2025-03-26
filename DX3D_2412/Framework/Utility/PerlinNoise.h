#pragma once
class PerlinNoise
{
public:
    PerlinNoise(UINT seed = 123456)
    {
        for (int i = 0; i < 256; i++)
            PermutationTable[i] = i;

        for (int i = 255; i > 0; i--)
        {
            int j = rand() % (i + 1);
            swap(PermutationTable[i], PermutationTable[j]);
        }

        for (int i = 0; i < 256; i++)
            PermutationTable[256 + i] = PermutationTable[i];
    }

    float Noise(float x, float z)
    {
        int X = (int)floor(x) & 255;
        int Z = (int)floor(z) & 255;
        float xf = x - floor(x);
        float zf = z - floor(z);

        float u = Fade(xf);
        float v = Fade(zf);

        int A = PermutationTable[X] + Z;
        int B = PermutationTable[X + 1] + Z;

        float gradAA = Grad(PermutationTable[A], xf, zf);
        float gradBA = Grad(PermutationTable[B], xf - 1, zf);
        float gradAB = Grad(PermutationTable[A + 1], xf, zf - 1);
        float gradBB = Grad(PermutationTable[B + 1], xf - 1, zf - 1);

        float lerpX1 = GameMath::Lerp(gradAA, gradBA, u);
        float lerpX2 = GameMath::Lerp(gradAB, gradBB, u);
        return GameMath::Lerp(lerpX1, lerpX2, v);
    }

private:
    float Fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float Grad(int hash, float x, float z)
    {
        int h = hash & 15; // 하위 4비트 값
        float u = h < 8 ? x : z; // 0~7: x, 8~15: z
        float v = h < 4 ? z : (h == 12 || h == 14 ? x : 0); // 12, 14는 x 사용, 나머지는 0

        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }
 
private:
    int PermutationTable[512];
};