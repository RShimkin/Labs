"""
Входной язык содержит арифметические выражения, разделенные символом ; (точка с запятой). 
Арифметические выражения состоят  из идентификаторов, символьных констант (один символ  в  одинарных  кавычках),
знака  присваивания  (:=),  знаков  операций +, -, *, / и  круглых скобок
"""
import string
 
 
COMMENT = '//'
NEWLINE = '\n'
LBRACKET = '('
RBRACKET = ')'
QUOTE = "'"
PLUS = '+'
MINUS = '-'
STAR = '*'
SLASH = '/'
DEF = ':='
SEMICOLON = ';'
STATE_START = 0                     # +
STATE_IDENTIFIER = 1                # +
STATE_DEF = 2                       # +
STATE_VALUE = 3                     # +
STATE_LBRACKET = 4                  # +
STATE_VARIABLE = 5                  # +
STATE_CHAR = 6                      # +
STATE_OP = 7                        # +
STATE_RBRACKET = 8                  # +
STATE_SEMICOLON = 9                 # +
STATE_COMMENT = 10                  # +
STATE_END = 11                      # +
 
 
class Tokenizer(object):
    def __init__(self, file):
        self.lexems = []
 
        with open(file, 'r') as f:
            self.source = f.read()
 
        self.state = STATE_START
        self.i = 0
        self.ident_start_sym = '_' + string.ascii_letters
        self.ident_all_sym = self.ident_start_sym + string.digits
 
    def find_next(self):
        while self.i < len(self.source) and self.source[self.i] in (' ', '\n', '\t'):
            if self.source[self.i] == '\n':
                self.lexems.append((NEWLINE, NEWLINE))
            self.i += 1
 
    def tokenize(self):
        while True:
            if self.state == STATE_START:
                self.state = STATE_IDENTIFIER
 
            elif self.state == STATE_IDENTIFIER:
                self.find_next()
 
                if self.source[self.i] not in self.ident_start_sym:
                    return 'Неверное имя идентификатора'
 
                j = self.i + 1
                while self.source[j] in self.ident_all_sym:
                    j += 1
 
                name = self.source[self.i:j]
                self.lexems.append(('Определение переменной', name))
 
                self.state = STATE_DEF
                self.i = j
 
            elif self.state == STATE_DEF:
                self.find_next()
                if self.source[self.i:self.i+2] == DEF:
                    self.lexems.append(('Присваивание', DEF))
 
                else:
                    return 'Неверный символ (ожидалось `:=`)'
 
                self.i += 2
                self.find_next()
                if self.source[self.i] in (string.digits + MINUS):
                    self.state = STATE_VALUE
                elif self.source[self.i] == LBRACKET:
                    self.state = STATE_LBRACKET
                elif self.source[self.i] in self.ident_start_sym:
                    self.state = STATE_VARIABLE
                elif self.source[self.i] == QUOTE:
                    self.state = STATE_CHAR
                else:
                    return 'Неизвестный переход'
 
            elif self.state == STATE_VALUE:
                self.find_next()
                j = self.i
                if self.source[j] not in (string.digits + MINUS):
                    return 'Ожидалось числовое значение'
 
                if self.source[j] == '-':
                    j += 1
 
                while self.source[j] in string.digits:
                    j += 1
 
                value = self.source[self.i:j]
                if value == MINUS:
                    return 'Ожидалось числовое значение'
 
                self.lexems.append(('Числовое значение', value))
                self.i = j
                self.find_next()
                if self.source[self.i] in [PLUS, MINUS, STAR, SLASH]:
                    self.state = STATE_OP
                elif self.source[self.i] == RBRACKET:
                    self.state = STATE_RBRACKET
                elif self.source[self.i] == SEMICOLON:
                    self.state = STATE_SEMICOLON
                else:
                    return 'Неизвестный переход'
 
            elif self.state == STATE_LBRACKET:
                self.find_next()
                self.lexems.append(('Открывающая скобка', LBRACKET))
 
                self.i += 1
                self.find_next()
                if self.source[self.i] in self.ident_start_sym:
                    self.state = STATE_VARIABLE
                elif self.source[self.i] in string.digits + MINUS:
                    self.state = STATE_VALUE
                else:
                    return 'Неизвестный переход'
 
            elif self.state == STATE_VARIABLE:
                self.find_next()
                if self.source[self.i] not in self.ident_start_sym:
                    return 'Неверное имя переменной'
 
                j = self.i + 1
                while self.source[j] in self.ident_all_sym:
                    j += 1
 
                var = self.source[self.i:j]
 
                if var == '-':
                    return 'Неверное имя переменной'
 
                self.lexems.append(('Обращение к переменной', var))
 
                self.i += len(var)
                self.find_next()
                if self.source[self.i] in (PLUS, MINUS, STAR, SLASH):
                    self.state = STATE_OP
                elif self.source[self.i] == RBRACKET:
                    self.state = STATE_RBRACKET
                elif self.source[self.i] == SEMICOLON:
                    self.state = STATE_SEMICOLON
                else:
                    return 'Неизвестный переход'
 
            elif self.state == STATE_CHAR:
                self.find_next()
                if self.source[self.i] == QUOTE \
                    and self.source[self.i + 1] in string.ascii_letters \
                    and self.source[self.i + 2] == QUOTE:
 
                    self.lexems.append(('Cимвольный литерал', self.source[self.i + 1]))
                    self.i += 3
                    self.state = STATE_SEMICOLON
 
                else:
                    return 'Неверное определение символьного литерала'
 
            elif self.state == STATE_OP:
                self.find_next()
 
                op = self.source[self.i]
 
                if op == PLUS:
                    self.lexems.append(('Операция сложения', PLUS))
                elif op == MINUS:
                    self.lexems.append(('Операция вычитания', MINUS))
                elif op == STAR:
                    self.lexems.append(('Операция умножения', STAR))
                elif op == SLASH:
                    self.lexems.append(('Операция деления', SLASH))
                else:
                    return 'Неверный символ операции (Ожидалось: `+`, `-`, `*`, `/`)'
 
                self.i += 1
                self.find_next()
                if self.source[self.i] == LBRACKET:
                    self.state = STATE_LBRACKET
                elif self.source[self.i] in string.digits + MINUS:
                    self.state = STATE_VALUE
                elif self.source[self.i] in self.ident_start_sym:
                    self.state = STATE_VARIABLE
 
                else:
                    return 'Неизвестный переход'
 
            elif self.state == STATE_RBRACKET:
                self.find_next()
                if self.source[self.i] == RBRACKET:
                    self.lexems.append(('Закрывающая скобка', RBRACKET))
                else:
                    return 'Ожидалась закрывающая скобка'
 
                self.i += 1
                self.find_next()
                if self.source[self.i] in (PLUS, MINUS, STAR, SLASH):
                    self.state = STATE_OP
                elif self.source[self.i] == SEMICOLON:
                    self.state = STATE_SEMICOLON
                elif self.source[self.i] == RBRACKET:
                    pass
                else:
                    return 'Неизвестный переход'
 
            elif self.state == STATE_SEMICOLON:
                self.find_next()
                if self.source[self.i] == SEMICOLON:
                    self.lexems.append(('Конец выражения', SEMICOLON))
                else:
                    return 'Не найден конец выражения'
 
                self.i += 1
                self.find_next()
                if self.i >= len(self.source):
                    self.state = STATE_END
                elif self.source[self.i] in self.ident_start_sym:
                    self.state = STATE_IDENTIFIER
                elif self.source[self.i:self.i + len(COMMENT)] == COMMENT:
                    self.state = STATE_COMMENT
 
            elif self.state == STATE_COMMENT:
                j = self.i + len(COMMENT)
                while self.source[j] != NEWLINE:
                    j += 1
 
                self.lexems.append(('Комментарий', self.source[self.i:j]))
                self.i = j
 
                self.find_next()
                if self.i >= len(self.source):
                    self.state = STATE_END
                else:
                    self.state = STATE_IDENTIFIER
 
            elif self.state == STATE_END:
                self.lexems.append(('Конец программы', ''))
                return
 
            else:
                return 'Это никогда не должно произойти'
 
    def print(self):
        max_width = max(len(lexem[0]) for lexem in self.lexems if lexem != NEWLINE)
        for lexem in self.lexems:
            if lexem[0] != NEWLINE:
                print(Tokenizer.to_len(lexem[0], max_width), '|', lexem[1])
 
    @staticmethod
    def to_len(text, len_):
        return text + ' ' * (len_ - len(text))
 
    def detokenize(self):
        if self.lexems is None:
            return ''
        res = ''
        for t, l in self.lexems:
            if l in (DEF, PLUS, MINUS, STAR, SLASH):
                res += ' %s ' % l
            elif l == SEMICOLON:
                res += '%s ' % l
            elif t == 'Cимвольный литерал':
                res += "'%s'" % l
            else:
                res += l
 
        return res
 
if __name__ == '__main__':
    fname = input('Input file name: ')
    t = Tokenizer(fname)
    res = t.tokenize()
 
    if res is not None:
        print("Ошибка:", res)
    else:
        t.print()
        pass
 
    print(t.detokenize())