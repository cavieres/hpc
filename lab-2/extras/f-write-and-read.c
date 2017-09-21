// http://www.java2s.com/Code/C/File/Readandwritearrayintofile.htm

#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	float floatValue[5] = { 1.1F, 2.2F, 3.3F, 4.4F, 5.5F};
	//float dynArray[5];
	int i;
	
	int N = 5;
	float *dynArray;
	dynArray = (float *)malloc(N * sizeof(float));
	float oneFloatValue;
	
	if ((fp = fopen("test.raw", "wb")) == NULL)
		printf("Cannot open file.\n");

	if (fwrite(floatValue, sizeof(float), N, fp) != N)
		printf("File read error.");
	
	fclose(fp);
	
	// Read Values.
	if ((fp = fopen("test.raw", "rb")) == NULL)
		printf("Cannot open file.\n");
	
	if (fread(dynArray, sizeof(float), N, fp) != N)
	{
		if (feof(fp))
			printf("Premature end of file.");
		else
			printf("File read error.");
	}
	
	/*for (i = 0; i < 5; i++)
	{
		fread(&oneFloatValue, sizeof(float) * N, 1, fp);
		dynArray[i] = oneFloatValue;
	}*/
	
	fclose(fp);
	
	for (i = 0; i < 5; i++)
		printf("%f ", dynArray[i]);
	
	printf("\n");
	
	return 0;
}