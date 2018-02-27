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

#ifndef _FILE_9bf1541fdf7efd41b7b39543fd870ac4_
#define _FILE_9bf1541fdf7efd41b7b39543fd870ac4_

#include "Defines.h"

// This class operates only in Windows environments.
#ifdef _OS_WINDOWS

#include <Shlwapi.h>
#include <Windows.h>
#include <functional>
#include <memory>

#include "WString.h"

namespace ofw {

class FolderEnumerator {
  HANDLE handle;
  WString current_file;
  WString path;
  WIN32_FIND_DATAW current_data;

 public:
  FolderEnumerator(const WString& dir);
  ~FolderEnumerator();

  bool NextFile();
  bool NextFolder();
  bool IsDirectory();
  bool IsFile();
  WString GetName();
  WString GetFullName();
  bool IsValid();
};

class File {
 public:
  static bool Exist(const WString& path);
  static bool Rename(const WString& oldname, const WString& newname);
  static bool Move(const WString& oldpath, const WString& newpath);
  static bool Delete(const WString& path);
};

}  // namespace ofw
#endif

#endif