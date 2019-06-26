/**************************************************************************
 * FILE PURPOSE: Wrapper library for vxWorks tickLib
 **************************************************************************
 * FILE NAME: tickLib.h
 *
 * DESCRIPTION: This file has wrapper functions implementation for vxworks tickLib
 *
 * REVISION HISTORY:
 *
 * Date          Name                      Reason
 * ----          -------------             ----------
 * 12Mar2010     Pankaj Malhotra           Removed compilation errors.
 *
 * COPYRIGHT (c) 2009-2012 Hughes Network Systems, LLC
 **************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif
#ifndef _TICKLIB_H_
#define _TICKLIB_H_

#ifdef __cplusplus
extern "C"
{
#endif

void tickSet( unsigned long ticks);

unsigned long tickGet (void);
inline unsigned int get_time_in_msec();

#ifdef __cplusplus
}
#endif

#endif
#ifdef __cplusplus
}
#endif
