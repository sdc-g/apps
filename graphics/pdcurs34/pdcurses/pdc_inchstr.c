/****************************************************************************
 * apps/graphics/pdcurses/pdc_inchstr.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Adapted from the original public domain pdcurses by Gregory Nutt
 ****************************************************************************/

/* Name: inchstr
 *
 * Synopsis:
 *       int inchstr(chtype *ch);
 *       int inchnstr(chtype *ch, int n);
 *       int winchstr(WINDOW *win, chtype *ch);
 *       int winchnstr(WINDOW *win, chtype *ch, int n);
 *       int mvinchstr(int y, int x, chtype *ch);
 *       int mvinchnstr(int y, int x, chtype *ch, int n);
 *       int mvwinchstr(WINDOW *, int y, int x, chtype *ch);
 *       int mvwinchnstr(WINDOW *, int y, int x, chtype *ch, int n);
 *
 *       int in_wchstr(cchar_t *wch);
 *       int in_wchnstr(cchar_t *wch, int n);
 *       int win_wchstr(WINDOW *win, cchar_t *wch);
 *       int win_wchnstr(WINDOW *win, cchar_t *wch, int n);
 *       int mvin_wchstr(int y, int x, cchar_t *wch);
 *       int mvin_wchnstr(int y, int x, cchar_t *wch, int n);
 *       int mvwin_wchstr(WINDOW *win, int y, int x, cchar_t *wch);
 *       int mvwin_wchnstr(WINDOW *win, int y, int x, cchar_t *wch, int n);
 *
 * Description:
 *       These routines read a chtype or cchar_t string from the window,
 *       starting at the current or specified position, and ending at the
 *       right margin, or after n elements, whichever is less.
 *
 * Return Value:
 *       All functions return the number of elements read, or ERR on
 *       error.
 *
 * Portability                                X/Open    BSD    SYS V
 *       inchstr                                 Y       -      4.0
 *       winchstr                                Y       -      4.0
 *       mvinchstr                               Y       -      4.0
 *       mvwinchstr                              Y       -      4.0
 *       inchnstr                                Y       -      4.0
 *       winchnstr                               Y       -      4.0
 *       mvinchnstr                              Y       -      4.0
 *       mvwinchnstr                             Y       -      4.0
 *       in_wchstr                               Y
 *       win_wchstr                              Y
 *       mvin_wchstr                             Y
 *       mvwin_wchstr                            Y
 *       in_wchnstr                              Y
 *       win_wchnstr                             Y
 *       mvin_wchnstr                            Y
 *       mvwin_wchnstr                           Y
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "curspriv.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int winchnstr(WINDOW *win, chtype *ch, int n)
{
  chtype *src;
  int i;

  PDC_LOG(("winchnstr() - called\n"));

  if (!win || !ch || n < 0)
    {
      return ERR;
    }

  if ((win->_curx + n) > win->_maxx)
    {
      n = win->_maxx - win->_curx;
    }

  src = win->_y[win->_cury] + win->_curx;

  for (i = 0; i < n; i++)
    {
      *ch++ = *src++;
    }

  *ch = (chtype)0;
  return OK;
}

int inchstr(chtype *ch)
{
#ifdef CONFIG_PDCURSES_MULTITHREAD
  FAR struct pdc_context_s *ctx = PDC_ctx();
#endif
  PDC_LOG(("inchstr() - called\n"));

  return winchnstr(stdscr, ch, stdscr->_maxx - stdscr->_curx);
}

int winchstr(WINDOW *win, chtype *ch)
{
  PDC_LOG(("winchstr() - called\n"));

  return winchnstr(win, ch, win->_maxx - win->_curx);
}

int mvinchstr(int y, int x, chtype *ch)
{
#ifdef CONFIG_PDCURSES_MULTITHREAD
  FAR struct pdc_context_s *ctx = PDC_ctx();
#endif
  PDC_LOG(("mvinchstr() - called: y %d x %d\n", y, x));

  if (move(y, x) == ERR)
    {
      return ERR;
    }

  return winchnstr(stdscr, ch, stdscr->_maxx - stdscr->_curx);
}

int mvwinchstr(WINDOW *win, int y, int x, chtype *ch)
{
  PDC_LOG(("mvwinchstr() - called:\n"));

  if (wmove(win, y, x) == ERR)
    {
      return ERR;
    }

  return winchnstr(win, ch, win->_maxx - win->_curx);
}

int inchnstr(chtype *ch, int n)
{
#ifdef CONFIG_PDCURSES_MULTITHREAD
  FAR struct pdc_context_s *ctx = PDC_ctx();
#endif
  PDC_LOG(("inchnstr() - called\n"));

  return winchnstr(stdscr, ch, n);
}

int mvinchnstr(int y, int x, chtype *ch, int n)
{
#ifdef CONFIG_PDCURSES_MULTITHREAD
  FAR struct pdc_context_s *ctx = PDC_ctx();
#endif
  PDC_LOG(("mvinchnstr() - called: y %d x %d n %d\n", y, x, n));

  if (move(y, x) == ERR)
    {
      return ERR;
    }

  return winchnstr(stdscr, ch, n);
}

int mvwinchnstr(WINDOW *win, int y, int x, chtype *ch, int n)
{
  PDC_LOG(("mvwinchnstr() - called: y %d x %d n %d\n", y, x, n));

  if (wmove(win, y, x) == ERR)
    {
      return ERR;
    }

  return winchnstr(win, ch, n);
}

#ifdef CONFIG_PDCURSES_WIDE
int win_wchnstr(WINDOW *win, cchar_t *wch, int n)
{
  PDC_LOG(("win_wchnstr() - called\n"));

  return winchnstr(win, wch, n);
}

int in_wchstr(cchar_t *wch)
{
  PDC_LOG(("in_wchstr() - called\n"));

  return win_wchnstr(stdscr, wch, stdscr->_maxx - stdscr->_curx);
}

int win_wchstr(WINDOW *win, cchar_t *wch)
{
  PDC_LOG(("win_wchstr() - called\n"));

  return win_wchnstr(win, wch, win->_maxx - win->_curx);
}

int mvin_wchstr(int y, int x, cchar_t *wch)
{
#ifdef CONFIG_PDCURSES_MULTITHREAD
  FAR struct pdc_context_s *ctx = PDC_ctx();
#endif
  PDC_LOG(("mvin_wchstr() - called: y %d x %d\n", y, x));

  if (move(y, x) == ERR)
    {
      return ERR;
    }

  return win_wchnstr(stdscr, wch, stdscr->_maxx - stdscr->_curx);
}

int mvwin_wchstr(WINDOW *win, int y, int x, cchar_t *wch)
{
  PDC_LOG(("mvwin_wchstr() - called:\n"));

  if (wmove(win, y, x) == ERR)
    {
      return ERR;
    }

  return win_wchnstr(win, wch, win->_maxx - win->_curx);
}

int in_wchnstr(cchar_t *wch, int n)
{
#ifdef CONFIG_PDCURSES_MULTITHREAD
  FAR struct pdc_context_s *ctx = PDC_ctx();
#endif
  PDC_LOG(("in_wchnstr() - called\n"));

  return win_wchnstr(stdscr, wch, n);
}

int mvin_wchnstr(int y, int x, cchar_t *wch, int n)
{
#ifdef CONFIG_PDCURSES_MULTITHREAD
  FAR struct pdc_context_s *ctx = PDC_ctx();
#endif
  PDC_LOG(("mvin_wchnstr() - called: y %d x %d n %d\n", y, x, n));

  if (move(y, x) == ERR)
    {
      return ERR;
    }

  return win_wchnstr(stdscr, wch, n);
}

int mvwin_wchnstr(WINDOW *win, int y, int x, cchar_t *wch, int n)
{
  PDC_LOG(("mvwinchnstr() - called: y %d x %d n %d\n", y, x, n));

  if (wmove(win, y, x) == ERR)
    {
      return ERR;
    }

  return win_wchnstr(win, wch, n);
}
#endif
