#ifndef COLOR_H
#define COLOR_H

#pragma once

#include <windows.h>

class Color
{
private:
	UCHAR m_Color [ 4 ];

public:
	Color( )
	{
		*( ( int* )this ) = 0;
	}
	Color( int r, int g, int b, int a = 255 )
	{
		SetColor( r, g, b, a );
	}
	int r( ) const
	{
		return m_Color [ 0 ];
	}
	int g( ) const
	{
		return m_Color [ 1 ];
	}
	int b( ) const
	{
		return m_Color [ 2 ];
	}
	int a( ) const
	{
		return m_Color [ 3 ];
	}
	int GetRawColor( ) const
	{
		return *( ( int* )this );
	}
	Color& operator = ( const Color& other )
	{
		SetRawColor( other.GetRawColor( ) );
		return *this;
	}
	bool operator == ( const Color& other ) const
	{
		return ( *( ( int* )this ) == *( ( int* ) &other ) );
	}
	bool operator != ( const Color& other ) const
	{
		return !( operator == ( other ) );
	}
	UCHAR& operator [] ( int iIndex )
	{
		switch ( iIndex )
		{
		case 1:
			return m_Color [ 1 ];
		case 2:
			return m_Color [ 2 ];
		case 3:
			return m_Color [ 3 ];
		default:
			return m_Color [ 0 ];
		}
	}
	const UCHAR& operator [] ( int iIndex ) const
	{
		switch ( iIndex )
		{
		case 1:
			return m_Color [ 1 ];
		case 2:
			return m_Color [ 2 ];
		case 3:
			return m_Color [ 3 ];
		default:
			return m_Color [ 0 ];
		}
	}
	float* ToFloatPtr( ) const
	{
		static float pflColor [ 4 ] = { 0.0f };

		pflColor [ 0 ] = float( r( ) );
		pflColor [ 1 ] = float( g( ) );
		pflColor [ 2 ] = float( b( ) );
		pflColor [ 3 ] = float( a( ) );

		return pflColor;
	}
	void SetColor( int r, int g, int b, int a = 255 )
	{
		m_Color [ 0 ] = UCHAR( r );
		m_Color [ 1 ] = UCHAR( g );
		m_Color [ 2 ] = UCHAR( b );
		m_Color [ 3 ] = UCHAR( a );
	}
	void SetRawColor( int iColor32 )
	{
		*( ( int* )this ) = iColor32;
	}
	float* Base( )
	{
		float clr [ 3 ];

		clr [ 0 ] = m_Color [ 0 ] / 255.0f;
		clr [ 1 ] = m_Color [ 1 ] / 255.0f;
		clr [ 2 ] = m_Color [ 2 ] / 255.0f;

		return &clr [ 0 ];
	}

public:
	template<class T> T R( ) const
	{
		return ( T ) m_Color [ 0 ];
	}
	template<class T> T G( ) const
	{
		return ( T ) m_Color [ 1 ];
	}
	template<class T> T B( ) const
	{
		return ( T ) m_Color [ 2 ];
	}
	template<class T> T A( ) const
	{
		return ( T ) m_Color [ 3 ];
	}
};

#endif