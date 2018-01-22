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
#include "WString.h"

using namespace ofw;

///===-----------------------------------------------------------------------===
///
///               Folder Enumerator
///
///===-----------------------------------------------------------------------===

ofw::FolderEnumerator::FolderEnumerator(const WString & dir)
{
  WString cpy;
  if (dir.EndsWith(L"\\"))
    cpy = WString::Concat(dir, L"*.*");
  else if (!dir.EndsWith(L"\\*.*"))
    cpy = WString::Concat(dir, L"\\*.*");
  path = WString(cpy.Substring(0, cpy.Length() - 4));
  handle = FindFirstFileW(cpy.Reference(), &current_data);
  if (handle == INVALID_HANDLE_VALUE)
    return;
  current_file = WString(current_data.cFileName);

  // Skup current or parents folder.
  do {
    if (!current_file.Equal(L"..") || !current_file.Equal(L"."))
      break;
  } while (NextFile());
}

ofw::FolderEnumerator::~FolderEnumerator()
{
  FindClose(handle);
}

bool ofw::FolderEnumerator::NextFile()
{
  if (FindNextFileW(handle, &current_data) == FALSE)
  {
    FindClose(handle);
    handle = INVALID_HANDLE_VALUE;
    return false;
  }

  current_file = WString(current_data.cFileName);

  return false;
}

bool ofw::FolderEnumerator::IsDirectory()
{
  if (current_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {
    return true;
  }
  return false;
}

bool ofw::FolderEnumerator::IsFile()
{
  return !IsDirectory();
}

WString ofw::FolderEnumerator::GetName()
{
  return current_file;
}

WString ofw::FolderEnumerator::GetFullName()
{
  if (!path.EndsWith(L"\\"))
  {
    return WString::Concat(path, L"\\", current_data.cFileName);
  }
  else
  {
    return path.Append(current_data.cFileName);
  }
}

bool ofw::FolderEnumerator::IsValid()
{
  return ( handle != INVALID_HANDLE_VALUE );
}

///===-----------------------------------------------------------------------===
///
///               File Enumerator
///
///===-----------------------------------------------------------------------===

ofw::FileEnumerator::FileEnumerator(FileEnumerator & fe)
{
}

ofw::FileEnumerator::FileEnumerator(const WString & dir)
{
}

ofw::FileEnumerator::~FileEnumerator()
{
}

bool ofw::FileEnumerator::NextFile()
{
  return false;
}

bool ofw::FileEnumerator::IsDirectory()
{
  return false;
}

bool ofw::FileEnumerator::IsFile()
{
  return false;
}

WString ofw::FileEnumerator::GetFullName()
{
  return WString();
}

WString ofw::FileEnumerator::GetName()
{
  return WString();
}

bool ofw::FileEnumerator::IsValid()
{
  return false;
}

uint64_t ofw::FileEnumerator::GetFileSize() const
{
  return uint64_t();
}
