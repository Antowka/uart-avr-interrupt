/*! \file nmea.h \brief NMEA protocol function library. */
//*****************************************************************************
//
// File Name	: 'nmea.h'
// Title		: NMEA protocol function library
// Author		: Pascal Stang - Copyright (C) 2002
// Created		: 2002.08.27
// Revised		: 2002.08.27
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup driver_hw
/// \defgroup nmea NMEA Packet Interface for GPS Receivers (nmea.c)
/// \code #include "nmea.h" \endcode
/// \par Overview
///		This library parses and decodes the standard NMEA data stream from a
/// GPS and stores the position, velocity, and time solutions in the gps.c
/// library.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef NMEA_H
#define NMEA_H

void Parser(unsigned char data);
char *getDate();
#endif
