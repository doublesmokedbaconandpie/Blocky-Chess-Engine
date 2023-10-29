#include <array>
#include <cstdint>
#include <vector>

#include "types.hpp"

namespace Attacks {
    
struct Magic {
    uint64_t slideMask;
    uint64_t magic;
    int shift;
    int offset;
};

// used to return attack table values
uint64_t rookAttacks(int square, uint64_t allPieces);
uint64_t bishopAttacks(int square, uint64_t allPieces);
uint64_t pawnAttacks(int square, bool isWhiteTurn);
uint64_t knightAttacks(int square);
uint64_t kingAttacks(int square);

// initilizes all tables 
void init();
template <typename Function, size_t SIZE>
void initMagicTable(std::array<Magic, BOARD_SIZE>& table,
                    const std::array<uint64_t, BOARD_SIZE>& magicTable, 
                    std::array<uint64_t, SIZE>& attackTable,
                    Function getAttackMask, bool isBishop);
int getMagicIndex(Magic& entry, uint64_t blockers);

uint64_t getRelevantBlockerMask(int square, bool isBishop);
std::vector<uint64_t> getPossibleBlockers(uint64_t slideMask);

// bitboard functions
uint64_t computeRookAttacks(int square, uint64_t blockers);
uint64_t computeBishopAttacks(int square, uint64_t blockers);
uint64_t fillInDir(int square, uint64_t blockers, int x, int y);
uint64_t computeKnightAttacks(int square);
uint64_t computeKingAttacks(int square);
uint64_t computePawnAttacks(int square, bool isWhiteTurn);

// table declarations
extern std::array<Magic, BOARD_SIZE> ROOK_TABLE;
extern std::array<Magic, BOARD_SIZE> BISHOP_TABLE;
extern std::array<uint64_t, 102400> ROOK_ATTACKS;
extern std::array<uint64_t, 5248> BISHOP_ATTACKS;
extern std::array<std::array<uint64_t, BOARD_SIZE>, 2> PAWN_ATTACKS;
extern std::array<uint64_t, BOARD_SIZE> KNIGHT_ATTACKS;
extern std::array<uint64_t, BOARD_SIZE> KING_ATTACKS;

// see ../tools/magic.cpp for how these magic numbers were generated
constexpr std::array<uint64_t, BOARD_SIZE> ROOK_MAGICS{
0x648000e01484c001ull, 0x0040084030002000ull, 0x00800c1000200080ull, 0x2080100004800800ull,
0xc200201012000c08ull, 0x4200089430020015ull, 0x0200110084082200ull, 0x2100044320810002ull,
0x0204800028804000ull, 0x2003002184400102ull, 0xc080802000801000ull, 0x4022001042002048ull,
0xc202800800800400ull, 0x82120004184a0010ull, 0x200c000204100801ull, 0x88a2000c81040246ull,
0x2162888001400020ull, 0x00b0084000c06000ull, 0x0801010040102006ull, 0x200105001000a008ull,
0x000101000800100cull, 0x0001010048060400ull, 0x0412808011004600ull, 0x224002000a80410cull,
0x0000400880008061ull, 0x0440500440022002ull, 0x0400100080802000ull, 0x8021100180080080ull,
0x8114001480580080ull, 0x821e000404002010ull, 0x0200020400100188ull, 0x0202800080006300ull,
0x0000304000800080ull, 0x8000924000802001ull, 0x2015042001004092ull, 0x0001020961001002ull,
0x000200201a000410ull, 0x8042001066004c08ull, 0x1000800200800100ull, 0x2884800044800500ull,
0x000088400a608000ull, 0x900100c000910020ull, 0x0c20003045010020ull, 0x0009002030030008ull,
0x0008080004008080ull, 0x0081000214010008ull, 0x4010020001008080ull, 0x43000591006a0004ull,
0x80190080a20aca00ull, 0x2005200040008880ull, 0x8020413020820200ull, 0x0824820800100180ull,
0x4000080100041100ull, 0x1600040002008080ull, 0x0012090e08100400ull, 0x20020100440c8200ull,
0x000080210200401aull, 0x0a0200d100204082ull, 0x0008200441006811ull, 0x0400100104486101ull,
0x2001000800500403ull, 0x0032001018214c0aull, 0x0901000429820001ull, 0x000221228400c502ull,
};

constexpr std::array<uint64_t, BOARD_SIZE> BISHOP_MAGICS{
0x8021022206140410ull, 0x4048081082820020ull, 0x0404080600480000ull, 0x2084350600209001ull,
0xa40c042002000140ull, 0x0562021004000242ull, 0x0200880118200020ull, 0x0000110108224080ull,
0x040040148c840040ull, 0x0000080908020840ull, 0x2001130802004400ull, 0x0900682602408001ull,
0x80000a0210700040ull, 0x0402020104201800ull, 0x0401014108601000ull, 0x10a006a0880c9010ull,
0x041020c2c4580880ull, 0x802002041d040101ull, 0x0048001000819194ull, 0x1204010202360020ull,
0x2046004c12022001ull, 0x8202001100898c20ull, 0x8201000202922100ull, 0x1006005a43042560ull,
0x0820200028090104ull, 0x1948020018028804ull, 0x1204100a02008010ull, 0x804a00800800800aull,
0x0001010008504000ull, 0x01010f0000300800ull, 0x040085022a011000ull, 0x00060a0102410088ull,
0x0008048448401801ull, 0x2002100402060868ull, 0x8004040200012200ull, 0x0040100820040400ull,
0x2040002021120080ull, 0x6020008080010040ull, 0x0012440044010818ull, 0x940100ca11130100ull,
0x000b04024082e012ull, 0x000a0104200a2a00ull, 0x0180140601000800ull, 0x0018502011010812ull,
0x00000801040280c0ull, 0xc002889005000480ull, 0x4220044108409a04ull, 0x8008014042008880ull,
0x2411921030052208ull, 0x002044c404212140ull, 0x2260020500880140ull, 0x8400400042020300ull,
0x0780004410440420ull, 0x0000081001020004ull, 0x00082029440100a0ull, 0x482048021285a080ull,
0x0000c10818010402ull, 0x000002020a020a22ull, 0x8008122900882440ull, 0x8020010005148801ull,
0x6000000410020200ull, 0x11000020ac010202ull, 0x000108200410a204ull, 0x0010041104002201ull,
};

} // namespace Attacks