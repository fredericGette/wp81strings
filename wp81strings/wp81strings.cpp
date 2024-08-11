// wp81strings.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BUFFER_SIZE 200
#define MAX_STRING_SIZE 256

DWORD minRealStringLength = 5;

BOOL printableChar(UCHAR character)
{
	if (character > 0x1F && character < 0x7F)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL foundCString(UCHAR* cString, DWORD cStringLength, INT distance)
{
	if (cStringLength >= minRealStringLength)
	{
		// End of a string
		cString[cStringLength] = '\0';
		printf("%9d [%s]\n", distance, cString);
		return TRUE;
	}
	return FALSE;
}

BOOL foundWCString(UCHAR* wcString, DWORD wcStringLength, INT distance)
{
	if (wcStringLength >= minRealStringLength)
	{
		// End of a wide-char string
		wcString[wcStringLength] = '\0';
		printf("%9d [%s]w\n", distance, wcString);
		return TRUE;
	}
	return FALSE;
}

int main(int arg, char* argv[])
{
	HANDLE hFile;
	CHAR ReadBuffer[BUFFER_SIZE] = { 0 };
	DWORD dwBytesRead = 0;

	UCHAR cString[MAX_STRING_SIZE] = { 0 };
	DWORD cStringLength = 0;

	UCHAR wcString[MAX_STRING_SIZE] = { 0 };
	DWORD wcStringLength = 0;

	UCHAR previousChar = '\0';

	INT distance = 0;

	if (arg < 2) // arg #1 is the name of the current executable
	{
		printf("\n");
		printf("Usage: %s <file> [length]\n\n", argv[0]);
		printf("  Find all printable strings contained in a file.\n\n");
		printf("  Parameters:\n\n");
		printf("    file\tpath of a file containing strings.\n");
		printf("    length\tminimum length of a string (default %u).\n", minRealStringLength);
		return 1;
	}

	CHAR* fileName = argv[1];

	if (arg > 2)
	{
		int minLength = atoi(argv[2]);
		if (minLength < 2)
		{
			printf("Parameter 'length' is invalid : [%s]\nExpecting an integer > 1.\n", argv[2]);
			return 1;
		}
		minRealStringLength = minLength;
	}

	hFile = CreateFileA(fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Unable to open file [%s]\n", fileName);
		return 1;
	}

	do
	{
		if (ReadFile(hFile, ReadBuffer, BUFFER_SIZE, &dwBytesRead, NULL) == FALSE)
		{
			printf("Unable to read file [%s]\n", fileName);
			CloseHandle(hFile);
			return 1;
		}

		for (DWORD i = 0; i < dwBytesRead; i++)
		{
			distance++;
			UCHAR currentChar = ReadBuffer[i];

			if (printableChar(currentChar))
			{
				// Printable character found.
				// Could be a character of a char string or a character of a wide-char string.
				cString[cStringLength++] = currentChar;
				wcString[wcStringLength++] = currentChar;

				if (cStringLength == MAX_STRING_SIZE)
				{
					// We reached the max string size.
					// Force a new string creation.
					foundCString(cString, cStringLength, distance - cStringLength);
					distance = 0;
					cStringLength = 0; // Prepare to start a new char string.
					wcStringLength = 0; // A char string is found, we can invalidate the wide-char string.
				}
				else if (wcStringLength == MAX_STRING_SIZE)
				{
					// We reached the max string size.
					// Force a new string creation.
					foundWCString(wcString, wcStringLength, distance - wcStringLength*2);
					distance = 0;
					wcStringLength = 0; // Prepare to start a new wide-char string.
					cStringLength = 0; // A wide-char string is found, we can invalidate the char string.
				}
			}
			else if (currentChar == '\0')
			{
				// \0 character found.
				// Maybe the end of a char or wide-char string ?

				if (foundCString(cString, cStringLength, distance - cStringLength))
				{
					distance = 0;
					wcStringLength = 0; // A char string is found, we can invalidate the wide-char string.
				}
				else
				{
					// It's not a char string, perhaps it's a wide-char string ?
					if (previousChar == '\0')
					{
						// We have two \0 character
						// Could be the end of a wide-char string.

						if (foundWCString(wcString, wcStringLength, distance - wcStringLength * 2))
						{
							distance = 0;
						}
						wcStringLength = 0; // Prepare to start a new wide-char string.
					}
					// Otherwise, we assume the \0 is the high byte of a wide-char.
				}
				cStringLength = 0; // Prepare to start a new char string.

			}
			else
			{
				// Not interesting character.
				// Maybe the end of a char or wide-char string ?

				if (!foundCString(cString, cStringLength, distance - cStringLength))
				{
					// It's not a char string, perhaps it's a wide-char string ?
					if (foundWCString(wcString, wcStringLength, distance - wcStringLength * 2))
					{
						distance = 0;
					}
				}
				else
				{
					distance = 0;
				}
				cStringLength = 0; // Prepare to start a new char string.
				wcStringLength = 0; // Prepare to start a new wide-char string.
			}

			previousChar = currentChar;
		}

	} while (dwBytesRead > 0);

	CloseHandle(hFile);

	return 0;
}


