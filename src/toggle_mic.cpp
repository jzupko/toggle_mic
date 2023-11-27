// Copyright (c) 2023 Joseph Zupko <jazupko@jazupko.com>
//
// Unlicense: This is free and unencumbered software released into the
// public domain.

// Define TOGGLE_MIC_NO_CONSOLE to compile as a Windows subsystem on
// Windows (e.g. run `build.bat /DTOGGLE_MIC_NO_CONSOLE=1`)
//
// By default, compiled as a Console subsystem process, which
// is assumed to be the expected behavior. Windows subsystem is supported
// for integration into (e.g.) Stream Deck (as a GUI process with no
// window).
/* #define TOGGLE_MIC_NO_CONSOLE 1 */

#include <stdio.h>
#include "toggle_mic_windows.h"

static inline bool IsEol(char ch) {
	return ('\r' == ch || '\n' == ch);
}

// Convenience, human readable format an HRESULT code, stripping
// any trailing EOL markers.
static void FormatHr(char* hrbuf, DWORD hrbufSize, HRESULT hr) {
	DWORD res = ::FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		hr,
		0,
		hrbuf,
		hrbufSize,
		nullptr);
	if (0 == res) {
		hrbuf[0] = 0;
	} else {
		while (res > 0 && IsEol(hrbuf[res-1])) {
			--res;
			hrbuf[res] = 0;
		}
	}
}

// Format and output an error message.
static void Error(const char* file, int line, HRESULT hr, const char* msg) {
	char hrbuf[128] = { 0 };
	FormatHr(hrbuf, sizeof(hrbuf), hr);

	auto constexpr fmt = "%s(%d): %s (%s)\n";

#ifdef TOGGLE_MIC_NO_CONSOLE
	char buf[1024] = { 0 };
	if (_snprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, file, line, msg, hrbuf) < 0) {
		buf[0] = 0;
	}

	::OutputDebugStringA(buf);
#else
	fprintf(stderr, fmt, file, line, msg, hrbuf);
#endif
}

#define TOGGLE_MIC_ERR(hr, msg) Error(__FILE__, __LINE__, (hr), (msg))

// Utility to initialize COM for the process.
class ScopedCom final
{
public:
	ScopedCom() { ::CoInitialize(nullptr); }
	~ScopedCom() { ::CoUninitialize(); }

private:
	ScopedCom(const ScopedCom&) = delete;
	ScopedCom& operator=(const ScopedCom&) = delete;
};

// Utility to ensure COM pointers are released,
// leveraging RAII.
template <typename T>
class ScopedComPtr final
{
public:
	ScopedComPtr() = default;
	~ScopedComPtr() { Release(); }

	T** PtrForDeferredConstruct() {
		Release();
		return &m_p;
	}

	void Release() {
		auto p = m_p;
		m_p = nullptr;

		if (p) {
			p->Release();
		}
	}

	explicit operator bool() const { return (nullptr != m_p); }
	T&       operator*()     const { return *m_p; }
	T*       operator->()    const { return m_p; }

private:
	ScopedComPtr(const ScopedComPtr&) = delete;
	ScopedComPtr& operator=(const ScopedComPtr&) = delete;

	T* m_p = nullptr;
};

static bool ToggleDefaultMic() {
	// Enumerator for devices.
	ScopedComPtr<IMMDeviceEnumerator> enumerator;
	HRESULT hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(LPVOID*)enumerator.PtrForDeferredConstruct());
	if (FAILED(hr) || !enumerator) {
		TOGGLE_MIC_ERR(hr, "failed to create Multimedia device enumerator.");
		return false;
	}

	// Capture device.
	ScopedComPtr<IMMDevice> device;
	hr = enumerator->GetDefaultAudioEndpoint(
		eCapture,        // capture device
		eCommunications, // for communications
		device.PtrForDeferredConstruct());
	if (FAILED(hr) || !device) {
		TOGGLE_MIC_ERR(hr, "failed to acquire default Multimedia capture (communications) device.");
		return false;
	}

	// Volume endpoint.
	ScopedComPtr<IAudioEndpointVolume> vol;
	hr = device->Activate(
		__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER,
		nullptr,
		(void**)vol.PtrForDeferredConstruct());
	if (FAILED(hr) || !vol) {
		TOGGLE_MIC_ERR(hr, "failed to acquire volume endpoint from default capture (communications) device.");
		return false;
	}

	// Acquire current mute state.
	BOOL bMute = FALSE;
	hr = vol->GetMute(&bMute);
	if (FAILED(hr)) {
		TOGGLE_MIC_ERR(hr, "failed to get current mute state from volume endpoint.");
		return false;
	}

	// Toggle mute state.
	bMute = !bMute;
	hr = vol->SetMute(bMute, nullptr);
	if (FAILED(hr)) {
		TOGGLE_MIC_ERR(hr, "failed to set updated mute state to volume endpoint.");
		return false;
	}

	// Success.
	return true;
}

#ifdef TOGGLE_MIC_NO_CONSOLE
INT WINAPI wWinMain(
	HINSTANCE /*hInstance*/,
	HINSTANCE /*hPrevInstance*/,
	LPWSTR /*pCmdLine*/,
	INT /*nCmdShow*/) {
#else
int main(int /*argc*/, char** /*argv*/) {
#endif

	ScopedCom com;
	return ToggleDefaultMic() ? 0 : 1;
}
