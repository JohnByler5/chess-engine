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
    int score;
    bool complete;

    SearchResult()
        : move(Move::NULL_MOVE), score(INT_MIN + 1), complete(true) {
    }

    SearchResult(Move move, int score, bool complete=true)
        : move(move), score(score), complete(complete) {
    }
};

enum NodeType {
    NONE,
    ALL,
    CUT,
    PV,
};

struct TranspositionEntry {
    uint64_t hashKey;
    uint16_t depth;
    int alpha;
    int beta;
    NodeType nodeType;
    SearchResult result;

    TranspositionEntry() 
        : hashKey(0), depth(0), alpha(0), beta(0), nodeType(NodeType::NONE), result() {
    }

    TranspositionEntry(uint64_t hashKey, uint16_t depth, int alpha, int beta) 
        : hashKey(hashKey), depth(depth), alpha(alpha), beta(beta), nodeType(NodeType::ALL), result() {
    }
};

const size_t TABLE_SIZE = 1024 * 1024 * 1024 / sizeof(TranspositionEntry);  // 1GB of memory space, should be 32 bytes per entry

class TranspositionTable {
private:
    TranspositionEntry* table;
    size_t tableSize;

public:
    TranspositionTable(size_t size) : tableSize(size) {
        table = new TranspositionEntry[tableSize]();
    }

    ~TranspositionTable() {
        delete[] table;
    }

    void set(const TranspositionEntry& entry) {
        table[entry.hashKey % tableSize] = entry;
    }

    TranspositionEntry get(uint64_t hashKey) {
        TranspositionEntry entry = table[hashKey % tableSize];
        if (entry.hashKey == hashKey) return entry;
        return TranspositionEntry();
    }
};


class ChessEngine {
private:
    Board board;
    int mgValue;
    int egValue;
    std::ofstream logFile;
    TranspositionTable transpositionTable;
    
public:
    ChessEngine(const std::string& fen, std::string& logPath) : board(Board(fen)), mgValue(0), egValue(0), logFile(logPath), transpositionTable(TABLE_SIZE) {
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
        int score = 0;

        int gamePhase = 0;
        for (const std::pair<PieceType, int>& pair : PieceValues) {
            int count = board.pieces(pair.first).count();
            gamePhase += count * GamePhaseTable[pair.first];
        }

        if (gamePhase > 24) gamePhase = 24;  // In case of early promotion
        score += (mgValue * gamePhase + egValue * (24 - gamePhase)) / 24;

        return score;
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

    Movelist sortMoves(Movelist& moves, Move& previousBestMove) {
        for (Move& move : moves) {
            uint16_t type = move.typeOf();
            int score = 0;
            if (move == previousBestMove) score = 1500;
            else if (type == Move::PROMOTION) score = 1000 + int(move.promotionType() * 100);  // Will be 1100 through 1400
            else if (type == Move::ENPASSANT) score = 200;
            else if (board.isCapture(move)) score = 100 + PieceValues[board.at<PieceType>(move.to())];  // Will be 200 through 1000
            else if (type == Move::CASTLING) score = 100;
            move.setScore(score);
        }
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score() > b.score();
        });
        return moves;
    }

    SearchResult quiescent(int alpha, int beta) {
        uint64_t hashKey = board.hash();
        TranspositionEntry foundEntry = transpositionTable.get(hashKey);
        if (foundEntry.nodeType != NodeType::NONE && foundEntry.alpha <= alpha && foundEntry.beta >= beta) {
            return foundEntry.result;
        }
        TranspositionEntry entry = {hashKey, 0, alpha, beta};

        if (board.isRepetition() || board.isHalfMoveDraw() || board.isInsufficientMaterial()) {
            entry.result.score = ResultValues[GameResult::DRAW];
            transpositionTable.set(entry);
            return entry.result;
        }

        int standPat = evaluate();
        if (standPat > entry.result.score) entry.result.score = standPat;
        if (standPat > alpha) alpha = standPat;
        if (alpha >= beta) return entry.result;

        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);
        captures = sortMoves(captures, foundEntry.result.move);

        for (const Move& capture : captures) {
            if (board.isCapture(capture)) {
                makeMove(capture);
                SearchResult result = quiescent(-beta, -alpha);
                int score = -result.score;
                unmakeMove(capture);

                if (score > entry.result.score) {
                    entry.result.score = score;
                    entry.result.move = capture;
                }
                if (score > alpha) alpha = score;
                if (alpha >= beta) {
                    entry.nodeType = NodeType::CUT;
                    break;
                }
            }
        }

        transpositionTable.set(entry);
        return entry.result;
    }

    SearchResult minimax(uint16_t depth, int alpha, int beta, float maxSeconds, std::chrono::time_point<std::chrono::system_clock>& startTime) {
        if (depth == 0) return quiescent(alpha, beta);

        uint64_t hashKey = board.hash();
        TranspositionEntry foundEntry = transpositionTable.get(hashKey);
        if (foundEntry.nodeType != NodeType::NONE && foundEntry.depth >= depth && foundEntry.alpha <= alpha && foundEntry.beta >= beta) {
            return foundEntry.result;
        }
        TranspositionEntry entry = {hashKey, depth, alpha, beta};

        if (board.isRepetition() || board.isHalfMoveDraw() || board.isInsufficientMaterial()) {
            entry.result.score = ResultValues[GameResult::DRAW];
            transpositionTable.set(entry);
            return entry.result;
        }

        Movelist moves;
        movegen::legalmoves(moves, board);
        if (moves.empty()) {
            if (board.inCheck()) entry.result.score = ResultValues[GameResult::LOSE];
            else entry.result.score = ResultValues[GameResult::DRAW];
            transpositionTable.set(entry);
        }
        moves = sortMoves(moves, foundEntry.result.move);

        for (const Move& move : moves) {
            makeMove(move);
            SearchResult result = minimax(depth - 1, -beta, -alpha, maxSeconds, startTime);
            unmakeMove(move);

            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            float elapsedSeconds = std::chrono::duration<float>(currentTime - startTime).count();
            if (elapsedSeconds > maxSeconds || result.complete == false) {
                entry.result.complete = false;
                return entry.result;
            }

            int score = -result.score;
            if (score > entry.result.score) {
                entry.result.score = score;
                entry.result.move = move;
            }
            if (score > alpha) {
                alpha = score;
                entry.nodeType = NodeType::PV;
            }
            if (alpha >= beta) {
                entry.nodeType = NodeType::CUT;
                break;
            }
        }

        transpositionTable.set(entry);
        return entry.result;
    }

    SearchResult bestMove(float maxSeconds) {
        logFile << "Max Seconds: " << maxSeconds << std::endl;

        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

        uint16_t depth = 0;
        SearchResult result = {Move::NULL_MOVE, INT_MIN};
        while (true) {
            SearchResult newResult = minimax(depth, INT_MIN + 1, INT_MAX, maxSeconds, startTime);
            if (newResult.complete == false) break;
            result = newResult; 

            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            float elapsedSeconds = std::chrono::duration<float>(currentTime - startTime).count();
            logFile << "Depth: " << depth << " - Move: " << result.move << " - Score: " << result.score  << " - Elapsed: " << elapsedSeconds << std::endl;

            if (elapsedSeconds > maxSeconds) {
                break;
            }

            ++depth;
        }

        logFile << "Selected: " << result.move << " - " << result.score << std::endl;

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
        std::cout << "id name ChessEngine-v0.2.1" << std::endl;
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
        int score = result.score;
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
    
    std::string fen = constants::STARTPOS;
    // std::string fen = "r3r1k1/1ppq1ppp/p1np1n2/2b1p3/2B1P1b1/P1PPBN2/1P1NQPPP/R4RK1 w - - 0 11";
    std::string logPath = "/Users/john/VS Code Projects/C++/chess-engine/log.txt";

    // ChessEngine engine(fen, logPath);
    // SearchResult result = engine.bestMove(1000);
    // std::cout << "Selected: " << result.move << " - " << result.score << std::endl;

    ChessEngine engine(fen, logPath);
    engine.uciLoop();

    return 0;
}
