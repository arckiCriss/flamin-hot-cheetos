#include "Drawing.h"

Drawing drawing;

Drawing::Drawing( )
{
	resetFonts( );
}

void Drawing::resetFonts( )
{
	menuFont = 0, menuTitleFont = 0;
	espFont = 0;
	scoreboardFont = 0;
}

void Drawing::initializeFonts( )
{
	resetFonts( );

	menuFont = interfaces::surface->createFont( );
	interfaces::surface->setFontGlyphSet( menuFont, charenc( "Verdana" ), 13, 20, 0, 0, FONTFLAG_ANTIALIAS );

	menuTitleFont = interfaces::surface->createFont( );
	interfaces::surface->setFontGlyphSet( menuTitleFont, charenc( "Tahoma" ), 13, 600, 0, 0, FONTFLAG_ANTIALIAS );

	espFont = interfaces::surface->createFont( );
	interfaces::surface->setFontGlyphSet( espFont, charenc( "Tahoma" ), 13, 1250, 0, 0, FONTFLAG_DROPSHADOW );

	scoreboardFont = interfaces::surface->createFont( );
	interfaces::surface->setFontGlyphSet( scoreboardFont, charenc( "Tahoma" ), 13, 500, 0, 0, FONTFLAG_ANTIALIAS );
}

int getWidth( unsigned long font, const char* text, ... )
{
	int width = 0, height = 0;
	int bufferSize = MultiByteToWideChar( CP_UTF8, 0x0, text, -1, 0, 0 );

	wchar_t* unicode = new wchar_t [ bufferSize ];
	MultiByteToWideChar( CP_UTF8, 0x0, text, -1, unicode, bufferSize );

	interfaces::surface->getTextSize( font, unicode, width, height );
	delete [ ] unicode;

	return width;
}

void Drawing::drawString( unsigned long font, bool shouldCenter, int x, int y, Color color, const char* fmt, ... )
{
	if ( !fmt )
		return;

	va_list args;
	va_start( args, fmt );
	char buffer [ 1024 ];
	vsprintf_s( buffer, fmt, args );

	if ( shouldCenter )
		x -= getWidth( font, buffer ) / 2;

	interfaces::surface->drawSetTextPos( x, y );
	interfaces::surface->drawSetTextFont( font );
	interfaces::surface->drawSetTextColor( color );

	int chars = MultiByteToWideChar( CP_ACP, 0, buffer, -1, NULL, 0 );

	const WCHAR* text = new WCHAR [ chars ];
	MultiByteToWideChar( CP_ACP, 0, buffer, -1, ( LPWSTR ) text, chars );

	va_end( args );

	interfaces::surface->drawPrintText( text, wcslen( text ) );
}

void Drawing::drawLine( int x1, int y1, int x2, int y2, Color color )
{
	interfaces::surface->drawSetColor( color );
	interfaces::surface->drawLine( x1, y1, x2, y2 );
}

void Drawing::drawFilledRect( int x, int y, int w, int h, Color color )
{
	interfaces::surface->drawSetColor( color );
	interfaces::surface->drawFilledRect( x, y, x + w, y + h );
}

void Drawing::drawOutlinedRect( int x, int y, int w, int h, Color color )
{
	interfaces::surface->drawSetColor( color );
	interfaces::surface->drawOutlinedRect( x, y, x + w, y + h );
}

void Drawing::drawOutlinedBox( int x, int y, int w, int h, Color color, Color colorOutline )
{
	drawOutlinedRect( x, y, w, h, color );

	interfaces::surface->drawSetColor( colorOutline );
	interfaces::surface->drawOutlinedRect( x + 1, y + 1, x + w - 1, y + h - 1 );
	interfaces::surface->drawOutlinedRect( x - 1, y - 1, x + w + 1, y + h + 1 );
}

void Drawing::drawBar( int x, int y, int w, int h, Color color, float value )
{
	x -= w / 2;
	y -= h / 2;

	drawFilledRect( x, y, w, h + 1, Color( 0, 0, 0 ) );

	UINT height = ( UINT ) ( ( ( h + 1 ) * value ) / 100 );
	drawFilledRect( x + 1, y + 1, w - 2, height - 2, color );
}