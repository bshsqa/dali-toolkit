/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// INTERNAL
#include "dali-scene-loader/public-api/utils.h"

// EXTERNAL
#include "dali/public-api/common/vector-wrapper.h"
#include "dali/public-api/animation/constraints.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdarg.h>

namespace Dali
{
namespace SceneLoader
{
namespace
{
thread_local char sExceptionFlingerMessageBuffer[ExceptionFlinger::MESSAGE_BUFFER_SIZE]{};
}

char* ExceptionFlinger::GetMessageBuffer() noexcept(true)
{
  return sExceptionFlingerMessageBuffer;
}

std::string FormatString(const char* format, ...)
{
  va_list vl;
  va_start(vl, format);
  va_list vl2;
  va_copy(vl2, vl);

  size_t sizeRequired = vsnprintf(nullptr, 0, format, vl);
  va_end(vl);

  ++sizeRequired;
  std::string result(sizeRequired, '\0');
  vsnprintf(&result[0], sizeRequired, format, vl2);
  va_end(vl2);

  return result;
}

std::string LoadTextFile(const char * path, bool* fail)
{
  std::ifstream inFile(path);
  if (inFile)
  {
    std::istreambuf_iterator<char> eos;
    std::istreambuf_iterator<char> i(inFile.rdbuf());
    return std::string(i, eos);
  }
  else if (fail)
  {
    *fail = true;
  }
  return std::string();
}

Geometry MakeTexturedQuadGeometry(TexturedQuadOptions::Type options)
{
  Property::Map properties;
  properties.Insert("aPosition", Property::VECTOR3);
  properties.Insert("aTexCoord", Property::VECTOR2);

  std::vector<uint8_t> bytes;
  size_t stride = 0;
  size_t uvOffset = 0;
  struct
  {
    Vector3 aPosition;
    Vector2 aTexCoord;
  } vertices[] = {
    { Vector3(-0.5f, 0.5f, 0.0f), Vector2(0.0f, .0f) },
    { Vector3(0.5f, 0.5f, 0.0f), Vector2(1.0f, .0f) },
    { Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f) },
    { Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f) }
  };

  bytes.resize(sizeof(vertices));
  stride = sizeof(vertices[0]);
  uvOffset = reinterpret_cast<const uint8_t*>(&vertices[0].aTexCoord) - reinterpret_cast<const uint8_t*>(&vertices[0]);

  std::memcpy(bytes.data(), vertices, sizeof(vertices));

  if (MaskMatch(options, TexturedQuadOptions::FLIP_VERTICAL))
  {
    Vector2* uv = reinterpret_cast<Vector2*>(reinterpret_cast<uint8_t*>(bytes.data()) + uvOffset);
    for (int i = 0; i < 4; ++i)
    {
      uv->y = 1.0f - uv->y;
      uv = reinterpret_cast<Vector2*>(reinterpret_cast<uint8_t*>(uv) + stride);
    }
  }

  VertexBuffer vertexBuffer = VertexBuffer::New(properties);
  vertexBuffer.SetData(bytes.data(), bytes.size() / stride);

  Geometry geometry = Geometry::New();
  geometry.AddVertexBuffer(vertexBuffer);
  geometry.SetType(Geometry::TRIANGLE_STRIP);
  return geometry;
}

void ToUnixFileSeparators(std::string& path)
{
  std::replace(path.begin(), path.end(), '\\', '/');
}

}
}
