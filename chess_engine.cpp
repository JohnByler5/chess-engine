#include <iostream>
#include "chess.hpp"

using namespace chess;

const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum PieceValue {
    PAWN = 100,
    BISHOP = 300,
    KNIGHT = 300,
    ROOK = 500,
    QUEEN = 900,
};

class ChessEngine {
private:
    Board board;
    
public:
    ChessEngine(const std::string& fen)
        : board(Board(fen)) {}

    int evaluate() {
        int value = 0;

        Color color = board.sideToMove();
        Bitboard pawns = board.pieces(PieceType::PAWN, color);
        Bitboard knights = board.pieces(PieceType::KNIGHT, color);
        Bitboard bishops = board.pieces(PieceType::BISHOP, color);
        Bitboard rooks = board.pieces(PieceType::ROOK, color);
        Bitboard queens = board.pieces(PieceType::QUEEN, color);

        value += pawns.count() * PieceValue::PAWN;
        value += knights.count() * PieceValue::PAWN;
        value += bishops.count() * PieceValue::PAWN;
        value += rooks.count() * PieceValue::PAWN;
        value += queens.count() * PieceValue::PAWN;

        color = !color;
        pawns = board.pieces(PieceType::PAWN, color);
        knights = board.pieces(PieceType::KNIGHT, color);
        bishops = board.pieces(PieceType::BISHOP, color);
        rooks = board.pieces(PieceType::ROOK, color);
        queens = board.pieces(PieceType::QUEEN, color);

        value -= pawns.count() * PieceValue::PAWN;
        value -= knights.count() * PieceValue::PAWN;
        value -= bishops.count() * PieceValue::PAWN;
        value -= rooks.count() * PieceValue::PAWN;
        value -= queens.count() * PieceValue::PAWN;

        return value;
    }

    Move bestMove() {
        Movelist moves;
        movegen::legalmoves(moves, board);

        int bestValue = INT_MIN;
        Move bestMove;
        for (const Move &move : moves) {
            board.makeMove(move);
            int value = evaluate();
            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
            board.unmakeMove(move);
        }

        return bestMove;
    }

    void uciLoop() {
        std::string line;
        while (getline(std::cin, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "uci") {
                handleUci();
            } else if (token == "isready") {
                handleIsReady();
            } else if (token == "position") {
                handlePosition(iss);
            } else if (token == "go") {
                handleGo();
            } else if (token == "ucinewgame") {
                handleUciNewGame();
            } else if (token == "quit") {
                break;
            }
        }
    }

    void handleUci() {
        std::cout << "id name MyChessEngine" << std::endl;
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
            fen =START_FEN;
            iss >> token;
        } else if (token == "fen") {
            while (iss >> token && token != "moves") {
                fen += token + " ";
            }
        }

        board.setFen(fen);

        while (iss >> token) {
            Move move = uci::uciToMove(board, token);
            if (move != Move::NULL_MOVE) {
                board.makeMove(move);
            }
        }
    }

    void handleGo() {
        Move move = bestMove();
        std::cout << "bestmove " << uci::moveToUci(move) << std::endl;
    }

    void handleUciNewGame() {
        board = Board(START_FEN);
    }
};

int main() {
    ChessEngine engine(START_FEN);

    return 0;
}
