
#ifndef PICViewer_HPP
#define PICViewer_HPP

#include <fstream>
#include <iostream>

typedef unsigned short WORD;
typedef unsigned char BYTE;

class PIC_File
{
	/* As both data structures are an odd number of bytes, the compiler adds
	   packing to aid data access.  This messes up reading the blocks of data
	   Adding the packed attribute to the structure ensures that the correct
	   number of bytes are read */

	public:
		/* File Header (17 bytes long) */
		/* 7 WORDs of 2 bytes + 3 BYTEs = 17 bytes */
		typedef struct __attribute__((packed))
		{
			WORD Id;			/* Magic number (always 1234h) */
			WORD Width;			/* Width of image in pixels */
			WORD Height;		/* Height of image in pixels */
			WORD XOffset;		/* X of lower left corner of image */
			WORD YOffset;		/* Y of lower left corner of image */
			BYTE PlaneInfo;		/* BPP and number color planes */
			BYTE PaletteFlag;	/* Color palette/video flag */
			BYTE VideoMode;		/* Video mode of image */
			/* Breaking PaletteType word into 2 bytes */
			BYTE PaletteType1;	/* Type of color palette */
			BYTE PaletteType2;	/* Type of color palette */
			/* Breaking PaletteSize word into 2 bytes */
			BYTE PaletteSize1;	/* Size of color palette */
			BYTE PaletteSize2;	/* Size of color palette */
		}PIC_Header;

		/* Block Header (5 bytes long) */
		/* 2 WORDS of 2 bytes + 1 BYTE = 5 bytes */
		typedef struct __attribute__((packed))
		{
			WORD BlockSize;    /* Size of encoded block including header */
			WORD RunLength;    /* Size of decoded pixel data */
			BYTE RunMarker;    /* Start-of-run indicator */
		} PIC_Block_Header;

		PIC_Header FileHeader;
		PIC_Block_Header BlockHeader;
};

#endif
