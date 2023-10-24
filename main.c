/*
  ===========================================================================

  Copyright (C) 2019 Emvivre

  This file is part of HELLO_WORLD.

  HELLO_WORLD is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  HELLO_WORLD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with HELLO_WORLD.  If not, see <http://www.gnu.org/licenses/>.

  ===========================================================================
*/

#include <efi.h>
#include <efilib.h>


EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{

  InitializeLib(ImageHandle, SystemTable);
  Print(L"Hello, world1!\n");
   EFI_STATUS status;

    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

  status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
  if(EFI_ERROR(status)){
    Print(L"Unable to locate GOP v1");
  } else {
    Print(L"Hello, world! locate gop v1 \n");
      EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, numModes, nativeMode;
    
    status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
    // this is needed to get the current video mode
    if (status == EFI_NOT_STARTED)
      status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
    if(EFI_ERROR(status)) {
      Print(L"Unable to get native mode. will exit");
    } else {
      nativeMode = gop->Mode->Mode;
      numModes = gop->Mode->MaxMode;
    
    
      Print(L"numModes , %d\n",1,numModes);
      Print(L"Hello, world2!\n");
    
      for (int i = 0; i < numModes; i++) {
        status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
        Print(L"mode %03d width %d height %d format %x%s;",
          i,
          info->HorizontalResolution,
          info->VerticalResolution,
          info->PixelFormat,
          i == nativeMode ? "(current)" : ""
        );
      }
    }
  }

  Pause();
  return EFI_SUCCESS;
}
