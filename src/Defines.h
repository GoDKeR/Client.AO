#pragma once

#include <vector>
#include <cstdint>
#include <wrl/client.h>
#include <memory>
#include <unordered_map>
#include <string>

#include <DirectXMath.h>

using Int8		= std::int8_t;
using Int16		= std::int16_t;
using Int32		= std::int32_t;
using Int64		= std::int64_t;
using Float		= float;
using Double	= double;

using UInt8		= std::uint8_t;
using UInt16	= std::uint16_t;
using UInt32	= std::uint32_t;

using Bool		= bool;

using String	= std::string;

template<typename T>
using Vector	= std::vector<T>;

using Vector2F	= DirectX::XMFLOAT2;
using Vector3F	= DirectX::XMFLOAT3;

using Matrix4F	= DirectX::XMMATRIX;

template<typename Key, typename T>
using UnorderedMap = std::unordered_map<Key, T>;

template<typename T>
using ComPtr	= Microsoft::WRL::ComPtr<T>;

template<typename T>
using UPtr		= std::unique_ptr<T>;
