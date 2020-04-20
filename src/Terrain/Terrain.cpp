#include "Terrain.h"

using namespace std;

Terrain::Terrain() {
    ChunkCoords coords{0,-1,0};
    chunks[coords] = make_shared<Chunk>(coords);
    coords.x = -1;
    chunks[coords] = make_shared<Chunk>(coords);
    coords.z = -1;
    chunks[coords] = make_shared<Chunk>(coords);
    coords.x = 0;
    chunks[coords] = make_shared<Chunk>(coords);
    for_each(begin(chunks), end(chunks),
        [](auto& it) {
            it.second->update_meshes();
        }
    );
}