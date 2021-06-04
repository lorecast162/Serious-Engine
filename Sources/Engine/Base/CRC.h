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

#ifndef SE_INCL_CRC_H
#define SE_INCL_CRC_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

extern ENGINE_API unsigned long crc_aulCRCTable[256];

// begin crc calculation
inline void CRC_Start(unsigned long &ulCRC) { ulCRC = 0xFFFFFFFF; };

// add data to a crc value
inline void CRC_AddBYTE( unsigned long &ulCRC, UBYTE ub)
{
  ulCRC = (ulCRC>>8)^crc_aulCRCTable[UBYTE(ulCRC)^ub];
};

inline void CRC_AddWORD( unsigned long &ulCRC, UWORD uw)
{
  CRC_AddBYTE(ulCRC, UBYTE(uw>> 8));
  CRC_AddBYTE(ulCRC, UBYTE(uw>> 0));
};

inline void CRC_AddLONG( unsigned long &ulCRC, unsigned long ul)
{
  CRC_AddBYTE(ulCRC, UBYTE(ul>>24));
  CRC_AddBYTE(ulCRC, UBYTE(ul>>16));
  CRC_AddBYTE(ulCRC, UBYTE(ul>> 8));
  CRC_AddBYTE(ulCRC, UBYTE(ul>> 0));
};

inline void CRC_AddLONGLONG( unsigned long &ulCRC, uint64_t x)
{
  CRC_AddBYTE(ulCRC, UBYTE(x>>56));
  CRC_AddBYTE(ulCRC, UBYTE(x>>48));
  CRC_AddBYTE(ulCRC, UBYTE(x>>40));
  CRC_AddBYTE(ulCRC, UBYTE(x>>32));
  CRC_AddBYTE(ulCRC, UBYTE(x>>24));
  CRC_AddBYTE(ulCRC, UBYTE(x>>16));
  CRC_AddBYTE(ulCRC, UBYTE(x>> 8));
  CRC_AddBYTE(ulCRC, UBYTE(x>> 0));
}

inline void CRC_AddFLOAT(unsigned long &ulCRC, FLOAT f)
{
  CRC_AddLONG(ulCRC, *(unsigned long*)&f);
};

// add memory block to a CRC value
inline void CRC_AddBlock(unsigned long &ulCRC, UBYTE *pubBlock, unsigned long ulSize)
{
  for( INDEX i=0; (unsigned long)i<ulSize; i++) CRC_AddBYTE( ulCRC, pubBlock[i]);
};

// end crc calculation
inline void CRC_Finish(unsigned long &ulCRC) { ulCRC ^= 0xFFFFFFFF; };

// in 32bit mode, it just returns iPtr unsigned long,
// in 64bit mode it returns the CRC hash of iPtr (or 0 if ptr == NULL)
// so either way you should get a value that very likely uniquely identifies the pointer
inline unsigned long IntPtrToID(size_t iPtr)
{
#if PLATFORM_32BIT
  return (unsigned long)iPtr;
#else
  // in case the code relies on 0 having special meaning because of NULL-pointers...
  if(iPtr == 0)  return 0;
  unsigned long ret;
  CRC_Start(ret);
  CRC_AddLONGLONG(ret, iPtr);
  CRC_Finish(ret);
  return ret;
#endif
}

// in 32bit mode, it just returns the pointer's address as unsigned long,
// in 64bit mode it returns the CRC hash of the pointer's address (or 0 if ptr == NULL)
// so either way you should get a value that very likely uniquely identifies the pointer
inline unsigned long PointerToID(void* ptr)
{
#if PLATFORM_32BIT
  return (unsigned long)(size_t)ptr;
#else
  return IntPtrToID((size_t)ptr);
#endif
}

#endif  /* include-once check. */

