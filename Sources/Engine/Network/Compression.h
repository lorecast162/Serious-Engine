/* Copyright (c) 2002-2012 Croteam Ltd. 
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#ifndef SE_INCL_COMPRESSION_H
#define SE_INCL_COMPRESSION_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

/*
 * Abstract base class for objects that can compress memory blocks.
 */
class CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory with given compression. */
  virtual long NeededDestinationSize(long slSourceSize) = 0;

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  virtual BOOL   Pack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize) = 0;
  /* Unpack a chunk of data using given compression. */
  virtual BOOL Unpack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize) = 0;

  /* Pack/unpack from stream to stream. */
  void UnpackStream_t(CTMemoryStream &strmSrc, CTStream &strmDst); // throw char *
  void PackStream_t(CTMemoryStream &strmSrc, CTStream &strmDst); // throw char *
};

/*
 * Compressor for compressing memory blocks using RLE BYTE-BYTE compression
 */
class CRLEBBCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  long NeededDestinationSize(long slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  BOOL   Pack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize);
  /* Unpack a chunk of data using given compression. */
  BOOL Unpack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize);
};


/*
 * Compressor for compressing memory blocks using LZ compression
 * (uses LZRW1 - a modification by Ross Williams)
 */
class CLZCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  long NeededDestinationSize(long slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  BOOL   Pack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize);
  /* Unpack a chunk of data using given compression. */
  BOOL Unpack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize);
};

/*
 * Compressor for compressing memory blocks using zlib compression
 * (zlib uses LZ77 - algorithm)
 */
class CzlibCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  long NeededDestinationSize(long slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  BOOL   Pack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize);
  /* Unpack a chunk of data using given compression. */
  BOOL Unpack(const void *pvSrc, long slSrcSize, void *pvDst, long &slDstSize);
};


#endif  /* include-once check. */

