#include "ChunkSystem.h"

#include <functional>
#include <cassert>
#include "../Async/SyncQueue.h"
#include "../Async/ThreadQueue.h"
#include "../util.h"

using namespace std;

size_t ChunkSystem::ChunkCoordsHash::operator() (const ChunkCoords& coords) const
{
    size_t h1 = hash<int>()(coords.x);
    size_t h2 = hash<int>()(coords.y);
    size_t h3 = hash<int>()(coords.z);

    return h1 ^ (h2 << 1) ^ (h3 << 2);
}

void ChunkSystem::create_chunk(ChunkCoords coords)
{
    chunks.insert({ coords, ChunkComponents{coords} });


}

void ChunkSystem::remove_chunk(ChunkCoords coords)
{
    chunks.erase(coords);
}

size_t ChunkSystem::num_chunks() const
{
    return chunks.size();
}

void ChunkSystem::generate_chunk_data(ChunkCoords coords)
{
    auto data = new ChunkData;
    // half dirt, 1 layer of grass, the rest is air
    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int z = 0; z < CHUNK_WIDTH; ++z) {
            int y = 0;
            for (; y < CHUNK_WIDTH / 2; ++y) {
                (*data)[x][y][z] = BlockData(BlockID::DIRT);
            }
            (*data)[x][y][z] = BlockData(BlockID::GRASS);
            ++y;
            for (; y < CHUNK_WIDTH; ++y) {
                (*data)[x][y][z] = BlockData(BlockID::AIR);
            }
        }
    }

    SyncQueue::get_instance().push(
        // // chunk data is captured by move to prevent unnecessary copying
        [this, coords, data ]() mutable
        {
            // auto lookup = chunks.find(coords);
            // if (lookup == chunks.end()) {
            //     // chunk doesn't exist anymore
            //     delete data;
            //     return;
            // }
            // ChunkComponents& components = lookup->second;
            // components.chunk_data = data;

            // PaddedChunkData* padded_data = make_padded_chunk_data(components);

            // using namespace placeholders;
            // ThreadQueue::get_instance().push(
            //     bind(&ChunkSystem::generate_chunk_meshes, this, coords, padded_data),
            //     ThreadQueue::Priority::NORMAL
            // );
        }
    );
}

PaddedChunkData* ChunkSystem::make_padded_chunk_data(const ChunkComponents& components) const
{
    PaddedChunkData* padded = new PaddedChunkData;

    // // copy data from current chunk to padded datas
    // for (size_t x = 0; x < CHUNK_WIDTH; ++x) {
    //     for (size_t y = 0; y < CHUNK_WIDTH; ++y) {
    //         for (size_t z = 0; z < CHUNK_WIDTH; ++z) {
    //             // increment each coordinate by 1 to account for padding
    //             (*padded)[x + 1][y + 1][z + 1] = (*components.chunk_data)[x][y][z];
    //         }
    //     }
    // }

    // // copy data from adjacent chunks, if it exists
    // static const CubeFace faces[] = {
    //     CubeFace::XNEG,
    //     CubeFace::XPOS,
    //     CubeFace::YNEG,
    //     CubeFace::YPOS,
    //     CubeFace::ZNEG,
    //     CubeFace::ZPOS
    // };
    // for (auto face : faces) {
    //     copy_data_from_adjacent_chunk(padded, components.coords, face);
    // }

    return padded;
}

void ChunkSystem::copy_data_from_adjacent_chunk(PaddedChunkData* padded, ChunkCoords coords, CubeFace face) const
{
    // parameters for looping through appropriate chunk data
    // depending on which face is passed, we set the parameters to iterate over a slice
    //      of one of the adjacent chunks.
    // this helps us avoid hard-coding a case for each CubeFace!
    int x_start, x_end, y_start, y_end, z_start, z_end, x_adj_sign, y_adj_sign, z_adj_sign;
    // by default, we assume that block data from 0 to CHUNK_WIDTH is copied for each axis
    x_start = y_start = z_start = 0;
    x_end = y_end = z_end = CHUNK_WIDTH;
    // a sign of -1 is used to indicate if a coordinate is to be flipped when reading the adjacent chunk's data
    // by default, we assume that sign is 1 (not flipped)
    // multiplication is faster than branch statements when we loop through data
    x_adj_sign = y_adj_sign = z_adj_sign = 1;

    switch (face) {
    case CubeFace::XNEG:
        // since we are reading chunk data from the "XNEG" face,
        //      we must flip the x coordinate when reading from the adjacent chunk's data
        x_adj_sign = -1;
        // the coordinates of the "XNEG" adjacent chunk are the same, but x is one less
        --coords.x;
        // write to the "left-most" block data in the padded chunk (on the x-axis)
        // this will be flipped when we read from the adjacent chunk's data
        x_start = 0;
        x_end = 1;
        break;
    case CubeFace::XPOS:
        // again, since we are reading chunk data from the "XPOS" face,
        //      the x coordinate when reading from the adjacent chunk's data is flipped
        x_adj_sign = -1;
        // self-explanatory
        ++coords.x;
        // x coordinates start at (CHUNK_WIDTH - 1) and end at (CHUNK_WIDTH).
        // aka, the "right-most" block data (on the x-axis)
        x_start = CHUNK_WIDTH - 1;
        break;
    case CubeFace::YNEG:
        y_adj_sign = -1;
        --coords.y;
        y_start = 0;
        y_end = 1;
        break;
    case CubeFace::YPOS:
        y_adj_sign = -1;
        ++coords.y;
        y_start = CHUNK_WIDTH - 1;
        break;
    case CubeFace::ZNEG:
        z_adj_sign = -1;
        --coords.z;
        z_start = 0;
        z_end = 1;
        break;
    case CubeFace::ZPOS:
        z_adj_sign = -1;
        ++coords.z;
        z_start = CHUNK_WIDTH - 1;
        break;
    }

    auto lookup = chunks.find(coords);
    if (lookup == chunks.end()) {
        // chunk does not exist, assume adjacent blocks are air blocks. This will force blocks
        //      on the edge of the current chunk to show their faces when a mesh is created
        return;
    }

    const ChunkComponents& adjacent = lookup->second;
    // this acts like a double-nested for loop, since one of the for loops will only have one iteration.
    // for (int x = x_start; x < x_end; ++x) {
    //     for (int y = y_start; y < y_end; ++y) {
    //         for (int z = z_start; z < z_end; ++z) {
    //             // one of the coordinates from adjacent chunk must be flipped across its axis
    //             // for instance, if we are reading blocks from the XNEG chunk, x must be flipped
    //             //      must be flipped
    //             int x_adj = util::positive_modulo(x * x_adj_sign, CHUNK_WIDTH);
    //             int y_adj = util::positive_modulo(y * y_adj_sign, CHUNK_WIDTH);
    //             int z_adj = util::positive_modulo(z * z_adj_sign, CHUNK_WIDTH);

    //             // increment each coordinate by 1 to account for padding
    //             (*padded)[x + 1][y + 1][z + 1] = (*adjacent.chunk_data)[x_adj][y_adj][z_adj];
    //         }
    //     }
    // }
}

void ChunkSystem::generate_chunk_meshes(ChunkCoords coords, const PaddedChunkData* const data)
{

}

void ChunkSystem::render_opaque(const Camera& camera) const {

}

void ChunkSystem::render_transparent(const Camera& camera) const {

}