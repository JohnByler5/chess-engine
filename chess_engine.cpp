#include <iostream>
#include <map>
#include <chrono>
#include <algorithm>
#include <fstream>
#include "chess.hpp"

using namespace chess;

std::map<PieceType, int> PieceValues = {
    {PieceType::PAWN, 100},
    {PieceType::KNIGHT, 300},
    {PieceType::BISHOP, 300},
    {PieceType::ROOK, 500},
    {PieceType::QUEEN, 900},
};

std::map<GameResult, int> ResultValues = {
    {GameResult::WIN, SHRT_MAX},
    {GameResult::LOSE, -SHRT_MAX},
    {GameResult::DRAW, 0},
};

int PawnTableMG[64] = {
      0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0,
};

int PawnTableEG[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0,
};

int KnightTableMG[64] = {
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23,
};

int KnightTableEG[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64,
};

int BishopTableMG[64] = {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21,
};

int BishopTableEG[64] = {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int RookTableMG[64] = {
     32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26,
};

int RookTableEG[64] = {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20,
};

int QueenTableMG[64] = {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50,
};

int QueenTableEG[64] = {
     -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41,
};

int KingTableMG[64] = {
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14,
};

int KingTableEG[64] = {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};

std::map<PieceType, int*> TableMG = {
    {PieceType::PAWN, PawnTableMG},
    {PieceType::KNIGHT, KnightTableMG},
    {PieceType::BISHOP, BishopTableMG},
    {PieceType::ROOK, RookTableMG},
    {PieceType::QUEEN, QueenTableMG},
    {PieceType::KING, KingTableMG},
};

std::map<PieceType, int*> TableEG = {
    {PieceType::PAWN, PawnTableEG},
    {PieceType::KNIGHT, KnightTableEG},
    {PieceType::BISHOP, BishopTableEG},
    {PieceType::ROOK, RookTableEG},
    {PieceType::QUEEN, QueenTableEG},
    {PieceType::KING, KingTableEG},
};

std::map<PieceType, int> GamePhaseTable = {
    {PieceType::PAWN, 0},
    {PieceType::KNIGHT, 1},
    {PieceType::BISHOP, 1},
    {PieceType::ROOK, 2},
    {PieceType::QUEEN, 4},
    {PieceType::KING, 0},
};

void initTables() {
    for (const std::pair<PieceType, int>& pair : PieceValues) {
        for (int square = 0; square < 64; ++square) {
            TableMG[pair.first][square] += pair.second;
            TableEG[pair.first][square] += pair.second;
        }
    }
}

struct SearchResult {
    Move move;
    int value;
};

class ChessEngine {
private:
    Board board;
    int mgValue;
    int egValue;
    std::ofstream logFile;
    
public:
    ChessEngine(const std::string& fen, std::string& logPath) : board(Board(fen)), mgValue(0), egValue(0), logFile(logPath) {
        init();
    }

    void init() {
        Color color = board.sideToMove();
        mgValue = 0;
        egValue = 0;
        for (int square = 0; square < 64; ++square) {
            Piece piece = board.at(Square(square));
            if (piece == Piece::NONE) {
                continue;
            }
            PieceType type = piece.type();

            if (piece.color() == color) {
                mgValue += TableMG[type][square];
                egValue += TableEG[type][square];
            } else {
                int flippedSquare = square ^ 56;
                mgValue -= TableMG[type][flippedSquare];
                egValue -= TableEG[type][flippedSquare];
            }
        }
    }

    Board getBoard() {
        return board;
    }

    int evaluate() {
        int value = 0;

        int gamePhase = 0;
        for (const std::pair<PieceType, int>& pair : PieceValues) {
            int count = board.pieces(pair.first).count();
            gamePhase += count * GamePhaseTable[pair.first];
        }

        if (gamePhase > 24) gamePhase = 24;  // In case of early promotion
        value += (mgValue * gamePhase + egValue * (24 - gamePhase)) / 24;

        return value;
    }

    void updateEvaluation(const Move& move) {
        Square from = move.from();
        Square to = move.to();
        PieceType type = board.at<PieceType>(from);
        Color color = !board.sideToMove();  // Flip color because piece square tables are flipped

        int mgUpdate = 0;
        int egUpdate = 0;

        from = from.relative_square(color);
        mgUpdate -= TableMG[type][from.index()];
        egUpdate -= TableEG[type][from.index()];

        if (board.isCapture(move)) {
            Square captureSquare;
            if (move.typeOf() == Move::ENPASSANT) captureSquare = board.enpassantSq();
            else captureSquare = to;
            PieceType captureType = board.at<PieceType>(captureSquare);
            captureSquare = captureSquare.relative_square(!color);  // Flip color because captured piece is the other color
            mgUpdate += TableMG[captureType][captureSquare.index()];
            egUpdate += TableEG[captureType][captureSquare.index()];
        }

        if (move.typeOf() == Move::PROMOTION) {
            to = to.relative_square(color);
            PieceType promotionType = move.promotionType();
            mgUpdate += TableMG[promotionType][to.index()];
            egUpdate += TableEG[promotionType][to.index()];

        } else if (move.typeOf() == Move::CASTLING) {
            bool kingSide = move.to() > move.from();
            Square kingTo = Square::castling_king_square(kingSide, color).relative_square(color);
            Square rookTo = Square::castling_rook_square(kingSide, color).relative_square(color);

            to = to.relative_square(color);

            mgUpdate -= TableMG[PieceType::ROOK][to.index()];
            egUpdate -= TableEG[PieceType::ROOK][to.index()];
            mgUpdate += TableMG[type][kingTo.index()];
            egUpdate += TableEG[type][kingTo.index()];
            mgUpdate += TableMG[PieceType::ROOK][rookTo.index()];
            egUpdate += TableEG[PieceType::ROOK][rookTo.index()];

        } else {
            to = to.relative_square(color);
            mgUpdate += TableMG[type][to.index()];
            egUpdate += TableEG[type][to.index()];
        }

        // Make values negative because of turn flip (negamax algorithm)
        mgValue = -(mgValue + mgUpdate);
        egValue = -(egValue + egUpdate);
    }

    void makeMove(const Move& move) {
        updateEvaluation(move);
        board.makeMove(move);
    }

    void unmakeMove(const Move& move) {
        board.unmakeMove(move);
        updateEvaluation(move);
    }

    void setFen(const std::string& fen) {
        board.setFen(fen);

        init();
    }

    int quiescent(int alpha, int beta) {
        // Check these draws before evaluate() as they should not be in that function and in this function
        if (board.isRepetition() || board.isHalfMoveDraw() || board.isInsufficientMaterial()) {
            return ResultValues[GameResult::DRAW];
        }

        int standPat = evaluate();
        if (standPat >= beta) return beta;
        if (alpha < standPat) alpha = standPat;

        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

        for (const Move& capture : captures) {
            if (board.isCapture(capture)) {
                makeMove(capture);
                int score = -quiescent(-beta, -alpha);
                unmakeMove(capture);

                if (score >= beta) return beta;
                if (score > alpha) alpha = score;
            }
        }
        return alpha;
    }
    
    Movelist sortMoves(Movelist& moves) {
        for (Move& move : moves) {
            uint16_t type = move.typeOf();
            int score = 0;
            if (type == Move::PROMOTION) score = 2 + int(move.promotionType());  // Will be either 3, 4, 5, or 6
            else if (board.isCapture(move)) score = 2;
            else if (type == Move::CASTLING) score = 1;
            move.setScore(score);
        }
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score() > b.score();
        });
        return moves;
    }

    int minimax(int depth, int alpha, int beta, float maxSeconds, std::chrono::time_point<std::chrono::system_clock>& startTime) {
        // Check these draws before quiescent() as they should not be both in evaluate function and in this function
        if (board.isRepetition() || board.isHalfMoveDraw() || board.isInsufficientMaterial()) {
            return ResultValues[GameResult::DRAW];
        }
        
        if (depth == 0) return evaluate();  // quiescent(alpha, beta);

        Movelist moves;
        movegen::legalmoves(moves, board);

        // Check these conditions after quiescent() in order to not search legalmoves within evaluate function
        if (moves.empty()) {
            if (board.inCheck()) return ResultValues[GameResult::LOSE];
            return ResultValues[GameResult::DRAW];
        }
        moves = sortMoves(moves);

        int bestValue = INT_MIN;
        for (const Move& move : moves) {
            makeMove(move);
            int value = -minimax(depth - 1, -beta, -alpha, maxSeconds, startTime);
            unmakeMove(move);

            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            float elapsedSeconds = std::chrono::duration<float>(currentTime - startTime).count();
            if (elapsedSeconds > maxSeconds) {
                return INT_MAX;
            }

            if (value > bestValue) bestValue = value;
            if (value > alpha) alpha = value;
            if (alpha >= beta) break;
        }

        return bestValue;
    }

    Movelist sortStartMoves(Movelist& moves, Move& bestPreviousMove) {
        for (Move& move : moves) {
            uint16_t type = move.typeOf();
            int score = 0;
            if (move == bestPreviousMove) score = 7;
            else if (type == Move::PROMOTION) score = 2 + int(move.promotionType());  // Will be either 3, 4, 5, or 6
            else if (board.isCapture(move)) score = 2;
            else if (type == Move::CASTLING) score = 1;
            move.setScore(score);
        }
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score() > b.score();
        });
        return moves;
    }

    SearchResult minimaxStart(int depth, float maxSeconds, std::chrono::time_point<std::chrono::system_clock>& startTime, Move& bestPreviousMove) {
        int alpha = INT_MIN + 1;
        int beta = INT_MAX;
        int bestValue = INT_MIN;
        Move bestMove = Move::NULL_MOVE;

        Movelist moves;
        movegen::legalmoves(moves, board);
        moves = sortStartMoves(moves, bestPreviousMove);
        
        for (const Move& move : moves) {
            makeMove(move);
            int value = -minimax(depth, -beta, -alpha, maxSeconds, startTime);
            unmakeMove(move);

            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            float elapsedSeconds = std::chrono::duration<float>(currentTime - startTime).count();
            if (elapsedSeconds > maxSeconds) {
                break;
            }

            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
            if (value > alpha) alpha = value;
            if (alpha >= beta) break;
        }

        return {bestMove, bestValue};
    }

    SearchResult bestMove(float maxSeconds) {
        logFile << "Max Seconds: " << maxSeconds << std::endl;

        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

        int depth = 0;
        SearchResult result = {Move::NULL_MOVE, INT_MIN};
        while (true) {
            SearchResult newResult = minimaxStart(depth, maxSeconds, startTime, result.move);
            if (newResult.move == Move::NULL_MOVE) break;
            result = newResult; 

            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            float elapsedSeconds = std::chrono::duration<float>(currentTime - startTime).count();
            logFile << "Depth: " << depth << " - Move: " << result.move << " - Value: " << result.value  << " - Elapsed: " << elapsedSeconds << std::endl;

            if (elapsedSeconds > maxSeconds) {
                break;
            }

            ++depth;
        }

        logFile << "Selected: " << result.move << " - " << result.value << std::endl;

        return result;
    }

    void uciLoop() {
        std::string line;
        std::string token;
        while (getline(std::cin, line)) {
            std::istringstream iss(line);
            iss >> token;

            if (token == "uci") handleUci();
            else if (token == "isready") handleIsReady();
            else if (token == "position") handlePosition(iss);
            else if (token == "go") handleGo(iss);
            else if (token == "ucinewgame") handleUciNewGame();
            else if (token == "quit") break;
        }
    }

    void handleUci() {
        std::cout << "id name ChessEngine-v0.1.0" << std::endl;
        std::cout << "id author John Byler" << std::endl;
        std::cout << "uciok" << std::endl;
    }

    void handleIsReady() {
        std::cout << "readyok" << std::endl;
    }

    void handlePosition(std::istringstream& iss) {
        std::string token, fen;
        iss >> token;

        if (token == "startpos") {
            fen = constants::STARTPOS;
            iss >> token;
        } else if (token == "fen") {
            while (iss >> token && token != "moves") {
                fen += token + " ";
            }
        }

        setFen(fen);

        while (iss >> token) {
            Move move = uci::uciToMove(board, token);
            if (move != Move::NULL_MOVE) makeMove(move);
        }
    }

    void handleGo(std::istringstream& iss) {
        float maxSeconds = 1.0;
        std::string param;
        while (iss >> param) {
            if (((param == "wtime") && (board.sideToMove() == Color::WHITE)) || ((param == "btime") && (board.sideToMove() == Color::BLACK))) {
                iss >> maxSeconds;
                maxSeconds /= 1000;  // Convert milliseconds to seconds
                maxSeconds /= 30;  // Assuming that 1/30th is a good portion of time to use
                break;
            }
        }

        SearchResult result = bestMove(maxSeconds);
        Move move = result.move;
        int score = result.value;
        std::cout << "bestmove " << uci::moveToUci(move) << " score cp " << score << std::endl;
    }

    void handleUciNewGame() {
        board = Board(constants::STARTPOS);
        mgValue = 0;
        egValue = 0;
    }
};

int main() {
    initTables(); 

    // std::string fen = constants::STARTPOS;
    std::string fen = "r3r1k1/1ppq1ppp/p1np1n2/2b1p3/2B1P1b1/P1PPBN2/1P1NQPPP/R4RK1 w - - 0 11";
    std::string logPath = "/Users/john/VS Code Projects/C++/chess-engine/log.txt";

    ChessEngine engine(fen, logPath);
    SearchResult result = engine.bestMove(1000);
    std::cout << "Selected: " << result.move << " - " << result.value << std::endl;

    // ChessEngine engine(fen, logPath);
    // engine.uciLoop();

    return 0;
}
