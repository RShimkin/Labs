   
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
            print(1,summ)
            
        summ += b[i]*x[i]
        
        if i != len(b)-1:
            summ += c[i]*x[i+1]
            
        tmp = q[i]*x[k]
        if i not in (k,k-1,k+1):
            summ += tmp
            
        res.append(summ)
        
    return res

def simplify(b,a,c,q,f,k, debug):
    
    for i in range(0,k):
        if debug:
            make_matr(b,a,c,q,f,k,i,i+2)
        
        if b[i] != 1 and b[i] != 0:
            div = b[i]
            b[i] = 1
            c[i] /= div
            if i == k - 1:
                q[i] = c[i]
            else:
                q[i] /= div
            f[i] /= div
        if b[i] == 0:
            raise Exception("0 в векторе b")
                            
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
        if i == k - 2:
            c[i+1] = q[i+1]
            
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
        if (i+1) < len(q):
            koef = q[i+1]
            a[i+1] = 0
            q[i+1] = b[i+1] = b[i+1] - c[i]*koef
            f[i+1] -= f[i]*koef 
        if (i+2) < len(q):
            koef = q[i+2]
            q[i+2] = a[i+2] = a[i+2] - c[i]*koef
            f[i+2] -= f[i]*koef
        if (i+3) < len(q):
            for ii in range(i+3,len(q)):
                f[ii] -= f[i] * q[ii]
                q[ii] = -c[i] * q[ii]
                            
    if debug:
        make_matr(b,a,c,q,f,k,0,len(b))

def solve(b,a,c,q,f,k):
    
    x = []
    
    for i in range(len(b)-1, k - 2, -1):
        num = len(x)
        if num:
            prev = x[num-1]
        else:
            prev = 0
            
        temp = (f[i] - c[i] * prev) / b[i]
        x.append(temp)
        
    xq = x[len(x) - 2]
    for i in range(k-2, -1, -1):
        prev = x[len(x)-1]
        temp = (f[i] - c[i] * prev - q[i] * xq) / b[i]
        x.append(temp)
        
    x.reverse()
    return x

def task(b,a,c,q,f,k, debug):
    
    x = []
    try:
        simplify(b,a,c,q,f,k, debug)
        x = solve(b,a,c,q,f,k)
    except Exception as e:
        print("Ошибка вычислений!")
        print(e.message)

    return x



def fread(name, debug):
    global b,a,c,q,f,k

    def parse(string):
        parts = string.split('=')
        arr = [ float(x) for x in parts[1].split(',') ]
        return arr

    try:
        with open(name) as file:
            lines = file.readlines()
            b = parse(lines[1])
            a = parse(lines[2])
            c = parse(lines[3])
            q = parse(lines[4])
            f = parse(lines[5])
            k = int(lines[6].split('=')[1])
        k -= 1
        if debug:
            make_matr(b,a,c,q,f,k)
    except:
        print("Ошибка открытия файла!")
        return False

    return True

def fread_matr(name,debug):
    global b,a,c,q,f,k

    def parse(string):
        parts = string.split(' ')
        arr = [ float(x) for x in parts ]
        return arr

    try:
        with open(name) as file:
            lines = file.readlines()

            for i in (0,len(lines)):
                arr = parse(lines[i])
                if i > 0:
                    a[i] = 
            
            b = parse(lines[1])
            a = parse(lines[2])
            c = parse(lines[3])
            q = parse(lines[4])
            f = parse(lines[5])
            k = int(lines[6].split('=')[1])
        k -= 1
        if debug:
            make_matr(b,a,c,q,f,k)
    except:
        print("Ошибка открытия файла!")
        return False

def main():
    global b,a,c,q,f,k
    check = True
    debug = False

    fname = input("Имя файла: ")
    if fread(fname, True):
        x = task(b,a,c,q,f,k, debug)
        print(x)

        if check:
            fread(fname, True)
            bigf = [ i + 0.05 for i in f ]
            smallf = [ i - 0.05 for i in f ]
            res = mult(b,a,c,q,x, k)
            eq = True
            print(f)
            print(res)
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

