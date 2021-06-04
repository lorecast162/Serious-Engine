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

#ifndef SE_INCL_EMSGBUFFER_H
#define SE_INCL_EMSGBUFFER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif


#include <Engine/Math/Placement.h>
#include "Buffer.h"

#define EM_SIZE 256
#define MAX_TICKS_KEPT 20 // data for how many ticks will be accumulated before an error is reported


// max 31 type of messages - stored in the lowest 4 bits of the em_uwType word 
// upper 12 bits contain event code, for create and copy messages
#define EMT_CREATE					1 
#define	EMT_COPY						2 
#define	EMT_SETPLACEMENT		3 
#define	EMT_DESTROY					4 
#define	EMT_EVENT						5 

#define	EMT_NUMBEROFTYPES		5	//	how many different types of messages are there

#define EMB_SUCCESS_OK			        0
#define EMB_ERR_MAX_TICKS		        1
#define EMB_ERR_NOT_IN_BUFFER       2
#define EMB_ERR_TICK_NOT_COMPLETE   4
#define EMB_ERR_BUFFER_TOO_SMALL    5
#define EMB_ERR_BUFFER_EMPTY        6



class CEntityMessage {
public:
	unsigned long em_ulType;
	UBYTE em_aubMessage[EM_SIZE];
	UBYTE em_ubSize;
	unsigned long em_ulEntityID;

	CEntityMessage(){};
	~CEntityMessage(){};

	void WritePlacement(unsigned long &ulEntityID,CPlacement3D &plPlacement);
	void ReadPlacement(unsigned long &ulEntityID,CPlacement3D &plPlacement);

	void WriteEntityCreate(unsigned long &ulEntityID,CPlacement3D &plPlacement,UWORD &uwEntityClassID,UWORD &uwEventCode,void* pvEventData,UWORD &uwDataSize);
	void ReadEntityCreate(unsigned long &ulEntityID,CPlacement3D &plPlacement,UWORD &uwEntityClassID,UWORD &uwEventCode,void* pvEventData,UWORD &uwDataSize);

	void WriteEntityCopy(unsigned long &ulSourceEntityID,unsigned long &ulTargetEntityID,CPlacement3D &plPlacement,UWORD &uwEventCode,void* pvEventData,UWORD &uwDataSize);
	void ReadEntityCopy(unsigned long &ulSourceEntityID,unsigned long &ulTargetEntityID,CPlacement3D &plPlacement,UWORD &uwEventCode,void* pvEventData,UWORD &uwDataSize);

	void WriteEntityDestroy(unsigned long &ulEntityID);
	void ReadEntityDestroy(unsigned long &ulEntityID);

	void WriteEntityEvent(unsigned long &ulEntityID,UWORD &uwEventCode,void* pvEventData,UWORD &uwDataSize);
	void ReadEntityEvent(unsigned long &ulEntityID,UWORD &uwEventCode,void* pvEventData,UWORD &uwDataSize);

};



struct TickMarker {
	float tm_fTickTime;
	long tm_slTickOffset;
	UBYTE tm_ubAcknowledgesExpected;
  UWORD tm_uwNumMessages;

  TickMarker() {
    tm_fTickTime = -1;
    tm_slTickOffset = -1;
    tm_ubAcknowledgesExpected = 0;
    tm_uwNumMessages = 0;
  }
};


// Entity message buffer - stores entity messages - create, copy, destroy, set placement...
class CEMsgBuffer : public CBuffer{
public:
	CBuffer emb_bufMsgBuffer;				// a buffer to store entity messages in
	TickMarker	emb_atmTickMarkers[MAX_TICKS_KEPT];		// pointers into the buffer, they point to the first message for eachtick
	UWORD		emb_uwNumTickMarkers;
	INDEX		emb_iFirstTickMarker;
  INDEX		emb_iCurrentTickMarker;
  float   emb_fCurrentTickTime;

	CEMsgBuffer();
	~CEMsgBuffer();

  virtual void Clear(void);

  // expand buffer to be given number of bytes in size
  virtual void Expand(long slNewSize);

	// write one block
  virtual void WriteBytes(const void *pv, long slSize);
	long PeekBytes(const void *pv, long slSize);
  long PeekBytesAtOffset(const void *pv, long slSize,long &slTickOffset);

  int StartNewTick(float fTickTime);
  int SetCurrentTick(float fTickTime);
  int GetTickIndex(float fTickTime,INDEX &iTickIndex);
  int GetTickOffset(float fTickTime,long &slTickOffset);
  int GetNextTickTime(float fTickTime,float &fNextTickTime);
	int RequestTickAcknowledge(float fTickTime,UBYTE ubNumAcknowledges);
	int ReceiveTickAcknowledge(float fTickTime);
  int MoveToStartOfTick(float fTickTime);
  

	// write the message from _emEntityMessage to the buffer
	void WriteMessage(CEntityMessage &emEntityMessage);
	int  ReadMessage(CEntityMessage &emEntityMessage);
  int  PeekMessageAtOffset(CEntityMessage &emEntityMessage,long &slTickOffset);

  int  ReadTick(float fTickTime,const void *pv, long &slSize);
  void WriteTick(float fTickTime,const void *pv, long slSize);


};

#endif
