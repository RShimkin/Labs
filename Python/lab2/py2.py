# Шимкин Р.Р. 62 группа 3 курс
# вариант 16 

def task(data: str) -> []:

    def find_next(now,words):

        def isrus(sym):
            rsf = ord('а')
            rsl = ord('я')
            rbf = ord('А')
            rbl = ord('Я')

            if sym in range(rsf,rsl+1) or sym in range(rbf,rbl+1):
                return True
            return False

        def iseng(sym):
            esf = ord('a')
            esl = ord('z')
            ebf = ord('A')
            ebl = ord('Z')

            if sym in range(esf,esl+1) or sym in range(ebf,ebl+1):
                return True
            return False

        if len(words) == 0:
            return []

        eng = True
        sym = ord(now[-1])

        if isrus(sym):
    
            eng = False
            offset = sym - ord('а')
            if offset > 33 or offset < 0:
                offset = sym - ord('А')
                    
        elif iseng(sym):

            offset = sym - ord('a')
            if offset > 26 or offset < 0:
                offset = sym - ord('A')

        else:
            raise Exception("Странный язык")

        for nextw in words:

            sym = nextw[0]
            os = ord(sym)
            of = offset

            if  eng and (os == ord('a') + of or os == ord('A') + of) or not eng and (os == ord('а') + of or os == ord('А') + of):
                neww = nextw
                tmp = words[:]
                tmp.remove(nextw)
                res = find_next(neww,tmp)
                if res != False:
                    res.append(neww)
                    return res
       
            
        return False


    #------------------------------------------------
    
    words = data.split(' ')
    print("WORDS:", words)
    if len(words) == 1:
        return words
    chain = False

    for first in words:

        res = []
        tmp = words[:]
        tmp.remove(first)
        cur = first

        chain = find_next(cur,tmp)
        if chain:
            chain.append(first)
            chain.reverse()
            return chain

        
    return False

#----- Основная функция ---------------------
def main():
    
    fname = input("Введите название файла: ")
    data = None
    try:
        with open(fname,'r', encoding='utf-8') as file:
            data = file.read()
            if data:
                print("DATA:", data)
                res = task(data)
                if res:
                    print("CHAIN:", res)
                else:
                    print("Не получилось составить цепочку...")
    except Exception as e:
        print(e)
        

    
    choice = input("Снова? ")
    if choice:
        print()
        main()
#-----------------------------

if __name__ == "__main__":
    main()
