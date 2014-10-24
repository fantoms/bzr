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
#include "net/BlobHandler.h"
#include "Core.h"
#include "Log.h"
#include "BinReader.h"

enum MessageType
{
    kWEENIE_ORDERED = 0xF7B0,
    kORDERED = 0xF7B1
};

BlobHandler::BlobHandler() : sequence_(1)
{}

void BlobHandler::handle(BlobPtr blob)
{
    BinReader reader(blob.get() + 1, blob->size);

    uint32_t messageType = reader.readInt();

    if(messageType == kWEENIE_ORDERED)
    {
        /*objectId*/ reader.readInt();
        uint32_t messageSequence = reader.readInt();

        orderedBlobs_[messageSequence] = move(blob);
        pumpOrderedBlobs();
    }
    else if(messageType == kORDERED)
    {
        // the server doesn't send these
        throw runtime_error("unexpected ORDERED blob");
    }
    else
    {
        handle(messageType, reader);
    }
}

void BlobHandler::pumpOrderedBlobs()
{
    for(auto it = orderedBlobs_.begin(); it != orderedBlobs_.end(); it = orderedBlobs_.erase(it))
    {
        if(it->first > sequence_)
        {
            break;
        }

        BinReader reader(it->second.get() + 1, it->second->size);

        /*messageType*/ reader.readInt();
        /*objectId*/ reader.readInt();
        /*messageSequence*/ reader.readInt();
        uint32_t actualMessageType = reader.readInt();

        handle(actualMessageType, reader);
        sequence_++;
    }
}

void BlobHandler::handle(uint32_t messageType, BinReader& reader)
{
    LOG(Net, Info) << "received blob " << hexn(messageType) << "\n";

    (void)reader;
}