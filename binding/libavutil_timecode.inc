//
// avutil.h header binding for the Free Pascal Compiler aka FPC
//
// Binaries and demos available at http://www.djmaster.com/
//

(*
 * Copyright (c) 2006 Smartjog S.A.S, Baptiste Coudurier <baptiste.coudurier@gmail.com>
 * Copyright (c) 2011-2012 Smartjog S.A.S, Clément Bœsch <clement.boesch@smartjog.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *)

(**
 * @file
 * Timecode helpers header
 *)

// #ifndef AVUTIL_TIMECODE_H
// #define AVUTIL_TIMECODE_H

// #include <stdint.h>
// #include "rational.h"

const
  AV_TIMECODE_STR_SIZE = 23;

type
  AVTimecodeFlag = (
    AV_TIMECODE_FLAG_DROPFRAME = (1 shl 0), ///< timecode is drop frame
    AV_TIMECODE_FLAG_24HOURSMAX = (1 shl 1), ///< timecode wraps after 24 hours
    AV_TIMECODE_FLAG_ALLOWNEGATIVE = (1 shl 2) ///< negative time values are allowed
  );

  PAVTimecode = ^AVTimecode;
  AVTimecode = record
    start: cint; ///< timecode frame start (first base frame number)
    flags: cuint32; ///< flags such as drop frame, +24 hours support, ...
    rate: AVRational; ///< frame rate in rational form
    fps: cunsigned; ///< frame per second; must be consistent with the rate field
  end;

(**
 * Adjust frame number for NTSC drop frame time code.
 *
 * @param framenum frame number to adjust
 * @param fps      frame per second, 30 or 60
 * @return         adjusted frame number
 * @warning        adjustment is only valid in NTSC 29.97 and 59.94
 *)
function av_timecode_adjust_ntsc_framenum2(framenum: cint; fps: cint): cint; cdecl; external LIB_AVUTIL;

(**
 * Convert frame number to SMPTE 12M binary representation.
 *
 * @param tc       timecode data correctly initialized
 * @param framenum frame number
 * @return         the SMPTE binary representation
 *
 * @note Frame number adjustment is automatically done in case of drop timecode,
 *       you do NOT have to call av_timecode_adjust_ntsc_framenum2().
 * @note The frame number is relative to tc->start.
 * @note Color frame (CF), binary group flags (BGF) and biphase mark polarity
 *       correction (PC) bits are set to zero.
 *)
function av_timecode_get_smpte_from_framenum(const tc: PAVTimecode; framenum: cint): cuint32; cdecl; external LIB_AVUTIL;

(**
 * Load timecode string in buf.
 *
 * @param buf      destination buffer, must be at least AV_TIMECODE_STR_SIZE long
 * @param tc       timecode data correctly initialized
 * @param framenum frame number
 * @return         the buf parameter
 *
 * @note Timecode representation can be a negative timecode and have more than
 *       24 hours, but will only be honored if the flags are correctly set.
 * @note The frame number is relative to tc->start.
 *)
function av_timecode_make_string(const tc: PAVTimecode; buf: pchar; framenum: cint): pchar; cdecl; external LIB_AVUTIL;

(**
 * Get the timecode string from the SMPTE timecode format.
 *
 * @param buf        destination buffer, must be at least AV_TIMECODE_STR_SIZE long
 * @param tcsmpte    the 32-bit SMPTE timecode
 * @param prevent_df prevent the use of a drop flag when it is known the DF bit
 *                   is arbitrary
 * @return           the buf parameter
 *)
function av_timecode_make_smpte_tc_string(buf: pchar; tcsmpte: cuint32; prevent_df: cint): pchar; cdecl; external LIB_AVUTIL;

(**
 * Get the timecode string from the 25-bit timecode format (MPEG GOP format).
 *
 * @param buf     destination buffer, must be at least AV_TIMECODE_STR_SIZE long
 * @param tc25bit the 25-bits timecode
 * @return        the buf parameter
 *)
function av_timecode_make_mpeg_tc_string(buf: pchar; tc25bit: cuint32): pchar; cdecl; external LIB_AVUTIL;

(**
 * Init a timecode struct with the passed parameters.
 *
 * @param log_ctx     a pointer to an arbitrary struct of which the first field
 *                    is a pointer to an AVClass struct (used for av_log)
 * @param tc          pointer to an allocated AVTimecode
 * @param rate        frame rate in rational form
 * @param flags       miscellaneous flags such as drop frame, +24 hours, ...
 *                    (see AVTimecodeFlag)
 * @param frame_start the first frame number
 * @return            0 on success, AVERROR otherwise
 *)
function av_timecode_init(tc: PAVTimecode; rate: AVRational; flags: cint; frame_start: cint; log_ctx: pointer): cint; cdecl; external LIB_AVUTIL;

(**
 * Parse timecode representation (hh:mm:ss[:;.]ff).
 *
 * @param log_ctx a pointer to an arbitrary struct of which the first field is a
 *                pointer to an AVClass struct (used for av_log).
 * @param tc      pointer to an allocated AVTimecode
 * @param rate    frame rate in rational form
 * @param str     timecode string which will determine the frame start
 * @return        0 on success, AVERROR otherwise
 *)
function av_timecode_init_from_string(tc: PAVTimecode; rate: AVRational; const str: pchar; log_ctx: pointer): cint; cdecl; external LIB_AVUTIL;

(**
 * Check if the timecode feature is available for the given frame rate
 *
 * @return 0 if supported, <0 otherwise
 *)
function av_timecode_check_frame_rate(rate: AVRational): cint; cdecl; external LIB_AVUTIL;

// #endif (* AVUTIL_TIMECODE_H *)
