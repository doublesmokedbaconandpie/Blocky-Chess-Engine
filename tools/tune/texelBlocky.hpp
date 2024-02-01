#include <array>
#include <map>
#include <string>
#include <vector>

#include "texel-tuner/src/base.h"
#include "texel-tuner/src/external/chess.hpp"
#include "../../src/eval.hpp"

namespace Blocky {

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
        static void pushEntry(parameters_t& parameters, Eval::S entry, const Eval::S adjustVal);

        static std::map<std::string, int> offsets;
        static std::map<std::string, int> sizes;
        static std::vector<std::string> tablesInOrder;
        static int totalSize;
    };

void printArr(const parameters_t& parameters, int offset, int size);
std::string toStr(pair_t val, int width);

}; // namespace Blocky