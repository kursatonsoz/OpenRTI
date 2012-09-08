/* -*-c++-*- OpenRTI - Copyright (C) 2004-2012 Mathias Froehlich
 *
 * This file is part of OpenRTI.
 *
 * OpenRTI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * OpenRTI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenRTI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ThreadLocal.h"

#include <vector>
#include <Windows.h>

namespace OpenRTI {

AbstractThreadLocal::_AbstractData::~_AbstractData()
{
}

struct OPENRTI_LOCAL AbstractThreadLocal::_Provider {
  typedef std::vector<AbstractThreadLocal::_AbstractData*> ThreadLocalVector;

  _Provider() :
    _key(TlsAlloc()),
    _index(0)
  {
  }
  ~_Provider()
  {
    TlsFree(_key);
    _key = TLS_OUT_OF_INDEXES;
  }
  unsigned getNextIndex()
  {
    return _index++;
  }

  _AbstractData* getData(unsigned index)
  {
    ThreadLocalVector& tlsVector = _tlsVector();
    if (tlsVector.size() <= index)
      return 0;
    return tlsVector[index];
  }
  void setData(unsigned index, _AbstractData* abstractThreadLocal)
  {
    ThreadLocalVector& tlsVector = _tlsVector();
    if (tlsVector.size() <= index)
      tlsVector.resize(index + 1, 0);
    delete tlsVector[index];
    tlsVector[index] = abstractThreadLocal;
  }

  static void destructor(void* data)
  {
    if (!data)
      return;
    ThreadLocalVector* tlsVector = static_cast<ThreadLocalVector*>(data);
    for (ThreadLocalVector::iterator i = tlsVector->begin();
         i != tlsVector->end(); ++i)
      delete *i;
    delete tlsVector;
  }

  ThreadLocalVector& _tlsVector()
  {
    ThreadLocalVector* tlsVector;
    tlsVector = static_cast<ThreadLocalVector*>(TlsGetValue(_key));
    if (tlsVector)
      return *tlsVector;

    tlsVector = new ThreadLocalVector;
    TlsSetValue(_key, tlsVector);
    return *tlsVector;
  }

  DWORD _key;
  unsigned _index;
};

AbstractThreadLocal::AbstractThreadLocal() :
  _index(_provider().getNextIndex())
{
}

AbstractThreadLocal::~AbstractThreadLocal()
{
}


AbstractThreadLocal::_AbstractData*
AbstractThreadLocal::_get()
{
  return _provider().getData(_index);
}

void
AbstractThreadLocal::_set(AbstractThreadLocal::_AbstractData* abstractThreadLocal)
{
  return _provider().setData(_index, abstractThreadLocal);
}

AbstractThreadLocal::_Provider&
AbstractThreadLocal::_provider()
{
  static _Provider provider;
  return provider;
}

} // namespace OpenRTI
