//******************************************************************************
//*
//*    Blackfin/bfin-elf support library
//*    
//*    Copyright (c) 2016, Harry E. Zhurov
//*
//******************************************************************************

#ifndef MACRO_H
#define MACRO_H

#include <stdint.h>

#define MMR16(x) ( *((uint16_t*)x) )
#define MMR32(x) ( *((uint32_t*)x) )

//------------------------------------------------------------------------------
#endif  /* MACRO_H */
