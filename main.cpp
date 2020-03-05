// Program to split big *.wav file into smaller files.
// Originally written for the use of N64 music tracks.
// Although, it surely may have uses for other things.

// Wave File Structure:
// --------------------
// 0x4(Int) - Length Header?
// 0x28(Int) - Length?
// 0x44(Char) - Sounds Data?
// --------------------

// Libraries.
#include <iostream>

// Macros.
#define HEADER  44     // Byte length for a wave header.
#define ONESEC  176400 // Byte length for one second of audio (stereo). // 173 KB of space.
#define ONESEC2 88200  // Byte length for one second of audio (mono).   // 87  KB of space.

// Variables.
FILE *fileIn;

int seconds;
int duration;
int type; // mono or stereo

char fileName[20];
char newFileName[20];

char header[HEADER] = 
	{
	0x52,0x49,0x46,0x46,0x34,0xB1,0x02,0x00,0x57,0x41,0x56,0x45,0x66,0x6D,0x74,0x20,
	0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,0x44,0xAC,0x00,0x00,0x10,0xB1,0x02,0x00,
	0x04,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x10,0xB1,0x02,0x00,
	};

char header2[HEADER] =
	{
	0x52,0x49,0x46,0x46,0xAC,0x58,0x01,0x00,0x57,0x41,0x56,0x45,0x66,0x6D,0x74,0x20,
	0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x44,0xAC,0x00,0x00,0x88,0x58,0x01,0x00,
	0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x88,0x58,0x01,0x00
	};

void splitWav(char fileName[20])
	{
	printf("Opening %s for reading... \n", fileName);
	
	if (type == 0)
		{
		printf("Splitting for mono... \n");
		}
	else if (type == 1)
		{
		printf("Splitting for stereo... \n");
		};
	
	// Open input file.
	fileIn = fopen(fileName, "rb");
	
	// Create a file for each second.
	FILE *fileOut[seconds];
	
	// First half of buffer.
	char soundBytesFirst[ONESEC];
	// Second half of buffer.
	char soundBytesLast[ONESEC];
	
	// Beginning of sound data.
	fseek(fileIn, 0x44, SEEK_SET);
	
	// Open each file.
	for (int i = 1; i <= seconds; i++)
		{
		sprintf(newFileName, "t_%d.wav", i);
		
		printf("Creating file %s... \n", newFileName);
		fileOut[i] = fopen(newFileName, "wb");
		
		printf("Writing header for %s... \n", newFileName);
		
		if (type == 1)
			{
			// Write header.
			for (int j = 0; j < HEADER; j++)
				{
				fwrite(&header[j], sizeof(char), 0x1, fileOut[i]);
				};
			
			// Read sound bytes.
			// We do it in halves, since the buffer is too small.
			printf("Reading section %d of %s... \n", i, fileName);
			for (int j = 1; j <= ONESEC/2; j++)
				{
				fread(&soundBytesFirst[j], sizeof(char), 0x1, fileIn);
				};
			printf("Reading second %d of %s... \n", i, fileName);
			for (int j = ONESEC/2+1; j <= ONESEC; j++)
				{
				fread(&soundBytesLast[j], sizeof(char), 0x1, fileIn);
				};
			
			// Write sound bytes.
			// We do it in halves, since the buffer is too small.
			printf("Writing first half of %s... \n", newFileName);
			for (int j = 1; j <= ONESEC/2; j++)
				{
				fwrite(&soundBytesFirst[j], sizeof(char), 0x1, fileOut[i]);
				};
			printf("Writing second half of %s... \n", newFileName);
			for (int j = ONESEC/2+1; j <= ONESEC; j++)
				{
				fwrite(&soundBytesLast[j], sizeof(char), 0x1, fileOut[i]);
				};
			}
		else if (type == 0)
			{
			// Write header.
			for (int j = 0; j < HEADER; j++)
				{
				fwrite(&header2[j], sizeof(char), 0x1, fileOut[i]);
				};
			
			// Read sound bytes.
			// We do it in halves, since the buffer is too small.
			printf("Reading section %d of %s... \n", i, fileName);
			for (int j = 1; j <= ONESEC2/2; j++)
				{
				fread(&soundBytesFirst[j], sizeof(char), 0x1, fileIn);
				};
			printf("Reading second %d of %s... \n", i, fileName);
			for (int j = ONESEC2/2+1; j <= ONESEC2; j++)
				{
				fread(&soundBytesLast[j], sizeof(char), 0x1, fileIn);
				};
			
			// Write sound bytes.
			// We do it in halves, since the buffer is too small.
			printf("Writing first half of %s... \n", newFileName);
			for (int j = 1; j <= ONESEC2/2; j++)
				{
				fwrite(&soundBytesFirst[j], sizeof(char), 0x1, fileOut[i]);
				};
			printf("Writing second half of %s... \n", newFileName);
			for (int j = ONESEC2/2+1; j <= ONESEC2; j++)
				{
				fwrite(&soundBytesLast[j], sizeof(char), 0x1, fileOut[i]);
				};
			};
		
		// Close each file.
		fclose(fileOut[i]);
		};
	
	// Close input file.
	fclose(fileIn);
	
	printf("Files split successfully. \n");
	};

int main()
	{
	printf("Type in the name of the *.wav file: ");
	scanf("%s", fileName);
	
	// Due to a bug in the program, 1 second must be added.
	// Otherwise, the last second of audio will be excluded from the process.
	//printf("What is the length of the *.wav file in seconds? (Add 1 Second) ");
	printf("What is the length of the *.wav file in seconds? ");
	scanf("%d", &seconds);
	
	printf("Split for mono(0) or stereo(1)? ");
	scanf("%d", &type);
	
	//printf("What is the duration of each file? ");
	//scanf("%d", &duration);
	
	splitWav(fileName);
	
	return 0;
	};
