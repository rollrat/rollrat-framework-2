//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  File.h - 01/22/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _FILE_
#define _FILE_

#include "Defines.h"

// This class operates only in Windows environments.
#ifdef _OS_WINDOWS

#include <Windows.h>
#include <Shlwapi.h>
#include <memory>
#include <functional>

#include "WString.h"

namespace ofw
{

class FolderEnumerator
{
  HANDLE handle;
  WString current_file;
  WString path;
  WIN32_FIND_DATAW current_data;

public:
  FolderEnumerator(const WString& dir);
  ~FolderEnumerator();

  bool NextFile();
  bool IsDirectory();
  bool IsFile();
  WString GetName();
  WString GetFullName();
  bool IsValid();
};

class FileEnumerator
{
  FolderEnumerator *folder_enum;
  FileEnumerator *prev;
  WString path;
  bool into;
  bool top;
  FileEnumerator(FileEnumerator& fe);

public:
  FileEnumerator(const WString& dir);
  ~FileEnumerator();

  bool NextFile();
  bool IsDirectory();
  bool IsFile();
  WString GetFullName();
  WString GetName();
  bool IsValid();
  uint64_t GetFileSize() const;

};

}
#endif

#endif