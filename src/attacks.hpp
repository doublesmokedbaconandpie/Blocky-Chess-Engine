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
        static uint64_t findMagic(Function slidingAttacks, int square, uint64_t blockerMask, int shift, std::array<uint64_t, 4> seed);

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
0x648000e01484c001, 0x0040084030002000, 0x00800c1000200080, 0x2080100008028004,
0x02002810a4020020, 0x8100040081000218, 0x0080430002000280, 0x2200002400410082,
0x4000800020400086, 0x0e00400820005004, 0x00020010c2802200, 0x0802003040082200,
0x000a002012000418, 0x1402802c00520080, 0x0004000821243042, 0x0211800241000980,
0x0090808000314000, 0x0001030020804000, 0x1083820020320540, 0x48a4808048001000,
0x0088808044004800, 0x000400801a00800c, 0x4021040008023021, 0x22010e0000408114,
0x0000802080004005, 0x0100200340005000, 0x4000450100102000, 0xc001000900201000,
0x0201080080440080, 0x0204000202000810, 0x0010100400080106, 0x5402208200040041,
0x1c2d400078800180, 0x04c0010041002080, 0x1000811000806000, 0x4010802800801000,
0x000a002012000418, 0x0816011002002814, 0x8801080154001022, 0x0084041082000845,
0x0000a08040028000, 0x0602c02010004000, 0x0400820090460020, 0x01060041900a0020,
0x80030c0008008080, 0x000400801a00800c, 0x0040100291240048, 0x002100588c020009,
0x5802204080010100, 0x2001c00080200480, 0x00020010c2802200, 0x0802003040082200,
0x0201080080440080, 0x1402802c00520080, 0x001010518a080400, 0x1200140085204200,
0x0000410480611202, 0x0000410480611202, 0x0110104020000901, 0x1201013000142009,
0x9002001029042002, 0x1011000288440001, 0x002100588c020009, 0x0048044104008022,
};

constexpr std::array<uint64_t, BOARD_SIZE> BISHOP_MAGICS{
0x0820080220802201, 0x00104438048b6000, 0xa650018210422001, 0x0084041082000845,
0x200404201804304a, 0x0101010841049120, 0x0844008809188086, 0x8013008080884001,
0x0224c08421241100, 0x01000488021401e0, 0x1d00500080811102, 0x0c00440400800000,
0x8002440308080000, 0xa650018210422001, 0xa650018210422001, 0x1300090321012000,
0x028a600c88280800, 0x0030802006820a40, 0x0ad0040884820008, 0x0018400404040880,
0x410b000820080128, 0x140200a148060804, 0x2400800042082000, 0x4002085151040120,
0x00200800a1085984, 0x02c40a0320880100, 0x0008011008004100, 0x0000840008021020,
0x140100102300c001, 0x4005010122101280, 0x400404a008480440, 0x000101000c540180,
0x0002114000940801, 0x0002010400109000, 0x8000209000080920, 0x2300404800008200,
0x8248020400001010, 0x0004808200050120, 0x00060c1041030800, 0x0004808200050120,
0x008a08200c04080a, 0x0090445004270840, 0x0028084248001000, 0x0000024010424600,
0x000240010b000602, 0x19a008100c4004e0, 0x00200800a1085984, 0x08020a1410618100,
0x0844008809188086, 0x0101010841049120, 0x0512020084048040, 0x4000100020880190,
0x1001201012020220, 0x5400902141110108, 0x0241300502088020, 0x00104438048b6000,
0x8013008080884001, 0x1300090321012000, 0x0000044082080600, 0x8008220002104400,
0x0080240204104402, 0x0c8008c038010100, 0x0224c08421241100, 0x0820080220802201,
};