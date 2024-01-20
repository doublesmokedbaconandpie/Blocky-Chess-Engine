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

        template<typename T>
        static void pushTable(parameters_t& parameters, T& table);
        static void pushEntry(parameters_t& parameters, Eval::S entry);
    };

void printPSQT(const parameters_t& parameters);
void printArr(const parameters_t& parameters, int offset, int size);
void printCoeff(const parameters_t& parameters); // for debugging

std::string toStr(pair_t val, int width);

}; // namespace Blocky