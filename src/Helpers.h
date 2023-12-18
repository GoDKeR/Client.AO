#pragma once
#include <Windows.h> // For HRESULT

#include <exception>

// Helper utility converts D3D API failures into exceptions.
inline void ThrowIfFailed(HRESULT hr) noexcept(false)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}