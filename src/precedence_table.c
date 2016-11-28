#include "precedence_table.h"
#include "expression.h"

const TokenPrecedence precedence_table[][STT_STRING] = {
    [EO_PLUS] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_MINUS] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_MULTIPLY] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_DIVIDE] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LESS] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_GREATER] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LESS_EQUALS] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_GREATER_EQUALS] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LOGIC_EQUAL] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LOGIC_NOT_EQUAL]   = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_AND]               = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = L,
        [EO_LOGIC_NOT_EQUAL] = L,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_OR]                = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = L,
        [EO_LOGIC_NOT_EQUAL] = L,
        [EO_AND] = L,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_NEGATE] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LEFT_PARENTHESE]   = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = L,
        [EO_LOGIC_NOT_EQUAL] = L,
        [EO_AND] = L,
        [EO_OR] = L,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = E
    },
    [EO_RIGHT_PARENTHESE]  = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = H,
        [EO_LEFT_PARENTHESE] = N,
        [EO_RIGHT_PARENTHESE] = H
    },
};