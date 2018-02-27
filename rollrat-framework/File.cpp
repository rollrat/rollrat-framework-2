//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  test.cpp - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "File.h"
#include <stdio.h>
#include "WString.h"

using namespace ofw;

///===-----------------------------------------------------------------------===
///
///               Folder Enumerator
///
///===-----------------------------------------------------------------------===

ofw::FolderEnumerator::FolderEnumerator(const WString& dir) {
  WString cpy;
  if (dir.EndsWith(L"\\"))
    cpy = WString::Concat(dir, L"*.*");
  else if (!dir.EndsWith(L"\\*.*"))
    cpy = WString::Concat(dir, L"\\*.*");
  path = WString(cpy.Substring(0, cpy.Length() - 4));
  handle = FindFirstFileW(cpy.Reference(), &current_data);
  if (handle == INVALID_HANDLE_VALUE) return;
  current_file = WString(current_data.cFileName);

  // Skip current or parents folder.
  do {
    if (!current_file.Equal(L"..") && !current_file.Equal(L".")) break;
  } while (NextFile());
}

ofw::FolderEnumerator::~FolderEnumerator() { FindClose(handle); }

bool ofw::FolderEnumerator::NextFile() {
  if (FindNextFileW(handle, &current_data) == FALSE) {
    FindClose(handle);
    handle = INVALID_HANDLE_VALUE;
    return false;
  }

  current_file = WString(current_data.cFileName);

  return true;
}

bool ofw::FolderEnumerator::NextFolder() {
  do {
    if (FindNextFileW(handle, &current_data) == FALSE) {
      FindClose(handle);
      handle = INVALID_HANDLE_VALUE;
      return false;
    }
  } while (!IsDirectory());
  return true;
}

bool ofw::FolderEnumerator::IsDirectory() {
  if (current_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    return true;
  }
  return false;
}

bool ofw::FolderEnumerator::IsFile() { return !IsDirectory(); }

WString ofw::FolderEnumerator::GetName() { return current_file; }

WString ofw::FolderEnumerator::GetFullName() {
  if (!path.EndsWith(L"\\")) {
    return WString::Concat(path, L"\\", current_data.cFileName);
  } else {
    return path.Append(current_data.cFileName);
  }
}

bool ofw::FolderEnumerator::IsValid() {
  return (handle != INVALID_HANDLE_VALUE);
}

///===-----------------------------------------------------------------------===
///
///               File Method
///
///===-----------------------------------------------------------------------===

bool ofw::File::Exist(const WString& path) {
  DWORD dwAttrib = GetFileAttributesW(path.Reference());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
          !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool ofw::File::Rename(const WString& oldname, const WString& newname) {
  int err;
  if (!_wrename(oldname.Reference(), newname.Reference())) return true;
  if (!MoveFileW(oldname.Reference(), newname.Reference())) return true;
  return false;
}

bool ofw::File::Move(const WString& oldpath, const WString& newpath) {
  if (!MoveFileW(oldpath.Reference(), newpath.Reference())) return true;
  return false;
}

bool ofw::File::Delete(const WString& path) {
  return DeleteFileW(path.Reference());
}
