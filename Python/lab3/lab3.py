from abc import ABC, abstractmethod

HERBI = 'herbivorous'
OMNI = 'omnivorous'
CARNI = 'carnivorous'

# -------- Abstract --------

class Animal(ABC):

    idgen = 100

    def __init__(self, mass: float):
        global idgen
        self.id = Animal.idgen
        Animal.idgen += 10
        self.mass = mass

    @abstractmethod
    def count_food(self):
        return self.mass

    def count_meat(self):
        return self.count_food()[0]

    def count_grass(self):
        return self.count_food()[1]

    def __str__(self):
        return f"animal id={self.id} mass={self.mass}"

    def __add__(self, other) -> tuple[float,float]:
        m1,g1 = self.count_food()
        m2,g2 = other.count_food()
        return (m1+m2,g1+g2)

    def __sub__(self, other):
        m1,g1 = self.count_food()
        if isinstance(other,Animal):
            m2,g2 = other.count_food()
        elif isinstance(other,int) or isinstance(other,float):
            m2,g2 = other, other
        return (m1-m2,g1-g2)

    def __mul__(self, other):
        m1,g1 = self.count_food()
        m2,g2 = other.count_food()
        return (m1*m2,g1*g2)

    def __truediv__(self, other):
        m1,g1 = self.count_food()
        m2,g2 = other.count_food()
        return (m1/m2,g1/g2)

# ------- 3 types ---------

class Carni(Animal):
    ftype = CARNI
    
    def __init__(self, mass):
        super().__init__(mass)

    def count_food(self):
        return (super().count_food()*0.05, 0)

class Herbi(Animal):
    ftype = HERBI

    def __init__(self, mass):
        super().__init__(mass)
        
    def count_food(self):
        return (0, 0.1*super().count_food())

class Omni(Animal):
    ftype = OMNI

    def __init__(self, mass):
        super().__init__(mass)

    def count_food(self):
        mass = super().count_food()
        return (mass*0.03, 0.03*mass)

# -------- species -----------
        
class Deer(Herbi):
    species = 'Deer'

    def __init__(self, name, mass=120):
        super().__init__(mass)
        self.name = name

    def __str__(self):
        prev = super().__str__()
        return f"{prev} {HERBI} {Wolf.species} name={self.name}"


class Wolf(Carni):
    species = 'Wolf'

    def __init__(self, name, mass=70):
        super().__init__(mass)
        self.name = name

    def __str__(self):
        prev = super().__str__()
        return f"{prev} {CARNI} {Wolf.species} name={self.name} food={self.count_food()}"

class Gorilla(Omni):
    species = 'Gorilla'

    def __init__(self, name, mass=150):
        super().__init__(mass)
        self.name = name

    def __str__(self):
        prev = super().__str__()
        return f"{prev} {OMNI} {Gorilla.species} name={self.name} food={self.count_food()}"

# ==============================


def compare(a: Animal):
    return a.count_food()

# ------- program ------------

zoo = [ Gorilla('Jack'), Gorilla('Micky'), Wolf('King'), Deer('Gjhj') ]

meat, grass = 0, 0
for animal in zoo:
    m,g = animal.count_food()
    meat += m
    grass += g
print(meat, grass)



sorted_zoo = zoo[:]
sorted_zoo.sort(key=compare)
for x in sorted_zoo:
    print(x)

print(zoo[1]+zoo[2])
