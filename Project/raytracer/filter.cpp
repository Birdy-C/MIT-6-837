#include "stdafx.h"
#include "filter.h"

Vec3f Filter::getColor(int i, int j, Film * film)
{
    float weight = 0;
    Vec3f color = Vec3f();
    int r = getSupportRadius();
    int samples = film->getNumSamples();
    for (int ii = -r; ii <= r; ii++)
    {
        for (int jj = -r; jj <= r; jj++)
        {
            int x = i + ii;
            int y = j + jj;
            if (x < 0 || y < 0 || x >= film->getWidth() || y >= film->getHeight())
                continue;
            for (int t = 0; t < samples; t++)
            {
                Sample temp = film->getSample(x, y, t);
                float c = getWeight(x + temp.getPosition().x() - i, y + temp.getPosition().y() - j);
                color += temp.getColor() * c;
                weight += c;
            }
        }
    }
    color /= weight;
    return color;
}

float BoxFilter::getWeight(float x, float y)
{
    if(x > radius || x < -radius || y > radius || y < -radius) 
        return 0.0f;
    return 1;
}

float TentFilter::getWeight(float x, float y)
{
    float d = x * x + y * y;
    if (d > radius * radius)
        return 0.0f;
    return 1 - sqrt(d) / radius;
}

float GaussianFilter::getWeight(float x, float y)
{
    float d = x * x + y * y;
    if(d > radius * radius) 
        return 0.0f;
    return exp(-d / (2 * sigma * sigma));
}
