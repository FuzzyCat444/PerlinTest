#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>

uint32_t xyHash(int x, int y) {
    uint32_t xi = static_cast<uint32_t>(x);
    uint32_t yi = static_cast<uint32_t>(y);
    const uint32_t multiplier = 0x1d9a566b;
    uint32_t h = x * 0x3582cb57 + y + multiplier;
    h *= multiplier;
    h = h ^ (h >> 13) ^ (h << 17);
    h *= multiplier;
    h = h ^ (h >> 13) ^ (h << 17);
    return h;
}

double grad(int x, int y, double dx, double dy) {
    const double rt2 = 1.4142135623731;
    const double rt12 = 0.70710678118655;
    const double rt32 = 1.22474487139159;
    
    uint32_t h = xyHash(x, y) % 12;
    
    switch (h) {
    case 0:
        return rt2 * dx;
    case 1:
        return rt32 * dx + rt12 * dy;
    case 2:
        return rt12 * dx + rt32 * dy;
    case 3:
        return rt2 * dy;
    case 4:
        return -rt12 * dx + rt32 * dy;
    case 5:
        return -rt32 * dx + rt12 * dy;
    case 6:
        return -rt2 * dx;
    case 7:
        return -rt32 * dx + -rt12 * dy;
    case 8:
        return -rt12 * dx + -rt32 * dy;
    case 9:
        return -rt2 * dy;
    case 10:
        return rt12 * dx + -rt32 * dy;
    case 11:
        return rt32 * dy + -rt12 * dy;
    }
}

double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b) {
    return t * (a - b) + b;
}

double perlinNoise(double x, double y) {
    int xf = static_cast<int>(floor(x));
    int yf = static_cast<int>(floor(y));
    x -= xf;
    y -= yf;
    double u = fade(x);
    double v = fade(y);
    
    return lerp(v,
        lerp(u, 
            grad(xf + 1, yf + 1, x - 1, y - 1), 
            grad(xf, yf + 1, x, y - 1) 
        ), 
        lerp(u,
            grad(xf + 1, yf, x - 1, y), 
            grad(xf, yf, x, y)
        )
    );
}

int main() {
    const int imgSize = 1000;
    
    std::ofstream ofs("img.ppm");
    ofs << "P3" << std::endl;
    ofs << imgSize << " " << imgSize << std::endl;
    ofs << "255" << std::endl;
    
    double scale = 0.025;
    for (int yp = 0; yp < imgSize; yp++) {
        double y = yp * scale;
        for (int xp = 0; xp < imgSize; xp++) {
            double x = xp * scale;
            double z = perlinNoise(x, y);
            int gray = static_cast<int>(255 * 0.5 * (z + 1));
            ofs << gray << " " << gray << " " << gray << " ";
        }
    }
    return 0;
}