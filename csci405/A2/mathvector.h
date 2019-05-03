#ifndef MATHVECTOR_H__
#define MATHVECTOR_H__

#include <stdlib.h>
#include <math.h>

class vector2
{
public:

	vector2() :
		m_X( 0.0f ), 
		m_Y( 0.0f )
	{
	}

	vector2( float x, float y ) :
		m_X( x ), 
		m_Y( y )
	{
	}
	
	vector2( const vector2 &other ) :
		m_X( other.m_X ), 
		m_Y( other.m_Y )
	{
	}

	const vector2& operator=( const vector2 &copyFrom )
	{
		if( this != &copyFrom )
		{
			m_X = copyFrom.m_X; 
			m_Y = copyFrom.m_Y;
		}
		return *this;
	}

	virtual float operator[]( int index )
	{
		switch( index )
		{
			case 0:
				return m_X;
			case 1:
				return m_Y;	
		}

		// assert( "What the ****" );
		return 0.0f;
	}

	vector2& operator+=( const vector2 &other )
	{
		if( &other != this )
		{
			m_X += other.m_X;
			m_Y += other.m_Y;
		}
		return *this;
	}

	vector2& operator-=( const vector2 &other )
	{
		if( &other != this )
		{
			m_X -= other.m_X;
			m_Y -= other.m_Y;
		}
		return *this;
	}

	vector2& operator*=( float scalar )
	{
		return Scale( scalar );
	}


	virtual ~vector2()
	{
	}

	float X() const { return m_X; };

	void X( float x ) { m_X = x; };

	float Y()  const { return m_Y; };

	void Y( float y ) { m_Y = y; };

	float Length() 
	{
		return sqrt( m_X*m_X + m_Y*m_Y );
	}

	vector2& Normalize() 
	{
		float length = Length();
		m_X /= length;
		m_Y /= length;
		return *this;
	} 

	vector2& Scale( float scalar ) 
	{
		m_X *= scalar;
		m_Y *= scalar;
		return *this;
	} 

	friend vector2 operator+( const vector2 &V1, const vector2 &V2 )
	{
		return vector2( V1.X() + V2.X(),  
						V1.Y() + V2.Y() );
	}

	friend vector2 operator-( const vector2 &V1, const vector2 &V2 )
	{
		return vector2( V1.X() - V2.X(),  
						V1.Y() - V2.Y() );
	}

	friend vector2 operator*( const vector2 &V1, float S )
	{
		return vector2( V1.X() * S,  
						V1.Y() * S );
	}

	friend vector2 operator*( float S, const vector2 &V1 )
	{
		return vector2( V1.X() * S,  
						V1.Y() * S );
	}

protected:

	float m_X;
	float m_Y;
};

class vector3 : public vector2
{
public:
	
	vector3() :
		vector2(), 
		m_Z( 0.0f )
	{
	}

	vector3( float x, float y, float z ) :
		vector2( x, y ), 
		m_Z( z )
	{
	}
	
	vector3( const vector2 &other ) :
		vector2( other ), 
		m_Z( 0.0f )
	{
	}

	vector3( const vector3 &other ) :
		vector2( other ), 
		m_Z( other.m_Z )
	{
	}

	const vector3& operator=( const vector3 &copyFrom )
	{
		if( this != &copyFrom )
		{		
			m_X = copyFrom.m_X; 
			m_Y = copyFrom.m_Y;
			m_Z = copyFrom.m_Z;
		}
		return *this;
	}

	virtual float operator[]( int index )
	{
		switch( index )
		{
			case 0:
				return m_X;
			case 1:
				return m_Y;	
			case 2:
				return m_Z;
		}

		// assert( "What the ****" );
		return 0.0f;
	}

	vector3& operator+=( const vector3 &other )
	{
		if( &other != this )
		{
			m_X += other.m_X;
			m_Y += other.m_Y;
			m_Z += other.m_Z;
		}
		return *this;
	}

	vector3 operator /(double d) const
	{ 
		return vector3( m_X/d,m_Y/d,m_Z/d);
	}

	vector3& operator-=( const vector3 &other )
	{
		if( &other != this )
		{
			m_X -= other.m_X;
			m_Y -= other.m_Y;
			m_Z -= other.m_Z;
		}
		return *this;
	}

	vector3& operator*=( float scalar )
	{
		return Scale( scalar );
	}

	float Z()  const { return m_Z; };

	void Z( float z ) { m_Z = z; };

	float Length() 
	{
		return sqrt( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
	}

	vector3& Normalize() 
	{
		float length = Length();
		m_X /= length;
		m_Y /= length;
		m_Z /= length;
		return *this;
	} 

	vector3& Scale( float scalar ) 
	{
		m_X *= scalar;
		m_Y *= scalar;
		m_Z *= scalar;
		return *this;
	} 

	static float DotProduct( const vector3 &V1, const vector3 &V2 )
	{
		return V1.X() * V2.X() + V1.Y() * V2.Y() + V1.Z() * V2.Z();
	}

	static vector3 CrossProduct( const vector3 &V1, const vector3 &V2 )
	{
		return vector3( V1.Y() * V2.Z() - V1.Z()*V2.Y(), 
						V1.Z() * V2.X() - V1.X()*V2.Z(), 
						V1.X() * V2.Y() - V1.Y()*V2.X() );
	}

	friend vector3 operator+( const vector3 &V1, const vector3 &V2 )
	{
		return vector3( V1.X() + V2.X(),  
						V1.Y() + V2.Y(),  
						V1.Z() + V2.Z() );
	}

	friend vector3 operator-( const vector3 &V1, const vector3 &V2 )
	{
		return vector3( V1.X() - V2.X(),  
						V1.Y() - V2.Y(),  
						V1.Z() - V2.Z() );
	}

	friend vector3 operator*( const vector3 &V1, float S )
	{
		return vector3( V1.X() * S,  
						V1.Y() * S,  
						V1.Z() * S );
	}

	friend vector3 operator*( float S, const vector3 &V1 )
	{
		return vector3( V1.X() * S,  
						V1.Y() * S,  
						V1.Z() * S );
	}
	friend vector3 operator*( const vector3 &V1, const vector3 &V2 )
	{
		return vector3( V1.X() * V2.X(),
				V1.Y() * V2.Y(),
				V1.Z() * V2.Z() );
	}

protected:

	float m_Z;
};


#endif // MATHVECTOR_H__
