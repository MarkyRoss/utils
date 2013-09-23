/*
 *  Copyright (c) Mark Ross <mark.ross@whitehatdev.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void Dissemble(int count, char* file);
void Assemble(int count, char* file);

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		printf("Usage: chunker [-d:-a] <count> <file>\n");
		return 0;
	}
	if(!strcmp("-d", argv[1]))
	{
		Dissemble((int)(argv[2][0] - '0'), argv[3]);
	}
	else if(!strcmp("-a", argv[1]))
	{
		Assemble((int)(argv[2][0] - '0'), argv[3]);
	}
	else
	{
		fprintf(stderr, "Second argument should be -d or -a (disassemble | assemble)\n");
		return 1;
	}
	return 0;
}

void Dissemble(int count, char* file)
{
	FILE* fh = fopen(file, "rb");
	fseek(fh, 0L, SEEK_END);
	int size = ftell(fh);
	fseek(fh, 0L, 0);
	int remaining = size;
	int bits = 0;
	int chunksize = (size / count);
	while(remaining)
	{
		int toread;
		if(remaining >= chunksize)
		{
			toread = chunksize;
		}
		else
		{
			toread = remaining;
		}
		char* contents = (char*)malloc(toread);
		fread(contents, sizeof(char), toread, fh);
		FILE* tmp;
		char* chunkname = (char*)malloc(4 + strlen(file) + 1);
		sprintf(chunkname, "%s.part%d", file, bits);
		tmp = fopen(chunkname, "wb");
		fwrite(contents, sizeof(char), toread, tmp);
		fclose(tmp);
		bits++;
		remaining = remaining - toread;
	}
	fclose(fh);
}

void Assemble(int count, char* file)
{
	FILE* orignal = fopen(file, "wb");
	int bits = 0;
	while(bits < count)
	{
		char* chunkname = (char*)malloc(4 + strlen(file) + 1);
		sprintf(chunkname, "%s.part%d", file, bits);
		FILE* fh = fopen(chunkname, "rb");
		fseek(fh, 0L, SEEK_END);
		int toread = ftell(fh);
		fseek(fh, 0L, 0);
		char* contents = (char*)malloc(toread);
		fread(contents, sizeof(char), toread, fh);
		fwrite(contents, sizeof(char), toread, orignal);
		fclose(fh);
		bits++;
	}
}
