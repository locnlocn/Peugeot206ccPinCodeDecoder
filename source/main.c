
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t map0_9[] = {0xcf, 0xce, 0xcd, 0xcc, 0xcb, 0xca, 0xc9, 0xc8, 0xc7, 0xc6};
uint8_t map0_9decode[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
uint8_t mapA_Z[] = {0xbe, 0xbd, 0xbc, 0xbb, 0xba, 0xb9, 0xb8, 0xb7, 0xb6, 0xb5,
					0xb4, 0xb3, 0xb2, 0xb1, 0xb0, 0xaf, 0xae, 0xad, 0xac, 0xab,
					0xaa, 0xa9, 0xa8, 0xa7, 0xa6, 0xa5};
uint8_t mapA_Zdecode[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
						  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
						  'U', 'V', 'W', 'X', 'Y', 'Z'};

int find_index(uint8_t val, uint8_t *map, int len)
{
	for (int i = 0; i < len; ++i)
	{
		if (map[i] == val)
			return i;
	}
	return -1;
}

int main(int argc, char *argv[])
{

	if (argc != 2)
	{
		printf("Usage: %s <binfile>\n", argv[0]);
		return 1;
	}
	FILE *fp = fopen(argv[1], "rb");
	if (!fp)
	{
		perror("fopen");
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	long filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	uint8_t *data = (uint8_t *)malloc(filesize);
	if (!data)
	{
		perror("malloc");
		fclose(fp);
		return 1;
	}
	fread(data, 1, filesize, fp);
	fclose(fp);

	for (long i = 0; i < filesize - 3; ++i)
	{
		int valid = 1;
		for (int k = 0; k < 4; ++k)
		{
			int idx0 = find_index(data[i + k], map0_9, 10);
			int idxA = find_index(data[i + k], mapA_Z, 26);
			if (idx0 == -1 && idxA == -1)
			{
				valid = 0;
				break;
			}
		}
		if (valid)
		{

			printf("Addr: 0x%lx, Hex: %02X %02X %02X %02X\n", i, data[i], data[i + 1], data[i + 2], data[i + 3]);

			printf("Reverse Hex: %02X %02X %02X %02X\n", data[i + 3], data[i + 2], data[i + 1], data[i]);

			printf("Reverse Decode\r\n");
			for (int k = 3; k >= 0; --k)
			{
				int idx0 = find_index(data[i + k], map0_9, 10);
				int idxA = find_index(data[i + k], mapA_Z, 26);
				if (idx0 != -1)
					printf("%c", map0_9decode[idx0]);
				else if (idxA != -1)
					printf("%c", mapA_Zdecode[idxA]);
			}
			printf("\n\n");
		}
	}

	printf("All decode: ");
	for (long i = 0; i < filesize; ++i)
	{
		int idx0 = find_index(data[i], map0_9, 10);
		int idxA = find_index(data[i], mapA_Z, 26);
		if (idx0 != -1)
			printf("%c", map0_9decode[idx0]);
		else if (idxA != -1)
			printf("%c", mapA_Zdecode[idxA]);
	}
	printf("\n");

	free(data);
	return 0;
}
