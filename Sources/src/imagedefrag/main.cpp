#include "StdAfx.h"
#include "..\\Image\\Image.h"
#include "..\\Misc\\FileUtils.h"
#include "..\\Misc\\StrProc.h"

// Pack/Unpack функции требуют CSpritesPackBuilder из RandomMapGen — отключены
// DrawLines и CalculateWeight — рабочие функции

typedef IImage* PIImage;
bool LoadImageLibrary()
{
	// Image.lib слинкован статически — ImageProcessor регистрируется автоматически
	IImageProcessor *pIP = GetSingleton<IImageProcessor>();
	return (pIP != 0);
}

int CalculateWeight( const std::string &originalImage, DWORD /*dwMinAlpha*/ )
{
	CPtr<IDataStream> pOriginalImageStream = OpenFileStream( originalImage.c_str(), STREAM_ACCESS_READ );
	NI_ASSERT_TF( pOriginalImageStream != 0, NStr::Format("Can't open image file \"%s\"", originalImage.c_str()), return 0xDEAD );

	IImageProcessor *pIP = GetImageProcessor();
	CPtr<IImage> pOriginalImage = pIP->LoadImage( pOriginalImageStream );
	CImageAccessor imageAccessor = pOriginalImage;

	QWORD red = 0;
	QWORD blue = 0;
	QWORD green = 0;
	QWORD count = 0;
	for ( int nYindex = 0; nYindex < pOriginalImage->GetSizeY(); ++nYindex )
	{
		for ( int nXindex = 0; nXindex < pOriginalImage->GetSizeX(); ++nXindex )
		{
			SColor &rColor = imageAccessor[nYindex][nXindex];
			if ( rColor.a >= 1 )
			{
				red += rColor.r;
				blue += rColor.g;
				green += rColor.b;
				++count;
			}
		}
	}
	if ( count > 0 ) {
		red /= count;
		blue /= count;
		green /= count;
	}
	printf( "Weight: r, g, b = %d, %d, %d\npoints: %d\n", (int)red, (int)blue, (int)green, (int)count );
	return 0;
}

int main( int argc, char* argv[] )
{
	if ( argc < 2 )
	{
		printf( "Image Defragger 1.0\n" );
		printf( "Written by Michael Melnikov\n" );
		printf( "(C) Nival Interactive, 2001\n" );
		printf( "Usage: ImageDefrag.exe <w> <image file>\n" );
		printf( "w - calculate picture color weight\n" );
		return -1;
	}

	if( !LoadImageLibrary() )
	{
		printf( "Error. ImageProcessor not available.\n" );
		return 0xDEAD;
	}

	std::string szInputImage = argv[2];
	char cmd = argv[1][0];

	if( cmd == 'w' || cmd == 'W' )
	{
		return CalculateWeight( szInputImage, 1 );
	}
	else
	{
		printf( "Unknown command: %c\n", cmd );
		return -1;
	}
}
