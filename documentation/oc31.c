#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

FILE* file;

void crash()
{
	fprintf(stderr, "NOT IMPLEMENTED\n");
	exit(2);
}

int endianswap(int num)
{
    return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
}

int readbe()
{
    int num;
    fread(&num, sizeof(num), 1, file);

    return endianswap(num);
}

int readint()
{
    int num;
    fread(&num, sizeof(num), 1, file);
    return num;
}

static void oc31_backref(const uint8_t** compressed, uint8_t** uncompressed, int offset, int length)
{
    uint8_t* backref = (*uncompressed) - offset;
    backref--;

    while(length--)
        *(*uncompressed)++ = *backref++;

    int lastByte = (*compressed)[-1];
    int addByte = lastByte & 3;
    while(addByte)
    {
        *(*uncompressed)++ = *(*compressed)++;
        addByte--;
    }
}

void oc31_uncompress(const uint8_t* compressed, const int compressedSize, uint8_t* uncompressed, const int uncompressedSize)
{
    const uint8_t* const origCompressed = compressed;
    uint8_t* const origUncompressed = uncompressed;

    while( ( (compressed - origCompressed) < compressedSize ) )
    {
    	int proc1 = compressedSize - (compressed - origCompressed);
    	int proc2 = uncompressedSize - (uncompressed - origUncompressed);

        int flagByte = *compressed++;
        printf("flag byte: %#x %d\n", flagByte, flagByte);
        if(flagByte >= 0x10)
        {
            if(flagByte >= 0x40)
            {
                int unknown2 = *compressed++;

                int tmp = ((flagByte << 8) | unknown2);
                int length = tmp >> 13;
                int offset = (tmp >> 2) & 0x7FF;
                length++;

                printf("backref from flagbyte and 1 additional byte - offset: %d length: %d extra bytes: %d\n", offset, length, unknown2 & 3);

                oc31_backref(&compressed, &uncompressed, offset, length);
            }
            else if(flagByte >= 0x20)
            {
                int tmp = flagByte & 0x1F;
                if(tmp < 2)
                {
                    if(tmp == 0)
                    {
                        int unknown2 = *compressed++;
                        int length = unknown2 + 0x22;

                        int unknown3 = *compressed++;
                        int unknown4 = *compressed++;
                        int offset = (unknown3 << 8) | unknown4;
                        offset >>= 2;

                        printf("backref from flagbyte and 3 additional bytes - offset: %d length: %d extra bytes: %d\n", offset, length, unknown4 & 3);

                        oc31_backref(&compressed, &uncompressed, offset, length);
                    }
                    else
                    {
                        int unknown3 = *compressed++;
                        int unknown4 = *compressed++;
                        int length = (unknown3 << 8) | unknown4;

                        int unknown5 = *compressed++;
                        int unknown6 = *compressed++;
                        int offset = (unknown5 << 8) | unknown6;
                        offset >>= 2;

						printf("backref from flagbyte and 4 additional bytes - offset: %d length: %d extra bytes: %d\n", offset, length, unknown6 & 3);

                        oc31_backref(&compressed, &uncompressed, offset, length);
                    }
                }
                else
                {
                    tmp += 2;

                    int unknown2 = *compressed++;
                    int unknown3 = *compressed++;
                    int offset = (unknown2 << 8) | unknown3;
                    offset >>= 2;

                    printf("backref from flagbyte and 2 additional bytes - offset: %d length: %d extra bytes: %d\n", offset, tmp, unknown3 & 3);

                    oc31_backref(&compressed, &uncompressed, offset, tmp);
                }
            }
            else
            {
				int tmp = flagByte & 7;
				if(tmp < 2)
				{
					if(tmp == 0)
					{
						int length = *compressed++ + 0xA;

						int unknown3 = *compressed++;
						int unknown4 = *compressed++;
						int offset = (unknown3 << 8) | unknown4;
						offset >>= 2;
						offset += 0x4000;

						if(flagByte & 8)
							offset += 0x4000;

						printf("backref from 1 byte len and 2 byte offset + 0x4000 - offset: %d length: %d extra bytes: %d\n", offset, length, unknown4 & 3);

						oc31_backref(&compressed, &uncompressed, offset, length);
					}
					else //1
					{
						int unknown2 = *compressed++;
						int unknown3 = *compressed++;
						int length = (unknown2 << 8) | unknown3;

						int unknown4 = *compressed++;
						int unknown5 = *compressed++;
						int offset = (unknown4 << 8) | unknown5;
						offset >>= 2;
						offset += 0x4000;

						if(flagByte & 8)
							offset += 0x4000;

						printf("backref from 2 byte len and 2 byte offset + 0x4000 - offset: %d length: %d extra bytes: %d\n", offset, length, unknown5 & 3);

						oc31_backref(&compressed, &uncompressed, offset, length);
					}
				}
				else
				{
					tmp += 2;

					int unknown3 = *compressed++;
					int unknown4 = *compressed++;
					int offset = (unknown3 << 8) | unknown4;
					offset >>= 2;
					offset += 0x4000;

					if(flagByte & 8)
						offset += 0x4000;

					printf("backref from len and 2 byte offset + 0x4000 - offset: %d length: %d extra bytes: %d\n", offset, tmp, unknown4 & 3);

					oc31_backref(&compressed, &uncompressed, offset, tmp);
				}
            }
        }
        else if(flagByte >= 2) //copy bytes directly to output
        {
            int len = flagByte + 2;
            memmove(uncompressed, compressed, len);

            printf("copy flagbyte+2 bytes from compressed - length: %d\n", len);

            compressed += len;
            uncompressed += len;
        }
        else
        {
            if(flagByte == 0)
            {
                int unknown2 = *compressed++;
                int len = unknown2 + 0x12;

                memmove(uncompressed, compressed, len);

                printf("copy bytes from compressed with one byte length - length: %d\n", len);

                compressed += len;
                uncompressed += len;
            }
            else //1
            {
                int unknown2 = *compressed++;
                int unknown3 = *compressed++;
                int len = (unknown2 << 8) | unknown3;
                len >>= 2;

                if(len == 0)
				{
                	if((uncompressed - origUncompressed) == uncompressedSize)
						break;
				}

				printf("copy bytes from compressed with two bytes length - length: %d\n", len);

				crash(); //TODO: NOT TESTED
				memmove(uncompressed, compressed, len);
				
				compressed += len;
                uncompressed += len;
            }
        }
    }

	int proc1 = compressedSize - (compressed - origCompressed);
    if(proc1)
		fprintf(stderr, "Decompression went wrong\n");
}

int main()
{
    const char* filename = "BANK14.STY";
    int startOffset = 0xbc44;

    file = fopen(filename, "rb");
    fseek(file, startOffset, SEEK_SET);

    int compressedSize = readbe();
    readint(); //OC31
    int uncompressedSize = readint();
	compressedSize -= 8;

	uint8_t* compressed = calloc(1, compressedSize);
    fread(compressed, 1, compressedSize, file);

    uint8_t* uncompressed = calloc(1, uncompressedSize);

    oc31_uncompress(compressed, compressedSize - 1, uncompressed, uncompressedSize);

	if(compressed[compressedSize-1] != 0xFE) //??? this is still unknown
		crash(); //fprintf(stderr, "End byte: %#x\n", compressed[compressedSize-1]);

    FILE* fout = fopen("__OUT", "wb");
    fwrite(uncompressed, 1, uncompressedSize, fout);
    fclose(fout);

    fclose(file);
}