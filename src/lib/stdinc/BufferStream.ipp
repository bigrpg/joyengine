

typedef union
{
	uint8 u8;
	int8 i8;
	char c;
	unsigned char uc;
	bool b;
} bw_netbyte;


typedef union
{
	uint16 u16;
	int16 i16;
	short s;
	unsigned short us;
} bw_netshort;


typedef union
{
	uint32 u32;
	int32 i32;
	int i;
	float f;
} bw_netlong;


typedef union
{
	uint64 u64;
	int64 i64;
	long long ll;
	unsigned long long ull;
	double d;
} bw_netlonglong;


// -----------------------------------------------------------------------------
// Section: Byte swapping macros
// -----------------------------------------------------------------------------

#ifdef _BIG_ENDIAN

inline short JG_HTONS( short x )
{
	short res =
		((x & 0x00ff) << 8) |
		((x & 0xff00) >> 8);
	return res;
}


inline long JG_HTONL( long x )
{
	long res =
		((x & 0x000000ff) << 24) |
		((x & 0x0000ff00) << 8) |
		((x & 0x00ff0000) >> 8) |
		((x & 0xff000000) >> 24);
	return res;
}


inline long long JG_HTONLL( long long x )
{
	long long res =
		((x & 0x00000000000000ffULL) << 56) |
		((x & 0x000000000000ff00ULL) << 40) |
		((x & 0x0000000000ff0000ULL) << 24) |
		((x & 0x00000000ff000000ULL) << 8) |
		((x & 0x000000ff00000000ULL) >> 8) |
		((x & 0x0000ff0000000000ULL) >> 24) |
		((x & 0x00ff000000000000ULL) >> 40) |
		((x & 0xff00000000000000ULL) >> 56);
	return res;
}


inline float JG_HTONF( float f )
{
	bw_netlong n;
	n.f = f;
	n.u32 = JG_HTONL( n.u32 );
	return n.f;
}


/**
 *  This one is a bit different, because the 360 will crash with alignment
 *  exceptions if you try to assign floats to non-aligned areas of memory.
 *  Parts of our code try to do exactly that, so a bit more coaxing is needed.
 */
inline void JG_HTONF_ASSIGN( float &dest, float f )
{
	bw_netlong *pDest = (bw_netlong*)&dest;
	pDest->u32 = *(uint32*)&f;
	pDest->u32 = JG_HTONL( pDest->u32 );
}


/**
 *  This is for streaming/destreaming between two 3 byte memory areas.  If your
 *  input or output is a long, use JG_PACK3()/JG_UNPACK3()
 */
inline void JG_HTON3_ASSIGN( char *pDest, const char *pData )
{
	pDest[0] = pData[2];
	pDest[1] = pData[1];
	pDest[2] = pData[0];
}


/**
 *  Pack the low 3 bytes of an int into a 3-byte memory area.
 */
inline void JG_PACK3( char *pDest, uint32 src )
{
	pDest[0] = (char)(src >> 16);
	pDest[1] = (char)(src >> 8);
	pDest[2] = (char)src;
}


/**
 *  Unpack a 3-byte memory area into an int.
 */
inline uint32 JG_UNPACK3( const char *pData )
{
	const uint8 *data = (const uint8*)pData;
	return (data[0] << 16) | (data[1] << 8) | data[2];
}


#else // BIG_ENDIAN

#define JG_HTONS( x ) x
#define JG_HTONL( x ) x
#define JG_HTONLL( x ) x
#define JG_HTONF( x ) x
#define JG_HTONF_ASSIGN( dest, x ) (dest = x)

inline void JG_HTON3_ASSIGN( char *pDest, const char *pData )
{
	pDest[0] = pData[0];
	pDest[1] = pData[1];
	pDest[2] = pData[2];
}

inline void JG_PACK3( char *pDest, uint32 src )
{
	pDest[0] = (char)src;
	pDest[1] = (char)(src >> 8);
	pDest[2] = (char)(src >> 16);
}

inline uint32 JG_UNPACK3( const char *pData )
{
	const uint8 *data = (const uint8*)pData;
	return data[0] | (data[1] << 8) | (data[2] << 16);
}

#endif

// The network-to-host operations are the same as the host-to-network ones.
#define JG_NTOHS( x ) JG_HTONS( x )
#define JG_NTOHL( x ) JG_HTONL( x )
#define JG_NTOHLL( x ) JG_HTONLL( x )
#define JG_NTOHF( x ) JG_HTONF( x )
#define JG_NTOHF_ASSIGN( dest, x ) JG_HTONF_ASSIGN( dest, x )
#define JG_NTOH3_ASSIGN( pDest, pData ) JG_HTON3_ASSIGN( pDest, pData )

// -----------------------------------------------------------------------------
// Section: Output streaming operators
// -----------------------------------------------------------------------------

inline BufferOStream& operator<<( BufferOStream &out, bw_netbyte x )
{
	JG_ASSERT( sizeof( bw_netbyte ) == 1 );
	*(char*)out.reserve( sizeof( x ) ) = x.c;
	return out;
}

inline BufferOStream& operator<<( BufferOStream &out, bw_netshort x )
{
	JG_ASSERT( sizeof( bw_netshort ) == 2 );
	*(short*)out.reserve( sizeof( x ) ) = JG_HTONS( x.s );
	return out;
}

inline BufferOStream& operator<<( BufferOStream &out, bw_netlong x )
{
	JG_ASSERT( sizeof( bw_netlong ) == 4 );
	*(uint32*)out.reserve( sizeof( x ) ) = JG_HTONL( x.u32 );
	return out;
}

inline BufferOStream& operator<<( BufferOStream &out, bw_netlonglong x )
{
	JG_ASSERT( sizeof( bw_netlonglong ) == 8 );
	*(uint64*)out.reserve( sizeof( x ) ) = JG_HTONLL( x.u64 );
	return out;
}

inline BufferOStream& operator<<( BufferOStream &out, uint8 x )
{
	bw_netbyte n;
	n.u8 = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, uint16 x )
{
	bw_netshort n;
	n.u16 = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, uint32 x )
{
	bw_netlong n;
	n.u32 = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, uint64 x )
{
	bw_netlonglong n;
	n.u64 = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, int32 x )
{
	bw_netlong n;
	n.i32 = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, int64 x )
{
	bw_netlonglong n;
	n.i64 = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, char x )
{
	bw_netbyte n;
	n.c = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, bool x )
{
	bw_netbyte n;
	n.b = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, short x )
{
	bw_netshort n;
	n.s = x;
	return out << n;
}


inline BufferOStream& operator<<( BufferOStream &out, float x )
{
	bw_netlong n;
	n.f = x;
	return out << n;
}

inline BufferOStream& operator<<( BufferOStream &out, double x )
{
	bw_netlonglong n;
	n.d = x;
	return out << n;
}

//This is because the ps3 compiler does not like converting to
//int32, it should be removed as soon as a better solution is
//found
#if defined( PLAYSTATION3 )
inline BufferOStream& operator<<( BufferOStream &out, long x )
{
	bw_netlong n;
	n.i32 = x;
	return out << n;
}
#endif

/*
inline BufferOStream& operator<<( BufferOStream &out, void* x )
{
	bw_netlong n;
	n.p = x;
	return out << n;
}
*/

// Provide the generalised template on intel so the server can compile without
// porting all the damn server interfaces to use MERCURY_[IO]STREAM()
#ifndef _BIG_ENDIAN
template <class TYPE>
inline BufferOStream& operator<<( BufferOStream &os, const TYPE &t )
{
	os.insertRaw( t );
	return os;
}
#endif


// -----------------------------------------------------------------------------
// Section: Input streaming operators
// -----------------------------------------------------------------------------

inline BufferIStream& operator>>( BufferIStream &in, bw_netbyte &x )
{
	JG_ASSERT( sizeof( bw_netbyte ) == 1 );
	x.c = *(char*)in.retrieve( sizeof( x ) );
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, bw_netshort &x )
{
	JG_ASSERT( sizeof( bw_netshort ) == 2 );
	x.s = JG_NTOHS( *(short*)in.retrieve( sizeof( x ) ) );
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, bw_netlong &x )
{
	JG_ASSERT( sizeof( bw_netlong ) == 4 );
	x.u32 = JG_NTOHL( *(uint32*)in.retrieve( sizeof( x ) ) );
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, bw_netlonglong &x )
{
	JG_ASSERT( sizeof( bw_netlonglong ) == 8 );
	x.u64 = JG_NTOHLL( *(uint64*)in.retrieve( sizeof( x ) ) );
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, uint8 &x )
{
	bw_netbyte n;
	in >> n;
	x = n.u8;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, uint16 &x )
{
	bw_netshort n;
	in >> n;
	x = n.u16;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, uint32 &x )
{
	bw_netlong n;
	in >> n;
	x = n.u32;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, uint64 &x )
{
	bw_netlonglong n;
	in >> n;
	x = n.u64;
	return in;
}

#if defined( PLAYSTATION3 )
inline BufferIStream& operator>>( BufferIStream &in, int8 &x )
{
	bw_netbyte n;
	in >> n;
	x = n.b;
	return in;
}
#endif

inline BufferIStream& operator>>( BufferIStream &in, int32 &x )
{
	bw_netlong n;
	in >> n;
	x = n.i32;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, int64 &x )
{
	bw_netlonglong n;
	in >> n;
	x = n.i64;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, char &x )
{
	bw_netbyte n;
	in >> n;
	x = n.c;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, bool &x )
{
	bw_netbyte n;
	in >> n;
	x = n.b;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, short &x )
{
	bw_netshort n;
	in >> n;
	x = n.u16;
	return in;
}


inline BufferIStream& operator>>( BufferIStream &in, float &x )
{
	bw_netlong n;
	in >> n;
	x = n.f;
	return in;
}

inline BufferIStream& operator>>( BufferIStream &in, double &x )
{
	bw_netlonglong n;
	in >> n;
	x = n.d;
	return in;
}

#ifndef _BIG_ENDIAN
template <class TYPE>
inline BufferIStream& operator>>( BufferIStream &is, TYPE &t )
{
	is.extractRaw( t );
	return is;
}
#endif


