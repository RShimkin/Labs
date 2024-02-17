import random as rand

def init_arrs(n):
    return ( [] for _ in range(0,n))

def make_rand(rank=10, size=10, bot=1):
    
    b = [ float(rand.random()*size + bot) for _ in range(0,rank) ]
    a = [ float(rand.random()*size + bot) for _ in range(1,rank) ]
    a.insert(0,0)
    c = [ float(rand.random()*size + bot) for _ in range(0,rank-1) ]
    c.append(0)
    q = [ float(rand.random()*size + bot) for _ in range(0,rank) ]
    k = rand.randint(0,rank-1)
    q[k-1] = c[k-1]
    q[k] = b[k]
    q[k+1] = a[k+1]
    f = [ float(rand.random()*size + bot) for _ in range(0,rank) ]
    
    return b,a,c,q,f,k

def make_matr(b,a,c,q,f,k,fro=0,to=None):
    
    def make_empty(n):
        return [ 0.0 for _ in range(0,n) ]

    def st(arg):
        if arg < 0:
            return "{:3.1f}  ".format(arg) 
        else:
            return "{:4.2f}  ".format(arg) 
    
    matr = [ make_empty(len(b)) for _ in range(0,len(b)) ]
    
    for i in range(0,len(b)):
        matr[i][i] = b[i]
        if i > 0:
            matr[i][i-1] = a[i]
        if i < len(b)-1:
            matr[i][i+1] = c[i]
        matr[i][k] = q[i]

    nums = [ x for x in range(1,len(b)+1) ]
    str = "    "
    for i in range(0,len(b)):
        str += " {:2}   ".format(nums[i])
    print(str)

    if to==None:
        to = len(b)
    for i in range(fro,to):
        str = "{:2}  ".format(nums[i])
        for ii in range(0,len(b)):
            str += st(matr[i][ii])
        str += "{:4.2f}".format(f[i])
        print(str)
        print()
    print()

def mult(b,a,c,q,x,k):
    
    res=[]
    
    for i in range(0,len(b)):
        summ = 0
        
        if i != 0:
            summ += a[i]*x[i-1]
            
        summ += b[i]*x[i]
        
        if i != len(b)-1:
            summ += c[i]*x[i+1]
            
        tmp = q[i]*x[k]
        if i not in (k,k-1,k+1):
            summ += tmp
            
        res.append(summ)
        
    return res

def simplify(b,a,c,q,f,F,k, debug):

    ph = [ i for i in range(0,len(b)) ]
    
    for i in range(0,k):
        if debug:
            make_matr(b,a,c,q,f,k,i,i+2)

        #
        if b[i] != 0:
            if b[i] != 1:
                div = b[i]
                b[i] = 1
                c[i] /= div
                if i == k - 1:
                    q[i] = c[i]
                else:
                    q[i] /= div
                f[i] /= div
                F[i] /= div
                
            if debug:
                make_matr(b,a,c,q,f,k,i,i+2)
                
            koef = a[i+1]
            a[i+1] = 0
            b[i+1] -= c[i] * koef
            if i == k - 1:
                q[i+1] = b[i+1]
            else:
                q[i+1] -= q[i] * koef
            f[i+1] -= f[i] * koef
            F[i+1] -= F[i] * koef
            if i == k - 2:
                c[i+1] = q[i+1]

            if debug:
                make_matr(b,a,c,q,f,k,0,len(b))

        elif c[i] != 0:
            p = i + 1
            
            koef = b[p] / c[i]
            b[p] = 0
            q[p] -= q[i] * koef
            f[p] -= f[i] * koef
            F[p] -= F[i] * koef

            p = i + 2
            koef = a[p] / c[i]
            a[p] = 0
            q[p] -= q[i] * koef
            f[p] -= f[i] * koef
            F[p] -= F[i] * koef

            ph[i] = i + 1
            ph[i+1] = i

            b[i] = c[i]
            c[i] = 0
            b[i+1] = a[i+1]
            a[i+1] = 0
            
        else:
            raise Exception("0 в векторах a и c")        
        #
            
    if debug:
        make_matr(b,a,c,q,f,k,0,len(b))
        
    for i in range(k,len(b)-1):
        
        if debug:
            make_matr(b,a,c,q,f,k,i,len(b))
            
        if q[i] != 1:
            div = q[i]
            q[i] = b[i] = 1
            c[i] /= div
            f[i] /= div
            F[i] /= div
        if (i+1) < len(q):
            koef = q[i+1]
            a[i+1] = 0
            q[i+1] = b[i+1] = b[i+1] - c[i]*koef
            f[i+1] -= f[i]*koef
            F[i+1] -= F[i]*koef
        if (i+2) < len(q):
            koef = q[i+2]
            q[i+2] = a[i+2] = a[i+2] - c[i]*koef
            f[i+2] -= f[i]*koef
            F[i+2] -= F[i]*koef
        if (i+3) < len(q):
            for ii in range(i+3,len(q)):
                f[ii] -= f[i] * q[ii]
                F[ii] -= F[i] * q[ii]
                q[ii] = -c[i] * q[ii]
                            
    if debug:
        make_matr(b,a,c,q,f,k,0,len(b))
    return (b,a,c,q,f,F,ph)

def solve(b,a,c,q,f,F,k,ph):
    
    #x,y = init_arrs(2)
    x = [ 0 for _ in range(0,len(b)) ]
    y = [ 0 for _ in range(0,len(b)) ]
    prevx = 0
    prevy = 0
    
    for i in range(len(b)-1, k - 2, -1):
            
        temp = (f[i] - c[i] * prevx) / b[i]
        x[ph[i]] = temp
        prevx = temp
        temp = (F[i] - c[i] * prevy) / b[i]
        y[ph[i]] = temp
        prevy = temp
        
    xq = x[k]
    yq = y[k]
    
    for i in range(k-2, -1, -1):
        
        #prevx = x[-1]
        temp = (f[i] - c[i] * prevx - q[i] * xq) / b[i]
        x[ph[i]] = temp
        prevx = temp
        #prevy = y[-1]
        temp = (F[i] - c[i] * prevy - q[i] * yq) / b[i]
        y[ph[i]] = temp
        prevy = temp
        
    x.reverse()
    return (x,y)

def task(b,a,c,q,f,F,k, debug):
    
    x = []
    try:
        b,a,c,q,f,F,ph = simplify(b,a,c,q,f,F,k, debug)
        x,X = solve(b,a,c,q,f,F,k,ph)
        #x,X = solve(b,a,c,q,f,F,k)
    except Exception as e:
        print("Ошибка вычислений!")
        print(e.message)

    return (x,X)

def fread_matr(name,debug):
    b,a,c,q,f = init_arrs(5)

    def parse(string):
        parts = string.split(' ')
        arr = [ float(x) for x in parts ]
        return arr

    try:
        with open(name) as file:
            lines = file.readlines()

            rank = int(lines[0])
            k = int(lines[-1]) - 1
            a.append(0)
            
            for i in range(1,len(lines)-1):
                arr = parse(lines[i])
                if i > 1:
                    a.append(arr[i-2])
                b.append(arr[i-1])
                if i < rank:
                    c.append(arr[i])
                q.append(arr[k])
                f.append(arr[-1])
            c.append(0)
            if debug:
                make_matr(b,a,c,q,f,k)
    except:
        print("Ошибка открытия файла!")
        return False

    return (b,a,c,q,f,k)

def main():
    
    check = False
    debug = False

    fname = input("Имя файла: ")
    if fname == "random":
        b,a,c,q,f,k = make_rand(10,20,-10)
        make_matr(b,a,c,q,f,k)
    else:
        b,a,c,q,f,k = fread_matr(fname, True)
        
    X = [ 1 for i in range(0,len(b))]
    F = mult(b,a,c,q,X, k)
    x,y = task(b,a,c,q,f,F,k, debug)
    rx = [ round(el,3) for el in x ]
    print(rx)
    #print(y)
    delta = max([ abs(i-1) for i in y ])
    print("delta:", delta)
    
    if check:

        b,a,c,q,f,k = fread_matr(fname, False)
        bigf = [ i + 0.05 for i in f ]
        smallf = [ i - 0.05 for i in f ]
        res = mult(b,a,c,q,x, k)
        eq = True
        for i in range(0,len(res)):
            if not smallf[i] < res[i] < bigf[i]:
                eq = False
                break
        if eq:
            print("ВЕРНО")
        else:
            print("НЕВЕРНО")
        
    
    if input("Снова? "):
        main()

if __name__ == "__main__":
    main()

