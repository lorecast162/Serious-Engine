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

#ifndef SE_INCL_BUFFER_H
#define SE_INCL_BUFFER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Timer.h>

/*
 * Class for buffering data streams at byte level.
 */
class CBuffer {
public:
  long bu_slAllocationStep;  // how many bytes to add when buffer overflows
  long bu_slWriteOffset; // where to write to next
  long bu_slReadOffset;  // where to read from next

  long bu_slFree;        // number of bytes free
  long bu_slSize;        // current buffer size
  UBYTE *bu_pubBuffer;    // buffer memory

  // default constructor
  CBuffer(void);
  // destructor
  ~CBuffer(void);
  // free buffer
  void Clear(void);
  // expand buffer to be given number of bytes in size
  void Expand(long slNewSize);
  // set how many bytes to add when buffer overflows
  void SetAllocationStep(long slStep);
  // read bytes from buffer
  long ReadBytes(void *pv, long slSize);
  // skip bytes from buffer (read without actually reading)
  long SkipBytes(long slSize);
  // read bytes from buffer to stream
  long ReadBytesToStream(CTStream &strm, long slSize);
  // unread bytes from buffer
  void UnreadBytes(long slSize);
  // check how many bytes are there to read
  long QueryReadBytes(void);
  // write bytes to buffer
  void WriteBytes(const void *pv, long slSize);
  // move all data from another buffer to this one
  void MoveBuffer(CBuffer &buFrom);
};

// data for limiting bandwidth/lantency and calculating statistics in block-buffers
class CBlockBufferStats {
public:
  FLOAT bbs_fLatencyLimit;    // minimum latency in seconds
  FLOAT bbs_fLatencyVariation;// additional latency variation
  FLOAT bbs_fBandwidthLimit;  // maximum bandwidth in bps (bits per second)
  CTimerValue bbs_tvTimeUsed; // next point in time free for data receiving

  void Clear(void);
  // get time when block of given size will be finished if started now
  CTimerValue GetBlockFinalTime(long slSize);
};

/*
 * Class for buffering data streams at block level.
 */
class CBlockBuffer : public CBuffer {
public:
  CBlockBufferStats *bb_pbbsStats;   // for bandwidth/latency stats and limits
  long bb_slBlockSizeRead;     // block size left for reading (0 if not inside block)
  long bb_slBlockSizeWrite;    // block size left for writing (0 if not inside block)

  // default constructor
  CBlockBuffer(void);
  // destructor
  ~CBlockBuffer(void);
  // free buffer
  void Clear(void);

  // read one block if possible
  BOOL ReadBlock(void *pv, long &slSize);
  // read one block from buffer to stream
  BOOL ReadBlockToStream(CTStream &strm);
  // write one block
  void WriteBlock(const void *pv, long slSize);
  // unread one block
  void UnreadBlock(long slSize);

  // read raw block data
  long ReadRawBlock(void *pv, long slSize);
  // write raw block data
  void WriteRawBlock(const void *pv, long slSize);
  // unread raw block data
  void UnreadRawBlock(long slSize);
  // move all data from another buffer to this one
  void MoveBlockBuffer(CBlockBuffer &buFrom);
  // peek sizes of next block
  void PeekBlockSize(long &slExpectedSize, long &slReceivedSoFar);
};


#endif  /* include-once check. */

