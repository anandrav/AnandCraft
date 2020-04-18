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

void Terrain::render_opaque(const Camera& camera) const {
    // render all chunks
    for_each(begin(chunks), end(chunks),
        [&camera](auto& it) {
            it.second->render_opaque(camera);
        }
    );
}

void Terrain::render_transparent(const Camera& camera) const {
    // render all chunks
    for_each(begin(chunks), end(chunks),
        [&camera](auto& it) {
            it.second->render_transparent(camera);
        }
    );
}