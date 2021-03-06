/*
 * Bael'Zharon's Respite
 * Copyright (C) 2014 Daniel Skorupski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "resource/PhysicsScript.h"
#include "resource/AnimationHook.h"
#include "BinReader.h"

void read(BinReader& reader, PhysicsScriptData& scriptData)
{
    scriptData.startTime = reader.readDouble();

    read(reader, scriptData.hook);
}

PhysicsScriptData::PhysicsScriptData()
{}

PhysicsScriptData::PhysicsScriptData(PhysicsScriptData&& other)
{
    startTime = other.startTime;
    hook = move(other.hook);
}

PhysicsScript::PhysicsScript(uint32_t id, const void* data, size_t size) : ResourceImpl{id}
{
    BinReader reader(data, size);

    uint32_t resourceId = reader.readInt();
    assert(resourceId == id);
    UNUSED(resourceId);

    uint32_t numHooks = reader.readInt();
    hooks.resize(numHooks);

    for(PhysicsScriptData& hook : hooks)
    {
        read(reader, hook);
    }

    assert(reader.remaining() == 0);
}

PhysicsScript::~PhysicsScript()
{}
