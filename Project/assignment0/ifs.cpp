#define _CRT_SECURE_NO_WARNINGS

#include "ifs.h"

IFS::IFS():num(0)
{
}

IFS::IFS(char *file)
{
	readFile(file);
}


void IFS::readFile(char * file)
{

	transformation.clear();
	p.clear();

	// open the file
	FILE *input = fopen(file, "r");
	assert(input != NULL);

	// read the number of transforms
	int num_transforms;
	fscanf(input, "%d", &num_transforms);
	assert(num_transforms > 0);

	// < DO SOMETHING WITH num_transforms >
	num = num_transforms;

	float temp = 0;
	// read in the transforms
	for (int i = 0; i < num_transforms; i++) {
		float probability;
		fscanf(input, "%f", &probability);
		Matrix m;
		m.Read3x3(input);
		// < DO SOMETHING WITH probability and m >
		transformation.push_back(m);

		temp += probability;
		//概率记录方式是 在这个之前的所有的概率
		p.push_back(temp);
	}
	p[num_transforms - 1] = 1.01;//防止精度误差

	// close the file
	fclose(input);
}

void IFS::render(Image & img, int pointNum, int iter)
{
    // TODO0
	Vec3f color(0, 0, 0);
	int Width = img.Width();
	int Height = img.Height();
	while (pointNum--)
	{
		Vec4f point((float)rand() / 32758, (float)rand() / 32758, 0, 1);
		for (int i = 0; i < iter; i++)
		{
			float randfloat = (float)rand() / 32758;
			Matrix m = transformation.back();
			for (int j = 0; j < p.size(); j++)
			{
				if (randfloat < p.at(j))
				{
					m = transformation.at(j);
					break;
				}
			}
			point *= m;
		}
        // clamp
		int tx = min((int)(point.x()*Width), Width - 1);
		int ty = min((int)(point.y()*Height), Height - 1);
		tx = max(tx, 0);
		ty = max(ty, 0);
		img.SetPixel(tx, ty, color);
		// cout << min((int)(point.x()*Width), Width - 1) << min((int)(point.y()*Height), Height - 1) << endl;
		//img.SetPixel((int)(point.x() * Width, (int)point.y() * Height, color);

	}
}

