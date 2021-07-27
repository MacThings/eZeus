#include "egameboard.h"

#include <random>

eGameBoard::eGameBoard() {

}

eGameBoard::eGameBoard(const eGameBoard& board) {
    const int w = board.mWidth;
    const int h = board.mHeight;
    mTiles.reserve(w);
    for(int x = 0; x < w; x++) {
        std::vector<eTile*> yArr;
        yArr.reserve(h);
        for(int y = 0; y < h; y++) {
            const auto tile = new eTile(*board.tile(x, y));
            yArr.push_back(tile);
        }
        mTiles.push_back(yArr);
    }
    mWidth = w;
    mHeight = h;

    updateDiagonalArray();
    updateNeighbours();
}

eGameBoard::~eGameBoard() {
    clear();
}

void eGameBoard::initialize(const int w, const int h) {
    clear();
    mTiles.reserve(w);
    for(int x = 0; x < w; x++) {
        std::vector<eTile*> yArr;
        yArr.reserve(h);
        for(int y = 0; y < h; y++) {
            const auto tile = new eTile(x, y);
            yArr.push_back(tile);
        }
        mTiles.push_back(yArr);
    }
    mWidth = w;
    mHeight = h;

    updateDiagonalArray();
    updateNeighbours();

    for(int i = 0; i < w + h; i++) {
        const int rr = rand() % 3;
        eTerrain type;
        if(rr == 0) {
            type = eTerrain::beach;
        } else if(rr == 1) {
            type = eTerrain::fertile;
        } else {
            type = eTerrain::river;
        }
        type = eTerrain::forest;
        const int x0 = rand() % w;
        const int y0 = rand() % h;
        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                const auto t = tile(x0 + j, y0 + k);
                if(!t) continue;
                t->setTerrain(type);
            }
        }
    }
    for(int i = 0; i < w + h; i++) {
        const int rr = rand() % 3;
        eTerrain type;
        double scrub;
        if(rr == 0) {
            scrub = 0.2;
        } else if(rr == 1) {
            scrub = 0.4;
        } else {
            scrub = 0.6;
        }
        type = eTerrain::forest;
        const int x0 = rand() % w;
        const int y0 = rand() % h;
        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                const auto t = tile(x0 + j, y0 + k);
                if(!t) continue;
                t->setScrub(scrub);
            }
        }
    }

}

void eGameBoard::clear() {
    for(const auto& x : mTiles) {
        for(const auto y : x) {
            delete y;
        }
    }
    mTiles.clear();
    mDiagTiles.clear();
    mWidth = 0;
    mHeight = 0;
}

eTile* eGameBoard::tile(const int x, const int y) const {
    if(x < 0 || x >= mWidth) return nullptr;
    if(y < 0 || y >= mHeight) return nullptr;
    return mTiles[x][y];
}

void eGameBoard::updateDiagonalArray() {
    for(int k = 0 ; k <= mWidth + mHeight - 2; k++) {
        std::vector<eTile*> diag;
        for(int j = 0 ; j <= k ; j++) {
            const int i = k - j;
            if(i < mWidth && j < mHeight) {
                diag.push_back(mTiles[i][j]);
            }
        }
        mDiagTiles.push_back(diag);
    }
}

void eGameBoard::updateNeighbours() {
    for(int x = 0; x < mWidth; x++) {
        for(int y = 0; y < mHeight; y++) {
            const auto t = mTiles[x][y];
            t->setTopLeft(tile(x - 1, y));
            t->setTopRight(tile(x, y - 1));
            t->setBottomRight(tile(x + 1, y));
            t->setBottomLeft(tile(x, y + 1));
        }
    }
}
