#include <array>
#include <map>
#include <string>
#include <vector>

#include "texel-tuner/src/base.h"
#include "texel-tuner/src/external/chess.hpp"
#include "../../src/eval.hpp"

namespace Blocky {

constexpr int PSQT_SIZE = NUM_PIECES * BOARD_SIZE;
constexpr int PIECE_VALS_SIZE = NUM_PIECES;
constexpr int PASSED_PAWNS_SIZE = NUM_RANKS;
constexpr int MOBILITY_SIZE = Eval::knightMobility.size() + Eval::bishopMobility.size() + Eval::rookMobility.size() + Eval::queenMobility.size();
constexpr int TOTAL_SIZE = PSQT_SIZE + PIECE_VALS_SIZE + PASSED_PAWNS_SIZE + MOBILITY_SIZE;

constexpr int PSQT_OFFSET = 0;
constexpr int PIECE_VALS_OFFSET = PSQT_OFFSET + PSQT_SIZE;
constexpr int PASSED_PAWNS_OFFSET = PIECE_VALS_OFFSET + PIECE_VALS_SIZE;
constexpr int MOBILITY_OFFSET = PASSED_PAWNS_OFFSET + PASSED_PAWNS_SIZE;
constexpr int KNIGHT_MOBILITY_OFFSET = MOBILITY_OFFSET;
constexpr int BISHOP_MOBILITY_OFFSET = KNIGHT_MOBILITY_OFFSET + Eval::knightMobility.size();
constexpr int ROOK_MOBILITY_OFFSET = BISHOP_MOBILITY_OFFSET + Eval::bishopMobility.size();
constexpr int QUEEN_MOBILITY_OFFSET = ROOK_MOBILITY_OFFSET + Eval::rookMobility.size();

// This is an interface for a texel-tuner used for the development for Blocky:
// https://github.com/GediminasMasaitis/texel-tuner
class BlockyEval
    {
    public:
        constexpr static bool includes_additional_score = false;
        constexpr static bool supports_external_chess_eval = false;

        static parameters_t get_initial_parameters();
        static EvalResult get_fen_eval_result(const std::string& fen);
        static EvalResult get_external_eval_result(const chess::Board& board); // unused
        static void print_parameters(const parameters_t& parameters);
    private: 
        template<size_t N>
        static void pushTable(parameters_t& parameters, std::string tableName, 
                                const std::array<Eval::S, N>& table, const Eval::S adjustVal = Eval::S{});
        static void pushSingleTerm(parameters_t& parameters, std::string termName, const Eval::S& term);
        static void pushEntry(parameters_t& parameters, Eval::S entry, const Eval::S adjustVal = Eval::S{});

        static std::map<std::string, int> offsets;
        static std::map<std::string, int> sizes;
        static std::vector<std::string> tablesInOrder;
        static int totalSize;
    };

void printArr(const parameters_t& parameters, int offset, int size);
std::string toStr(pair_t val, int width);

}; // namespace Blocky