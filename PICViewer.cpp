
#include <iostream>
#include "PICViewer.h"
#include <bitset>
//#include <sstream>

#include "PICViewer.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setupUi(this);
}

MainWindow::~MainWindow()
{

}

void MainWindow::updateLabel(QImage myImage, QString myInfo)
{
	label_4->setPixmap(QPixmap::fromImage(myImage));
	label->setText(myInfo);
}

void findPixelValues(BYTE myByte, int TotalPixels, int RepeatNum, QImage& myImage)
{
	int m;
	int x;
	int y;
	QRgb value;

	std::bitset<8> binary(myByte);

	for(m = 7; m > 0; m-=2)
	{
		switch ((binary[m] * 2) + binary[m-1]) 
		{
			case 0:
				value = qRgb(0, 0, 0); // Black
				break;
			case 1:
				value = qRgb(0, 170, 170); // Blue
				break;
			case 2:
				value = qRgb(170, 0, 0); // Red
				break;
			case 3:
				value = qRgb(170, 170, 170); // White
				break;
		}
	}

	int new_x;

	for(x = TotalPixels; x < (RepeatNum + TotalPixels); x++)
	{
		y = (199 - (x / 320));	
		new_x = x - (320 * (x / 320));
		 
		myImage.setPixel(new_x, y, value);
	}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow * window;
    window = new(MainWindow);

	PIC_File o_PIC_File;
	BYTE MyByte;
	WORD MyWord;
	FILE *File = NULL;
	BYTE RunMarker;
	WORD NumBlocks;
	int i;
	int Pixels = 0;
	int TotalPixels = 0;
	int RunLength;
	int RunCount;
	int RepeatNum;
	
	File = fopen(argv[1], "rb");

	int jim;
    printf("argc = %d\n", argc);
   
    for (jim = 0; jim < argc; jim++)
	{
    	printf("argv[%d] = %s\n", jim, argv[jim]);
	}

	fread(&o_PIC_File.FileHeader, sizeof(o_PIC_File.FileHeader), 1, File);

	printf("\nId: %x\n", o_PIC_File.FileHeader.Id);
	printf("Width: %d\n", o_PIC_File.FileHeader.Width);
	printf("Height: %d\n", o_PIC_File.FileHeader.Height);
	printf("X Offset: %d\n", o_PIC_File.FileHeader.XOffset);
	printf("Y Offset: %d\n", o_PIC_File.FileHeader.YOffset);
	printf("Plane Info: %x\n", o_PIC_File.FileHeader.PlaneInfo);
	printf("Palette Flag: %x\n", o_PIC_File.FileHeader.PaletteFlag);
	printf("Video Mode: %c\n", o_PIC_File.FileHeader.VideoMode);
	printf("Palette Type: %x, %x\n", o_PIC_File.FileHeader.PaletteType1, o_PIC_File.FileHeader.PaletteType2);
	printf("Palette Size: %x, %x\n", o_PIC_File.FileHeader.PaletteSize1, o_PIC_File.FileHeader.PaletteSize2);
/* COMMENTED OUT FOR ASALING.PIC
	fread(&MyByte, sizeof(BYTE), 1, File);
	fread(&MyByte, sizeof(BYTE), 1, File); */
	fread(&NumBlocks, sizeof(WORD), 1, File);

	QString Info;

	int Height = o_PIC_File.FileHeader.Height;
	int Width = o_PIC_File.FileHeader.Width;
	char buffer [500];
 	int n;
 	n = sprintf(buffer, "Width:\t\t%d\nHeight:\t\t%d\nX Offset:\t\t%d\nY Offset:\t\t%d\nPlane Info:\t\t%x\nPalette Flag:\t%x\nVideo Mode:\t%c\nPalette Type:\t%x, %x\nPalette Size:\t%x, %x", Width, Height, o_PIC_File.FileHeader.XOffset, o_PIC_File.FileHeader.YOffset, o_PIC_File.FileHeader.PlaneInfo, o_PIC_File.FileHeader.PaletteFlag, o_PIC_File.FileHeader.VideoMode, o_PIC_File.FileHeader.PaletteType1, o_PIC_File.FileHeader.PaletteType2, o_PIC_File.FileHeader.PaletteSize1, o_PIC_File.FileHeader.PaletteSize2);
  	   
	Info = buffer;

	/*printf("Size of File Header: %d\n", sizeof(o_PIC_File.FileHeader));
	printf("Size of Block Header: %d\n", sizeof(o_PIC_File.BlockHeader));*/

	printf("\nNumber of Data Blocks: %d\n", NumBlocks);

	QImage myImage(o_PIC_File.FileHeader.Width, o_PIC_File.FileHeader.Height, QImage::Format_ARGB32);

	for(i = 0; i < NumBlocks; i++)
	{
		fread(&o_PIC_File.BlockHeader, sizeof(o_PIC_File.BlockHeader), 1, File);

		printf("\nBlock #%d:\n", i+1);
		printf("  Header:\n");
		printf("    Block Size: %d\n", o_PIC_File.BlockHeader.BlockSize);
		printf("    Run Length: %d\n", o_PIC_File.BlockHeader.RunLength);
		printf("    Run Marker: %d\n", o_PIC_File.BlockHeader.RunMarker);
		
	//	printf("  Data:\n");
		Pixels = 0;
		while(Pixels != (o_PIC_File.BlockHeader.RunLength * 4))
		{	
			fread(&MyByte, sizeof(BYTE), 1, File);
			if(MyByte == o_PIC_File.BlockHeader.RunMarker)
			{
		//		printf("    Run Marker: %x\n", MyByte);
				fread(&MyByte, sizeof(BYTE), 1, File);
				if(MyByte != 0)
				{
		//			printf("    Run Length: %d\n", MyByte);
					RepeatNum = MyByte;
			//		printf("    RepeatNum: %d\n", RepeatNum);
					fread(&MyByte, sizeof(BYTE), 1, File);	
			//		printf("    Run Value: %d\n", MyByte);
				}
				else
				{
				//	printf("    Run Length: %d\n", MyByte);
					fread(&MyWord, sizeof(WORD), 1, File);	
				//	printf("    Run Count: %d\n", MyWord);
//					RunCount = MyWord;
					RepeatNum = MyWord;
				//	printf("    RepeatNum: %d\n", RepeatNum);
					fread(&MyByte, sizeof(BYTE), 1, File);
				//	printf("    Run Value: %d\n", MyByte);
				}
			}
			else
			{
			//	printf("    Value: %d\n", MyByte);
				RepeatNum = 1;
			}
			Pixels += (RepeatNum * 4);
			findPixelValues(MyByte, TotalPixels, RepeatNum*4, myImage);	
			TotalPixels += (RepeatNum * 4);

		//	printf("\nTotal number of pixels = %d\n", TotalPixels);
		}
		
	}
	
	printf("\n");

	fclose(File);

    window->show();
	window->updateLabel(myImage, Info);

	return app.exec();
}

