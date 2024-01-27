#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "types.hpp"

class Attacks {
    public:
        struct Magic {
            uint64_t slideMask;
            uint64_t magic;
            int shift;
            int offset;
        };
        static void init();
        static void generateMagics(); // not used to generate attacks

        // used to return attack table values
        static uint64_t rookAttacks(int square, uint64_t allPieces);
        static uint64_t bishopAttacks(int square, uint64_t allPieces);
        static uint64_t pawnAttacks(int square, bool isWhiteTurn);
        static uint64_t knightAttacks(int square);
        static uint64_t kingAttacks(int square);
    private:
        static int getMagicIndex(Magic& entry, uint64_t blockers);
        // not used to generate attacks
        template<typename Function>
        static uint64_t findMagic(Function slidingAttacks, int square, uint64_t blockerMask, int shift, std::array<uint64_t, 4>& seed);

        // used to initialize magic tables
        template <typename Function, size_t SIZE>
        static void initMagicTable(std::array<Magic, BOARD_SIZE>& table,
                            const std::array<uint64_t, BOARD_SIZE>& magicTable,
                            std::array<uint64_t, SIZE>& attackTable,
                            Function getAttackMask, bool isBishop);
        static uint64_t getRelevantBlockerMask(int square, bool isBishop);
        static std::vector<uint64_t> getPossibleBlockers(uint64_t slideMask);

        // bitboard functions
        static uint64_t computeRookAttacks(int square, uint64_t blockers);
        static uint64_t computeBishopAttacks(int square, uint64_t blockers);
        static uint64_t fillInDir(int square, uint64_t blockers, int x, int y);
        static uint64_t computeKnightAttacks(int square);
        static uint64_t computeKingAttacks(int square);
        static uint64_t computePawnAttacks(int square, bool isWhiteTurn);

        // global attack tables
        static std::array<Magic, BOARD_SIZE> ROOK_TABLE;
        static std::array<Magic, BOARD_SIZE> BISHOP_TABLE;
        static std::array<uint64_t, 102400> ROOK_ATTACKS;
        static std::array<uint64_t, 5248> BISHOP_ATTACKS;
        static std::array<std::array<uint64_t, BOARD_SIZE>, 2> PAWN_ATTACKS;
        static std::array<uint64_t, BOARD_SIZE> KNIGHT_ATTACKS;
        static std::array<uint64_t, BOARD_SIZE> KING_ATTACKS;
};

constexpr std::array<uint64_t, BOARD_SIZE> ROOK_MAGICS{
0x648000e01484c001, 0x0040084030002000, 0x00800c1000200080, 0x2080100004800800,
0xc200201012000c08, 0x4200089430020015, 0x0200110084082200, 0x2100044320810002,
0x0204800028804000, 0x2003002184400102, 0xc080802000801000, 0x4022001042002048,
0xc202800800800400, 0x82120004184a0010, 0x200c000204100801, 0x88a2000c81040246,
0x2162888001400020, 0x00b0084000c06000, 0x0801010040102006, 0x200105001000a008,
0x000101000800100c, 0x0001010048060400, 0x0412808011004600, 0x224002000a80410c,
0x0000400880008061, 0x0440500440022002, 0x0400100080802000, 0x8021100180080080,
0x8114001480580080, 0x821e000404002010, 0x0200020400100188, 0x0202800080006300,
0x0000304000800080, 0x8000924000802001, 0x2015042001004092, 0x0001020961001002,
0x000200201a000410, 0x8042001066004c08, 0x1000800200800100, 0x2884800044800500,
0x000088400a608000, 0x900100c000910020, 0x0c20003045010020, 0x0009002030030008,
0x0008080004008080, 0x0081000214010008, 0x4010020001008080, 0x43000591006a0004,
0x80190080a20aca00, 0x2005200040008880, 0x8020413020820200, 0x0824820800100180,
0x4000080100041100, 0x1600040002008080, 0x0012090e08100400, 0x20020100440c8200,
0x000080210200401a, 0x0a0200d100204082, 0x0008200441006811, 0x0400100104486101,
0x2001000800500403, 0x0032001018214c0a, 0x0901000429820001, 0x000221228400c502,
};

constexpr std::array<uint64_t, BOARD_SIZE> BISHOP_MAGICS{
0x8021022206140410, 0x4048081082820020, 0x0404080600480000, 0x2084350600209001,
0xa40c042002000140, 0x0562021004000242, 0x0200880118200020, 0x0000110108224080,
0x040040148c840040, 0x0000080908020840, 0x2001130802004400, 0x0900682602408001,
0x80000a0210700040, 0x0402020104201800, 0x0401014108601000, 0x10a006a0880c9010,
0x041020c2c4580880, 0x802002041d040101, 0x0048001000819194, 0x1204010202360020,
0x2046004c12022001, 0x8202001100898c20, 0x8201000202922100, 0x1006005a43042560,
0x0820200028090104, 0x1948020018028804, 0x1204100a02008010, 0x804a00800800800a,
0x0001010008504000, 0x01010f0000300800, 0x040085022a011000, 0x00060a0102410088,
0x0008048448401801, 0x2002100402060868, 0x8004040200012200, 0x0040100820040400,
0x2040002021120080, 0x6020008080010040, 0x0012440044010818, 0x940100ca11130100,
0x000b04024082e012, 0x000a0104200a2a00, 0x0180140601000800, 0x0018502011010812,
0x00000801040280c0, 0xc002889005000480, 0x4220044108409a04, 0x8008014042008880,
0x2411921030052208, 0x002044c404212140, 0x2260020500880140, 0x8400400042020300,
0x0780004410440420, 0x0000081001020004, 0x00082029440100a0, 0x482048021285a080,
0x0000c10818010402, 0x000002020a020a22, 0x8008122900882440, 0x8020010005148801,
0x6000000410020200, 0x11000020ac010202, 0x000108200410a204, 0x0010041104002201,
};