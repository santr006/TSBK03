#include "grid.h"

Grid::~Grid()
{

}

void Grid::compareGridToTexture(glm::vec4* texture, int w, int h, glm::vec3* normals, glm::vec4* resultPos, glm::vec3* resultNorm)
{
	if (w != width || h != height)
	{
		printf("wrong size of texture\n");
		return;
	}

	//position is a vec4
	//normal is a vec3

	int noPixelsBetweenPhotonsW = w / noPointsOnWidth;
	int noPixelsBetweenPhotonsH = h / noPointsOnHeight;

	int row = 0;
	int column = 0;
	int j = 0;
	for (int i = 0; i < w * h; i++)
	{
		if (row % noPixelsBetweenPhotonsH == 0)
		{
			if (column % noPixelsBetweenPhotonsW == 0)
			{
				resultPos[j] = texture[i];
				resultNorm[j] = normals[i];
				j++;
			}
			column++;
		}

		//new row in texture
		if ((i + 1) % w == 0)
		{
			row++;
			column = 0;
		}
	}

}