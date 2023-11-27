// Copyright (c) 2023 Joseph Zupko <jazupko@jazupko.com>
//
// Unlicense: This is free and unencumbered software released into the
// public domain.

#pragma once
#ifndef TOGGLE_MIC_WINDOWS_H
#define TOGGLE_MIC_WINDOWS_H

#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOCOMM
#define NODEFERWINDOWPOS
#define NOGDI
#define NOGDICAPMASKS
#define NOHELP
#define NOICONS
#define NOKANJI
#define NOKERNEL
#define NOKEYSTATES
#define NOMB
#define NOMCX
#define NOMEMMGR
#define NOMENUS
#define NOMETAFILE
#define NOMINMAX
#define NONLS
#define NOOEMRESOURCE
#define NOOPENFILE
#define NOPROFILER
#define NORASTEROPS
#define NOSCROLL
#define NOSERVICE
#define NOSHOWWINDOW
#define NOSOUND
#define NOSYSCOMMANDS
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOVIRTUALKEYCODES
#define NOWH
#define NOWINMESSAGES
#define NOWINOFFSETS
#define NOWINSTYLES
#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0600 /* _WIN32_WINNT_VISTA */
#define _WIN32_WINNT WINVER
#pragma warning(push)
#pragma warning(disable:4255)
#pragma warning(disable:4668)
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#pragma warning(pop)

#endif
