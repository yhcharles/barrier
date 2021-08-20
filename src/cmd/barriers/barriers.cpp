/*
 * barrier -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2002 Chris Schoeneman
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "arch/Arch.h"
#include "barrier/ServerApp.h"
#include "base/EventQueue.h"
#include "base/Log.h"

#if WINAPI_MSWINDOWS
#include "MSWindowsServerTaskBarReceiver.h"
#elif WINAPI_XWINDOWS
#include "XWindowsServerTaskBarReceiver.h"
#elif WINAPI_CARBON
#include "OSXServerTaskBarReceiver.h"
#else
#error Platform not supported.
#endif

int main(int argc, char** argv) {
#if SYSAPI_WIN32
  // record window instance for tray icon, etc
  ArchMiscWindows::setInstanceWin32(GetModuleHandle(NULL));
#endif

#ifdef __APPLE__
  /* Silence "is calling TIS/TSM in non-main thread environment" as it is a red
  herring that causes a lot of issues to be filed for the MacOS client/server.
  */
  setenv("OS_ACTIVITY_DT_MODE", "NO", true);
#endif

  Arch arch;
  arch.init();

  Log log;
  EventQueue events;

  ServerApp app(&events, createTaskBarReceiver);
  int result = app.run(argc, argv);
#if SYSAPI_WIN32
  if (IsDebuggerPresent()) {
    printf("\n\nHit a key to close...\n");
    getchar();
  }
#endif
  return result;
}
