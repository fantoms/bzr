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
#ifndef BZR_NET_BLOBASSEMBLER_H
#define BZR_NET_BLOBASSEMBLER_H

#include "Noncopyable.h"
#include <unordered_map>

const size_t kMaxFragmentSize = 448; // excludes header

PACK(struct FragmentHeader {
    uint64_t id;
    uint16_t count;
    uint16_t size;
    uint16_t index;
    uint16_t queueId;
});

struct Blob
{
    uint16_t count;
    uint16_t queueId;
    uint32_t fragmentsReceived;
    size_t size; // not set until last fragment by index is received
};

struct FreeDeleter
{
    void operator()(void* p) const
    {
        free(p);
    }
};

typedef unique_ptr<Blob, FreeDeleter> BlobPtr;

class BlobAssembler : Noncopyable
{
public:
    typedef vector<BlobPtr> container;

    void addFragment(const FragmentHeader* fragment);
    void getBlobs(container& blobs);

private:
    unordered_map<uint64_t, BlobPtr> partialBlobs_;
    container completeBlobs_;
};

#endif